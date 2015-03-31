/* MessageParser.cpp */

#include "MessageParser.h"
#include "Document.h"
#include "Element.h"
#include "Text.h"
#include "Attr.h"
#include "QuoterDict.h"
#include "InternalException.h"
#include <vector>

class LinesList : public vector<string_slice> {};



MessageParser::MessageParser(string_slice textIn, Document* documentIn)
	: text(textIn), document(documentIn)
{
	p = text.begin();

	// set up the document
	Element* docElement = document->CreateElement("message");
	document->AppendChild(docElement);
	docElement->AppendChild(document->CreateElement("head"));
	docElement->AppendChild(document->CreateElement("body"));
}


void MessageParser::Parse()
{
	string_slice line;

	// get the parts of the document
	Element* docElement = document->DocumentElement();
	Element* head = docElement->GetElementByTagName("head");
	Element* body = docElement->GetElementByTagName("body");

	// read header
	string_slice headerName, headerText;
	Text* lastHeaderText = NULL;
	Element* lastHeader = NULL;
	while (!AtEOF()) {
		// get next line and check for end of headers
		line = GetLine();
		if (IsBlank(line))
			break;

		// handle continuation of headers
		if (line[0] == ' ' || line[0] == '\t') {
			if (lastHeaderText) {
				lastHeaderText->AppendData("\n");
				lastHeaderText->AppendData(line);
				}
			else if (lastHeader) {
				lastHeaderText = document->CreateTextNode(line);
				lastHeader->AppendChild(lastHeaderText);
				}
			continue;
			}

		// handle certain headers
		ParseHeader(line, &headerName, &headerText);
		string_slice headerTag = "";
		if (headerName == "Subject")
			headerTag = "title";
		else if (headerName == "From")
			headerTag = "from";
		else if (headerName == "Date")
			headerTag = "date";
		else if (headerName == "Newsgroups")
			headerTag = "newsgroups";
		else if (headerName == "To")
			headerTag = "to";
		else if (headerName == "Message-ID")
			headerTag = "message-id";
		else if (headerName == "References")
			headerTag = "references";
		if (!headerTag.empty()) {
			Element* header = document->CreateElement(headerTag);
			if (!headerText.trim().empty()) {
				lastHeaderText = document->CreateTextNode(headerText);
				header->AppendChild(lastHeaderText);
				}
			else
				lastHeaderText = NULL;
			if (headerTag == "title") {
				// always put the title first
				head->InsertBefore(header, head->FirstChild());
				}
			else
				head->AppendChild(header);
			lastHeader = header;
			}
		else {
			lastHeaderText = NULL;
			lastHeader = NULL;
			}
		}

	// read the message body
	string_slice curQuoter;
	Element* receptacle = body;
	Element* curQuote = NULL;
	LinesList curParaLines;
	QuoterDict quoterDict;
	while (!AtEOF()) {
		line = GetLine();

		// check quoting
		string_slice quoter, lineBody;
		ParseQuote(line, &quoter, &lineBody);
		if (quoter != curQuoter && !IsBlank(lineBody)) {
			// starting a new quote
			if (!quoter.empty()) {
				// check for attribution
				Node* attribution = NULL;
				if (quoter.substr(0, curQuoter.length()) == curQuoter)
					attribution = StripAttribution(&curParaLines);
				// finish off current block
				HandleParas(&curParaLines, receptacle);
				// finish off current quote
				FinishQuote(curQuote, body);
				// start new quote
				Element* newQuote = document->CreateElement("blockquote");
				if (attribution) {
					newQuote->AppendChild(attribution);
					quoterDict.SetAttributionFor(quoter, attribution->CloneNode(true));
					}
				else {
					attribution = quoterDict.GetAttributionFor(quoter);
					if (attribution)
						attribution = attribution->CloneNode(true);
					else {
						attribution = document->CreateElement("attribution");
						attribution->AppendChild(document->CreateTextNode("Somebody wrote:"));
						}
					newQuote->AppendChild(attribution);
					}
				receptacle = newQuote;
				curQuote = newQuote;
				}
			// end of quotes
			else {
				// finish off current block
				HandleParas(&curParaLines, receptacle);
				// finish off current quote
				FinishQuote(curQuote, body);
				// get out of the quote
				receptacle = body;
				curQuote = NULL;
				}
			curQuoter = quoter;
			}

		// add line
		curParaLines.push_back(lineBody);
		}
	// finish off current block
	HandleParas(&curParaLines, receptacle);
	// finish off current quote
	FinishQuote(curQuote, body);
}


string_slice MessageParser::GetLine()
{
	const char* start = p;
	const char* stopper = text.end();
	while (p < stopper) {
		char c = *p++;
		if (c == '\n' || c == '\r') {
//***			string_slice line(start, p - 1);	// strip the '\n' too
			if (p < stopper &&
			    ((c == '\n' && *p == '\r') ||
			     (c == '\r' && *p == '\n'))) {
				// CRLF or LFCR--skip the second char
				p++;
				}
			string_slice line(start, p);
			return line;
			break;
			}
		}

	// last line
	return string_slice(start, p);
}


bool MessageParser::IsBlank(string_slice line)
{
	if (line.empty())
		return true;

	const char* stopper = line.end();
	for (const char* p = line.begin(); p != stopper; p++) {
		char c = *p;
		if (c != ' ' && c != '\t' && c != '\n' && c != '\r')
			return false;
		}

	return true;
}


void MessageParser::ParseHeader(string_slice line, string_slice* headerNameOut, string_slice* headerTextOut)
{
	const char* p = line.begin();
	const char* stopper = line.end();
	char c;

	// get the name
	for (; p < stopper; p++) {
		if (*p == ':') {
			*headerNameOut = string_slice(line.begin(), p);
			p++;
			break;
			}
		}

	// skip whitespace
	for (; p < stopper; p++) {
		c = *p;
		if (c != ' ' && c != '\t')
			break;
		}

	// return the text
	*headerTextOut = string_slice(p, stopper);
}


void MessageParser::ParseQuote(string_slice line, string_slice* quoter, string_slice* lineBody)
{
	const char* p = line.begin();
	const char* stopper = line.end();

	bool haveQuoter = false;
	const char* quoterEnd = p;	// prevent trailing whitespace from becoming part of the "quoter"
	for (; p < stopper; p++) {
		char c = *p;
		if (c == '>' || c == '|' || c == ':') {
			haveQuoter = true;
			quoterEnd = p + 1;
			}
		else if (c != ' ' && c != '\t')
			break;
		}
	if (haveQuoter) {
		*quoter = string_slice(line.begin(), quoterEnd);
		*lineBody = string_slice(p, stopper);
		}
	else {
		*quoter = string_slice();
		*lineBody = line;
		}
}


bool MessageParser::IsAttribution(string_slice line)
{
	// trim trailing whitespace
	int len = line.length();
	while (true) {
		char c = line[len-1];
		if (c != ' ' && c != '\t' && c != '\n' && c != '\r')
			break;
		--len;
		line.resize(len);
		}

	// check end of line for telltale ":" or "writes"
	if (line[len-1] == ':')
		return true;
	else if (len >= 5 && line.substr(len-5) == "writes")
		return true;

	// check for '@' anywhere in the line
	for (const char* p = line.begin(); p != line.end(); p++) {
		if (*p == '@')
			return true;
		}

	return false;
}


Element* MessageParser::StripAttribution(LinesList* lines)
{
	bool haveAttribution = false;
	bool haveLines = false;
	int lastLine = lines->size() - 1;
	int firstLine;
	for (firstLine=lastLine; firstLine>=0; --firstLine) {
		string_slice line = (*lines)[firstLine];

		// skip blank lines at the end, but if we've already seen non-blank lines
		// a blank line means the start of the paragraph
		if (IsBlank(line)) {
			if (haveLines)
				break;
			else {
				lastLine = firstLine - 1;	// trim trailing blank lines while we're at it
				continue;
				}
			}

		// if it's the last line, check the end of the line
		if (!haveLines) {
			haveLines = true;

			// trim trailing whitespace
			int len = line.length();
			while (true) {
				char c = line[len-1];
				if (c != ' ' && c != '\t' && c != '\n' && c != '\r')
					break;
				--len;
				}
			
			// check end of line for telltale ":" or "writes" or "wrote"
			if (line[len-1] == ':')
				haveAttribution = true;
			else if (len >= 6 && line.substr(len-6) == "writes")
				haveAttribution = true;
			else if (len >= 5 && line.substr(len-5) == "wrote")
				haveAttribution = true;
			}

		// check for '@' anywhere in the line, which will also take to indicate attribution
		if (!haveAttribution) {
			for (const char* p = line.begin(); p != line.end(); p++) {
				if (*p == '@') {
					haveAttribution = true;
					break;
					}
				}
			}
		}
	firstLine += 1;		// point at first line of paragraph

	if (!haveAttribution)
		return NULL;

	// move attribution lines to new list
	LinesList attribLines;
	for (int i=firstLine; i<=lastLine; i++)
		attribLines.push_back((*lines)[i]);
	lines->resize(firstLine);

	// get rid of common boring garbage
	// (like "In article <lsdklskd>," or "On 7 Dec 1941 0700 GMT,")
	for (LinesList::iterator l = attribLines.begin(); l != attribLines.end(); ++l) {
		string_slice line = *l;
		if (line.startsWith("In ") || line.startsWith("On ")) {
			// search for a ','
			int commaPos = -1;
			const char* p = line.begin();
			const char* stopper = line.end();
			for (; p < stopper; ++p) {
				if (*p == ',') {
					commaPos = p - line.begin();
					break;
					}
				}
			// if there is one, we wanna kill the text up thru it
			if (commaPos > 0)
				*l = line.substr(commaPos + 1);
			}
		}

	// build paragraph
	Element* attribution = document->CreateElement("attribution");
	FillBlock(attribution, &attribLines);

	return attribution;
}


void MessageParser::FillBlock(Element* element, LinesList* lines)
{
	Text* text;
	Element* newElement;
	char c;

	DOMString linesText;
	for (LinesList::iterator l = lines->begin(); l != lines->end(); ++l) {
//***		linesText += *l;
		string_slice line = *l;

		// split the line into words and handle them one at a time
		const char* p = line.begin();
		const char* stopper = line.end();
		while (p < stopper) {
			// skip whitespace
			const char* glyphStart = p;
			for (; p < stopper; ++p) {
				c = *p;
				if (c != ' ' && c != '\t' && c != '\n' && c != '\r')
					break;
				}
			if (p > glyphStart)
				linesText += string_slice(glyphStart, p);

			if (p >= stopper)
				break;

			// find the word
			glyphStart = p;
			for (; p < stopper; ++p) {
				c = *p;
				if (c == ' ' || c == '\t' || c == '\n' || c == '\r')
					break;
				}
			// get rid of trailing punctuation
			const char* punctStart = p - 1;
			for (; punctStart >= glyphStart; --punctStart) {
				c = *punctStart;
				if (c != '.' && c != ',' && c != '?' && c != '!' &&
						c != ':' && c != ';' && c != '"' && c != '\'') {
					++punctStart;
					break;
					}
				}
			string_slice word(glyphStart, punctStart);
			string_slice trailingPunct(punctStart, p);

			// special handling of words
			if (!word.empty()) {
				string_slice trimmedWord = word;
				if (trimmedWord.startsWith("<") && trimmedWord.endsWith(">"))
					trimmedWord = trimmedWord.substr(1, trimmedWord.length() - 2);
				bool isEmailAddress = IsEmailAddress(trimmedWord);
				if (trimmedWord.startsWith("http:") || trimmedWord.startsWith("URL:") || 
						trimmedWord.startsWith("ftp:") || isEmailAddress) {
					// it's a URL, make a <a>
					// finish off the current text
					if (!linesText.empty()) {
						text = document->CreateTextNode(linesText);
						element->AppendChild(text);
						linesText.erase();
						}
					// build the URL
					DOMString url;
					if (isEmailAddress && !trimmedWord.startsWith("mailto:")) {
						url = "mailto:";
						url += trimmedWord;
						}
					else
						url = trimmedWord;
					// add the <a>
					// (Element::SetAttribute() doesn't work, especially if not attached to a document)
					newElement = document->CreateElement("a");
					Attr* newAttr = document->CreateAttribute("href");
					text = document->CreateTextNode(trimmedWord);
					newAttr->AppendChild(text);
					newElement->SetAttributeNode(newAttr);
					text = document->CreateTextNode(word);
					newElement->AppendChild(text);
					element->AppendChild(newElement);
					}
				else if ((word.startsWith("*") && word.endsWith("*")) ||
				         (word.startsWith("_") && word.endsWith("_"))) {
					// finish off the current text
					if (!linesText.empty()) {
						text = document->CreateTextNode(linesText);
						element->AppendChild(text);
						linesText.erase();
						}
					// make an <i>
					word = word.substr(1, word.length() - 2);
					newElement = document->CreateElement("i");
					text = document->CreateTextNode(word);
					newElement->AppendChild(text);
					element->AppendChild(newElement);
					}

				else
					linesText += word;
				}

			// add the trailingPunct
			if (!trailingPunct.empty())
				linesText += trailingPunct;
			}
		}

	if (!linesText.empty()) {
		text = document->CreateTextNode(linesText);
		element->AppendChild(text);
		}
}


void MessageParser::HandleParas(LinesList* lines, Element* receptacle)
{
	int i;

	// add the paragraphs
	LinesList curParaLines;
	int numLines = lines->size();
	for (i=0; i<numLines; i++) {
		string_slice line = (*lines)[i];
		if (IsBlank(line)) {
			// end of paragraph
			if (!curParaLines.empty())
				receptacle->AppendChild(BuildParagraph(&curParaLines));
			curParaLines.clear();
			}
		else if (line.startsWith("-- ") && line.trim() == "--") {
			// sigdash
			if (!curParaLines.empty())
				receptacle->AppendChild(BuildParagraph(&curParaLines));
			curParaLines.clear();
			Element* signature = document->CreateElement("signature");
			receptacle->AppendChild(signature);
			receptacle = signature;
			}
		else
			curParaLines.push_back(line);
		}
	// finish last paragraph
	if (!curParaLines.empty())
		receptacle->AppendChild(BuildParagraph(&curParaLines));

	// reset lines for the next go-round
	lines->clear();
}


Element* MessageParser::BuildParagraph(LinesList* lines)
{
	Element* para = document->CreateElement("p");
	FillBlock(para, lines);
	return para;
}


Element* MessageParser::BuildParagraph(string_slice text)
{
	Element* para = document->CreateElement("p");
	LinesList lines;
	lines.push_back(text);
	FillBlock(para, &lines);
	return para;
}


void MessageParser::FinishQuote(Element* quote, Element* receptacle)
{
	// see if there really is anything in the quote
	if (quote == NULL)
		return;
	if (quote->GetElementByTagName("p") == NULL) {
		delete quote;
		return;
		}

	// yes there is, add it to the receptacle
	receptacle->AppendChild(quote);
}


bool MessageParser::IsEmailAddress(string_slice word)
{
	// if there's an '@' followed by a '.', we consider it an email address.
	const char* p = word.begin();
	const char* stopper = word.end();
	bool hadAt = false;
	for (; p < stopper; ++p) {
		char c = *p;
		if (c == '@') {
			if (hadAt)
				return false;
			else
				hadAt = true;
			}
		else if (c == '.') {
			if (hadAt)
				return true;
			}
		}
	return false;
}


