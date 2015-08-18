/* MessageFileWriter.cpp */

#include "MessageFileWriter.h"
#include "TextFileWriter.h"
#include "Document.h"
#include "Element.h"
#include "Text.h"
#include "DOMUtils.h"



MessageFileWriter::MessageFileWriter(string_slice filePath, Document* documentIn)
	: document(documentIn)
{
	out = new TextFileWriter(filePath);
}


MessageFileWriter::~MessageFileWriter()
{
	delete out;
}


void MessageFileWriter::Write()
{
	Node* childNode;

	Element* docElement = document->DocumentElement();
	Element* head = docElement->GetElementByTagName("head");
	Element* body = docElement->GetElementByTagName("body");

	// write the header
	for (childNode = head->FirstChild(); childNode; childNode = childNode->NextSibling()) {
		Element* header = dynamic_cast<Element*>(childNode);
		if (header == NULL)
			continue;

		String tagName = header->TagName();
		String headerName;
		if (tagName == "from")
			headerName = "From: ";
		else if (tagName == "title")
			headerName = "Subject: ";
		else if (tagName == "date")
			headerName = "Date: ";
		else if (tagName == "newsgroups")
			headerName = "Newsgroups: ";
		else if (tagName == "to")
			headerName = "To: ";
		else if (tagName == "message-id")
			headerName = "Message-ID: ";
		else if (tagName == "references")
			headerName = "References: ";

		if (!headerName.empty()) {
			out->WriteRawText(headerName);
			out->WriteRawText(DOMUtils::NodeContents(header).trim());
			out->NewLine();
			}
		}

	// write the divider
	out->NewLine();

	// write the body
	bool started = false;
	for (childNode = body->FirstChild(); childNode; childNode = childNode->NextSibling()) {
		Element* element = dynamic_cast<Element*>(childNode);
		if (element == NULL)
			continue;
		String tagName = element->TagName();

		// put space after the last block
		if (started)
			out->NewLine();
		else
			started = true;

		// quotes are special
		if (tagName == "blockquote") {
			// first, write the attribution if there is one
			Element* attribution = element->GetElementByTagName("attribution");
			if (attribution) {
				WriteContents(attribution);
				out->NewLine();
				}
			// write the contents, quoted
			out->SetLinePrefix("> ");
			bool startedQuote = false;
			for (Node* quotedBlock = element->FirstChild(); quotedBlock; quotedBlock = quotedBlock->NextSibling()) {
				Element* quotedElement = dynamic_cast<Element*>(quotedBlock);
				if (quotedElement == NULL || quotedElement->TagName() == "attribution")
					continue;

				// add space if needed
				if (startedQuote)
					out->NewLine();
				else
					startedQuote = true;

				// write the element at a paragraph
				WriteContents(quotedElement);
				out->NewLine();
				}
			out->SetLinePrefix("");
			}

		// treat all others as paragraphs
		else {
			WriteContents(element);
			out->NewLine();
			}
		}
}


void MessageFileWriter::WriteContents(Node* node)
{
	for (Node* childNode = node->FirstChild(); childNode; childNode = childNode->NextSibling()) {
		switch (childNode->NodeType()) {
			case ELEMENT_NODE:
				{
				Element* element = dynamic_cast<Element*>(childNode);
				String tagName = element->TagName();
				bool italicize = (tagName == "i" || tagName == "cite");
				if (italicize)
					out->WriteText("_");
				WriteContents(element);
				if (italicize)
					out->WriteText("_");
				}
				break;

			case TEXT_NODE:
			case CDATA_SECTION_NODE:
				{
				CharacterData* textNode = dynamic_cast<CharacterData*>(childNode);
				out->WriteText(textNode->Data());
				}
				break;

			case ENTITY_REFERENCE_NODE:
				{
				if (childNode->HasChildNodes())
					WriteContents(childNode);
				else {
					out->WriteText("&");
					out->WriteText(childNode->NodeName());
					out->WriteText(";");
					}
				}
				break;
			}
		}
}


