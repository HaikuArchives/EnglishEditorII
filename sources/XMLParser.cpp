/* XMLParser.cpp */

#include "XMLParser.h"
#include "Document.h"
#include "Element.h"
#include "Attr.h"
#include "Text.h"
#include "EntityReference.h"
#include "Comment.h"
#include "ProcessingInstruction.h"
#include "CDATASection.h"
#include "InvalidXMLException.h"



XMLParser::XMLParser(string_slice textIn)
	: text(textIn)
{
}


Document* XMLParser::Parse()
{
	document = new Document();
	ParseInto(document);
	return document;
}


void XMLParser::ParseInto(Document* documentIn)
	// useful when running the parser in its own thread while accessing
	// the document in another thread (assuming one has make thread-safe nodes)
{
	document = documentIn;
	p = text.begin();
	ParsePCDataInto(document, "");
}


void XMLParser::ParsePCDataInto(Node* node, string_slice elementName)
	// read PCData, until end-tag or EOF
{
	const char* stopper = text.end();

	while (p < stopper) {
		// skip whitespace
		const char* fullTextStart = p;
		SkipWhitespace();
		if (p >= stopper)
			break;

		// tags, etc.
		if (*p == '<') {
			// handle ignorable whitespace
			if (p > fullTextStart)
				node->AppendChild(document->CreateTextNode(string_slice(fullTextStart, p)));

			string_slice restOfText(p, stopper);

			if (restOfText.startsWith("<?"))
				node->AppendChild(ParseProcessingInstruction());

			else if (restOfText.startsWith("<!--"))
				node->AppendChild(ParseComment());

			else if (restOfText.startsWith("<![CDATA["))
				node->AppendChild(ParseCDATASection());

			else if (restOfText.startsWith("<!DOCTYPE")) {
				p += 9;	// skip "<!DOCTYPE"
				// we don't handle this--just try to skip it
				int bracketLevel = 1;
				for (; p < stopper; p++) {
					char c = *p;
					if (c == '<')
						bracketLevel++;
					else if (c == '>') {
						--bracketLevel;
						if (bracketLevel <= 0) {
							p++;	// skip '>'
							break;
							}
						}
					else if (c == '\'' || c == '"') {
						p++;	// skip the quotechar
						// something's quoted--skip it
						for (; p < stopper; p++) {
							if (*p == c)
								break;
							}
						// the trailing quotechar will get skipped by the enclosing loop
						}
					}
				}

			else if (restOfText.startsWith("</")) {
				// end tag
				const char* endTagStart = p;
				if (ParseEndTag() != elementName) {
					// wrong end tag; implicitly close this element and
					// rewind "p" so the enclosing element can see its end tag
					p = endTagStart;
					}
				// done with this element
				return;
				}

			else {
				// start tag--add the element
				node->AppendChild(ParseElement());
				}
			}

		// text
		else {
			p = fullTextStart;	// we need the whitespace too; rewind to get it
			ParseTextInto(node);			
			}
		}
}


Element* XMLParser::ParseElement()
{
	// parse the start tag
	p++;	// skip the '<'
	const char* stopper = text.end();
	// read the name
	const char* nameStart = p;
	for (; p < stopper; p++) {
		char c = *p;
		if (c == ' ' || c == '\t' || c == '\n' || c == '\r' || c == '>' || c == '/')
			break;
		}
	string_slice name(nameStart, p);

	// create the element
	Element* element = document->CreateElement(name);

	// read the attributes
	while (p < stopper && *p != '>' && *p != '/') {
		SkipWhitespace();
		if (*p == '>' || *p == '/')
			break;
		element->SetAttributeNode(ParseAttribute());
		}

	// read the end of the tag
	bool isEmptyElement = false;
	if (*p == '/') {
		p++;
		isEmptyElement = true;
		}
	if (*p != '>')
		throw InvalidXMLException(text, p);
	p++;

	// read the tag contents
	if (!isEmptyElement)
		ParsePCDataInto(element, element->NodeName());

	return element;
}


void XMLParser::ParseTextInto(Node* node, char terminator)
{
	const char* stopper = text.end();
	DOMString data;

	const char* textStart = p;
	while (p < stopper) {
		if (*p == terminator)
			break;

		// handle entity
		if (*p == '&') {
			// add to current literal text
			if (p > textStart)
				data += string_slice(textStart, p);

			// read the entity
			const char* entityNameStart = ++p;	// skip '&'
			for (; ; p++) {
				if (p >= stopper) {
					// unclosed entity
					throw InvalidXMLException(this->text, entityNameStart);
					}

				if (*p == ';') {
					string_slice entityName(entityNameStart, p);
					if (*entityName.begin() == '#') {
						entityName.eraseFirst(1);
						long charValue;
						if (*entityName.begin() == 'x') {
							entityName.eraseFirst(1);
							charValue = ParseHex(entityName);
							}
						else
							charValue = entityName.asInt();
						data += CharToUTF8(charValue);
						}
					else if (entityName == "amp")
						data += "&";
					else if (entityName == "lt")
						data += "<";
					else if (entityName == "gt")
						data += ">";
					else if (entityName == "apos")
						data += "\'";
					else if (entityName == "quot")
						data += "\"";
					else {
						// a normal entity reference
						// finish current literal text
						node->AppendChild(document->CreateTextNode(data));
						data = "";
						// add the entity reference
						node->AppendChild(document->CreateEntityReference(string_slice(entityNameStart, p)));
						}
					++p;	// skip ';'
					break;
					}
				}

			// start next literal text
			textStart = p;
			}

		else
			p++;
		}

	// close final literal text
	if (p > textStart)
		data += string_slice(textStart, p);
	if (!data.empty())
		node->AppendChild(document->CreateTextNode(data));
}


ProcessingInstruction* XMLParser::ParseProcessingInstruction()
{
	p += 2;		// skip the "<?"

	// find the target
	const char* piStart = p;
	SkipWhitespace();
	const char* targetStart = p;
	const char* stopper = text.end() - 1;
	for (; p < stopper; p++) {
		char c = *p;
		if (c == ' ' || c == '\t' || c == '\n' || c == '\r')
			break;
		if (c == '?' && p[1] == '>')
			break;
		}
	string_slice target(targetStart, p);

	// get the data
	SkipWhitespace();
	const char* dataStart = p;
	for (; ; p++) {
		if (p >= stopper) {
			// unterminated Processing Instruction
			throw InvalidXMLException(text, piStart - 2);
			}
		if (*p == '?' && p[1] == '>')
			break;
		}
	string_slice data(dataStart, p);
	p += 2;		// skip the "?>"

	// make the Processing Instruction
	return document->CreateProcessingInstruction(target, data);
}


Comment* XMLParser::ParseComment()
{
	p += 4;		// skip the "<!--"
	const char* commentStart = p;
	const char* stopper = text.end() - 1;
	for (; p < stopper; p++) {
		if (*p == '-' && p[1] == '-' && p[2] == '>') {
			string_slice comment(commentStart, p);
			p += 3;	// skip the "-->"
			return document->CreateComment(comment);
			}
		}

	// fell thru--unterminated comment
	throw InvalidXMLException(text, commentStart - 4);
}


CDATASection* XMLParser::ParseCDATASection()
{
	p += 9;		// skip the "<![CDATA["
	const char* cdataStart = p;
	const char* stopper = text.end() - 2;
	for (; p < stopper; p++) {
		if (*p == ']' && p[1] == ']' && p[2] == '>') {
			string_slice cdata(cdataStart, p);
			p += 3;	// skip the CDEnd
			return document->CreateCDATASection(cdata);
			}
		}

	// fell thru--unterminated CDATA
	throw InvalidXMLException(text, cdataStart - 9);
}


string_slice XMLParser::ParseEndTag()
{
	p += 2;		// skip "</"
	SkipWhitespace();
	const char* nameStart = p;
	const char* stopper = text.end();
	for (; p < stopper; p++) {
		char c = *p;
		if (c == ' ' || c == '\t' || c == '\n' || c == '\r') {
			// whitespace ends the name
			string_slice name(nameStart, p);
			// read until the tag end character
			for (; p < stopper; p++) {
				if (*p == '>') {
					++p;	// skip '>'
					return name;
					}
				}
			// fell thru--unterminated tag
			throw InvalidXMLException(text, nameStart - 2);
			}

		else if (c == '>') {
			string_slice name(nameStart, p);
			++p;	// skip '>'
			return name;
			}
		}

	// fell thru--unterminated tag
	throw InvalidXMLException(text, nameStart - 2);
}


Attr* XMLParser::ParseAttribute()
{
	const char* stopper = text.end();

	// get the name
	string_slice name;
	const char* nameStart = p;
	for (; ; p++) {
		if (p >= stopper)
			throw InvalidXMLException(text, nameStart);
		if (AtWhitespace()) {
			name = string_slice(nameStart, p);
			SkipWhitespace();
			if (*p != '=' || p >= stopper)
				throw InvalidXMLException(text, p);
			p++;	// skip '='
			SkipWhitespace();
			break;
			}
		else if (*p == '=') {
			name = string_slice(nameStart, p);
			p++;	// skip '='
			SkipWhitespace();
			break;
			}
		}

	// get the value
	Attr* attribute = document->CreateAttribute(name);
	if (*p == '"' || *p == '\'') {
		// normal XML attribute value--we parse this as a Text
		char quoteChar = *p;
		p++;	// skip the quoteChar
		ParseTextInto(attribute, quoteChar);
		p++;	// skip the closing quoteChar
		}
	else {
		// as a special courtesy, we allow non-quoted attribute values--but they
		// must be a single word and can't contain entities
		const char* valueStart = p;
		for (; ; p++) {
			if (p >= stopper)
				throw InvalidXMLException(text, p);	// can't be valid--the tag isn't closed
			char c = *p;
			if (c == ' ' || c == '\t' || c == '\n' || c == '\r' || c == '>' || c == '/')
				break;
			}
		// make the value
		attribute->AppendChild(document->CreateTextNode(string_slice(valueStart, p)));
		}

	return attribute;
}


void XMLParser::SkipWhitespace()
{
	const char* stopper = text.end();
	for (; p < stopper; p++) {
		switch (*p) {
			case ' ':
			case '\t':
			case '\n':
			case '\r':
				break;

			default:
				return;
				break;
			}
		}
}


bool XMLParser::AtWhitespace()
{
	switch (*p) {
		case ' ':
		case '\t':
		case '\n':
		case '\r':
			return true;

		default:
			return false;
		}
}


long XMLParser::ParseHex(string_slice hexStr)
{
	long value = 0;
	const char* stopper = hexStr.end();
	for (const char* p = hexStr.begin(); p < stopper; ++p) {
		char c = *p;
		if (c >= '0' && c <= '9') {
			value *= 16;
			value += c - '0';
			}
		else if (c >= 'A' && c <= 'F') {
			value *= 16;
			value += c - 'A' + 0x0A;
			}
		else if (c >= 'a' && c <= 'f') {
			value *= 16;
			value += c - 'a' + 0x0A;
			}
		}
	return value;
}


DOMString XMLParser::CharToUTF8(int c)
{
	char result[8];
	char* p = &result[0];
	if ((c & ~0x007F) == 0)
		*p++ = c;
	else if ((c & ~0x07FF) == 0) {
		*p++ = 0xC0 | (c >> 6);
		*p++ = 0x80 | (c & 0x003F);
		}
	else {
		*p++ = 0xE0 | (c >> 12);
		*p++ = 0x80 | ((c >> 6) & 0x003F);
		*p++ = 0x80 | (c & 0x003F);
		}
	*p++ = 0;	// terminate the string
	return result;
}


