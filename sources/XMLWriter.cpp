/* XMLWriter.cpp */

#include "XMLWriter.h"
#include "Document.h"
#include "Element.h"
#include "Attr.h"
#include "Text.h"
#include "CDATASection.h"
#include "EntityReference.h"
#include "ProcessingInstruction.h"
#include "Comment.h"
#include "NamedNodeMap.h"
#include "DOMStringIter.h"
#include "InternalException.h"



XMLWriter::XMLWriter(Node* rootNodeIn)
	: rootNode(rootNodeIn)
{
}


XMLWriter::~XMLWriter()
{
}


void XMLWriter::Write()
{
	WriteNode(rootNode);
}


void XMLWriter::WriteNode(Node* node)
{
	switch (node->NodeType()) {
		case DOCUMENT_NODE:
			WriteChildren(node);
			break;

		case ELEMENT_NODE:
			WriteElement(dynamic_cast<Element*>(node));
			break;

		case TEXT_NODE:
			WriteText(dynamic_cast<Text*>(node));
			break;

		case CDATA_SECTION_NODE:
			WriteText(dynamic_cast<CDATASection*>(node));
			break;

		case ENTITY_REFERENCE_NODE:
			WriteEntityReference(dynamic_cast<EntityReference*>(node));
			break;

		case PROCESSING_INSTRUCTION_NODE:
			WriteProcessingInstruction(dynamic_cast<ProcessingInstruction*>(node));
			break;

		case COMMENT_NODE:
			WriteComment(dynamic_cast<Comment*>(node));
			break;
		}
}


void XMLWriter::WriteChildren(Node* node)
{
	for (Node* child = node->FirstChild(); child; child = child->NextSibling())
		WriteNode(child);
}


void XMLWriter::WriteElement(Element* element)
{
	if (element == NULL)
		throw InternalException("DOMWriter: NodeType doesn't match node type.");

	// begin the start tag
	Write("<");
	Write(element->TagName());

	// attributes
	NamedNodeMap* attributes = element->Attributes();
	int numAttrs = attributes->Length();
	for (int i = 0; i < numAttrs; ++i) {
		Attr* attr = dynamic_cast<Attr*>(attributes->Item(i));
		if (attr == NULL) {
			qstring msg = "XMLWriter::WriteElement: Element's attributes contains non-attribute (node-type=";
			Node* attrNode = attributes->Item(i);
			if (attrNode)
				msg += qstring(attrNode->NodeType());
			else
				msg += "NULL";
			msg += ").";
			qstring* leaky = new qstring(msg);
			throw InternalException(*leaky);
			}
		if (!attr->Specified())
			continue;
		Write(" ");
		Write(attr->Name());
		Write("=\"");
		WriteChildren(attr);
		Write("\"");
		}

	// end of start tag
	bool hasChildren = element->HasChildNodes();
	if (hasChildren)
		Write(">");
	else
		Write("/>");

	// children
	if (hasChildren)
		WriteChildren(element);

	// end tag
	if (hasChildren) {
		Write("</");
		Write(element->TagName());
		Write(">");
		}
}


void XMLWriter::WriteText(Text* text)
{
	String data = text->Data();
	DOMStringIter p = data.begin();
	DOMStringIter stopper = data.end();
	int startPos = 0;
	int curPos = 0;
	const char* entity;
	for (; p != stopper; ++p) {
		char c = *p;
		switch (c) {
			case '&':
				entity = "&amp;";
				goto useEntity;
			case '<':
				entity = "&lt;";
				goto useEntity;
			case '>':
				entity = "&gt;";
				goto useEntity;
			case '\'':
				entity = "&apos;";
				goto useEntity;
			case '\"':
				entity = "&quot;";
				goto useEntity;
			useEntity:
				if (curPos > startPos)
					Write(data.substr(startPos, curPos - startPos));
				Write(entity);
				curPos += 1;
				startPos = curPos;
				break;

			default:
				curPos += 1;
				break;
			}
		}

	if (startPos == 0)
		Write(data);	// slight optimization
	else if (curPos > startPos)
		Write(data.substr(startPos, curPos - startPos));
}


void XMLWriter::WriteCDATASection(CDATASection* cdata)
{
	static const char cdataEnd[] = { ']', ']', '>', 0 };	// don't put these together so Castle Yankee doesn't get confused

	Write("<![CDATA[");
	Write(cdata->Data());
	Write(cdataEnd);
}


void XMLWriter::WriteEntityReference(EntityReference* entityRef)
{
	Write("&");
	Write(entityRef->NodeName());
	Write(";");
}


void XMLWriter::WriteProcessingInstruction(ProcessingInstruction* pi)
{
	Write("<?");
	Write(pi->Target());
	Write(" ");
	Write(pi->Data());
	Write("?>");
}


void XMLWriter::WriteComment(Comment* comment)
{
	Write("<!--");
	Write(comment->Data());
	Write("-->");
}


