/* MessageFileSource.cpp */

#include "MessageFileSource.h"
#include "Document.h"
#include "Element.h"
#include "Text.h"
#include "Attr.h"
#include "CDATASection.h"
#include "EntityReference.h"
#include "NamedNodeMap.h"
#include "MessageParser.h"
#include "MessageFileWriter.h"
#include "XMLFileSource.h"
#include "XMLParser.h"
#include "DisplayDirector.h"
#include "DisplayNode.h"
#include "EnglishEditorWind.h"
#include "DOMUtils.h"
#include "System.h"
#include "Error.h"
#include "OSException.h"
#include "InternalException.h"
#include <memory>	// for auto_ptr
#include <stdio.h>
#include <errno.h>



MessageFileSource::MessageFileSource(const string_slice filePathIn, bool isNewMessageIn)
	: filePath(filePathIn), fileText(NULL), document(NULL), director(NULL),
	  isNewMessage(isNewMessageIn), isSent(false)
{
	// set up statics
	InitStatics();

	// read in the file
	// open it
	FILE* file = fopen(filePath.c_str(), "r");
	if (file == NULL)
		throw OSException("Couldn't open file.", errno);
	// find out how big it is
	fpos_t fileLen;
	if (fseek(file, 0, SEEK_END) != 0 || fgetpos(file, &fileLen) != 0) {
		fclose(file);
		throw OSException("Couldn't get size of file.", errno);
		}
	// read it
	rewind(file);
	fileText = new char[fileLen];
	fread(fileText, 1, fileLen, file);
	if (ferror(file)) {
		fclose(file);
		throw OSException("Couldn't read file.", errno);
		}
	fclose(file);

	// parse it
	document = new Document();
	if (!isNewMessage) {
		MessageParser parser(string_slice(fileText, fileText + fileLen), document);
		parser.Parse();
		}
	else {
		// new message--prototype file is XML
		XMLParser parser(string_slice(fileText, fileText + fileLen));
		document = parser.Parse();
		}

	if (isNewMessage)
		filePath = "";

	// mark it as read
	if (!isNewMessage)
		System::MarkMailMessageRead(string_slice(filePath));
}


MessageFileSource::MessageFileSource(Document* documentIn, bool isNewMessageIn)
	: fileText(NULL), document(documentIn), director(NULL),
	  isNewMessage(isNewMessageIn), isSent(false)
{
	InitStatics();
}


MessageFileSource::~MessageFileSource()
{
	delete document;
	delete fileText;
}


Document* MessageFileSource::GetDocument()
{
	return document;
}


string_slice MessageFileSource::GetWindowTitle()
{
	const char* fileName = strrchr(filePath.c_str(), '/');
	return (fileName ? fileName + 1 : "Reply");
}


DocumentSource* MessageFileSource::MakeMenuDocSource()
{
	// find out if it's mail or news, incoming or outgoing
	bool isNews = false;
	bool isIncoming = false;
	Element* head = document->DocumentElement()->GetElementByTagName("head");
	if (head) {
		if (head->GetElementByTagName("from"))
			isIncoming = true;
		if (head->GetElementByTagName("newsgroups"))
			isNews = true;
		}

	String filePath = System::AppDirectory();
	filePath += "Aux-Docs/";
	filePath += (isIncoming ? "incoming-" : "outgoing-");
	filePath += (isNews ? "news-" : "mail-");
	filePath += "menu";
	return new XMLFileSource(filePath);
}


void MessageFileSource::AttachedToDirector(DisplayDirector* directorIn)
{
	director = directorIn;

	// if there's no file, its a new message, and we'll want to select something
	if (filePath.empty()) {
		bool haveSelected = false;

		// if the title is empty, select that
		Element* docElement = document->DocumentElement();
		if (docElement) {
			Element* head = docElement->GetElementByTagName("head");
			if (head) {
				Element* title = head->GetElementByTagName("title");
				if (title && DOMUtils::NodeContents(title).trim().empty()) {
					DisplayNode* titleDisplayNode = director->DisplayNodeFor(title);
					if (titleDisplayNode) {
						director->SetSelection(titleDisplayNode->GetSelection());
						haveSelected = true;
						}
					}
				}
			}

		// select the blank paragraph at the end
		if (!haveSelected) {
			DisplayNode* docDisplayNode = director->DisplayNodeFor(document);
			if (docDisplayNode)
				director->SetSelection(docDisplayNode->EndSelection());
			}
		}
}


void MessageFileSource::Save()
{
	char dateStr[128];

	Element* head = document->DocumentElement()->GetElementByTagName("head");
	time_t curTime = time(NULL);

	// if it hasn't been saved yet, create the file
	bool createdFile = false;
	bool isNews = false;
	if (filePath.empty()) {
		// find out if it's mail or news
		if (head && head->GetElementByTagName("newsgroups"))
			isNews = true;

		// get the title
		DOMString title;
		if (head) {
			Element* subjectElement = head->GetElementByTagName("title");
			if (subjectElement) {
				title = DOMUtils::NodeContents(subjectElement);
				}
			}
		if (title.empty()) {
			Error::ShowError("Your message needs a subject before you can save it.");
			return;
			}

		// adjust the title so it's appropriate as a filename
		title = System::AdjustFileName(title.trim());

		// add a timestamp to the title
		strftime(dateStr, 128, " %Y.%m.%d.%H.%M.%S", localtime(&curTime));
		title += dateStr;

		// set up the file path
		filePath = (isNews ? System::OutgoingNewsDirectory() : System::OutgoingMailDirectory());
		filePath += title;

		// see if it already exists
		struct stat statBuf;
		if (stat(filePath.c_str(), &statBuf) == 0) {
			Error::ShowError("Your message has the same subject as one you already created, which is currently not supported.  As a workaround, rename the existing file and save this one again.");
			return;
			}

		createdFile = true;
		}

	// update the date
	strftime(dateStr, 128, "%a, %d %b %Y %X %Z", localtime(&curTime));
	DOMString date(dateStr);
	date.detach();
	Element* dateElement = head->GetElementByTagName("date");
	if (dateElement == NULL) {
		dateElement = document->CreateElement("date");
		dateElement->AppendChild(document->CreateTextNode(date));
		head->AppendChild(dateElement);
		}
	else {
		Text* dateText = dynamic_cast<Text*>(dateElement->FirstChild());
		if (dateText == NULL) {
			dateText = document->CreateTextNode(date);
			dateElement->AppendChild(dateText);
			}
		else
			dateText->SetData(date);
		}

	// write the file
	MessageFileWriter writer(filePath, document);
	writer.Write();

	// if we created the file, set its type
	if (createdFile)
		System::SetFileMimeType(filePath, (isNews ? "text/x-news" : "text/x-email"));
}


bool MessageFileSource::CanCloseOnSpacebar()
{
	return true;
}


bool MessageFileSource::IsSent()
{
	return isSent;
}


bool MessageFileSource::IsNewMessage()
{
	return isNewMessage;
}


void MessageFileSource::Reply(bool toNews)
{
	// start creating the reply
	Document* replyDoc = dynamic_cast<Document*>(document->CloneNode(false));
	Element* docElement = replyDoc->CreateElement("message");
	replyDoc->AppendChild(docElement);

	// make the header
	Element* head = replyDoc->CreateElement("head");
	docElement->AppendChild(head);
	Element* origHead = document->DocumentElement()->GetElementByTagName("head");
	// subject
	Node* subject = head->AppendChild(replyDoc->CreateElement("title"));
	Element* origSubjectNode = (origHead ? origHead->GetElementByTagName("title") : NULL);
	if (origSubjectNode) {
		DOMString origSubject = origSubjectNode->FirstChild()->NodeValue().trim();
		if (!origSubject.empty()) {
			DOMString newSubject;
			if (!origSubject.startsWith("Re:") && !origSubject.startsWith("re:"))
				newSubject = "Re: ";
			newSubject += origSubject;
			newSubject.detach();
			subject->AppendChild(replyDoc->CreateTextNode(newSubject));
			}
		}
	// newsgroups
	if (toNews && origHead) {
		Node* newsgroups = head->AppendChild(replyDoc->CreateElement("newsgroups"));
		Element* origGroupsNode = origHead->GetElementByTagName("newsgroups");
		if (origGroupsNode) {
			DOMString groups = origGroupsNode->FirstChild()->NodeValue().detach();
			newsgroups->AppendChild(replyDoc->CreateTextNode(groups));
			}
		}
	// to
	if (!toNews && origHead) {
		DOMString replyTo;
		Element* replyToNode = origHead->GetElementByTagName("reply-to");
		if (replyToNode)
			replyTo = DOMUtils::NodeContents(replyToNode).trim();
		if (replyTo.empty()) {
			Element* fromNode = origHead->GetElementByTagName("from");
			if (fromNode)
				replyTo = DOMUtils::NodeContents(fromNode).trim();
			}
		if (!replyTo.empty()) {
			Node* toNode = head->AppendChild(replyDoc->CreateElement("to"));
			toNode->AppendChild(replyDoc->CreateTextNode(replyTo));
			}
		}
	// references
	if (toNews && origHead) {
		// build the references
		DOMString refsList;
		Element* origRefs = origHead->GetElementByTagName("references");
		if (origRefs) {
			refsList = DOMUtils::NodeContents(origRefs).trim().detach();
			refsList += " ";
			}
		Element* origID = origHead->GetElementByTagName("message-id");
		if (origID)
			refsList += DOMUtils::NodeContents(origID).trim();
		// install the "References" header
		if (!refsList.empty()) {
			Node* newRefs = head->AppendChild(replyDoc->CreateElement("references"));
			newRefs->AppendChild(replyDoc->CreateTextNode(refsList));
			}
		}

	// prepare to attribute
	Element* attributionTemplate = replyDoc->CreateElement("blockquote");
	if (origHead) {
		Element* origFromNode = origHead->GetElementByTagName("from");
		if (origFromNode) {
			DOMString from = origFromNode->FirstChild()->NodeValue().detach();
			from += " wrote:";
			Element* attributionNode = replyDoc->CreateElement("attribution");
			attributionNode->AppendChild(replyDoc->CreateTextNode(from));
			attributionTemplate->AppendChild(attributionNode);
			}
		}

	// copy the body
	Element* body = replyDoc->CreateElement("body");
	docElement->AppendChild(body);
	Element* origBody = document->DocumentElement()->GetElementByTagName("body");
	Element* directQuote = NULL;
	for (Node* origChild = origBody->FirstChild(); origChild; origChild = origChild->NextSibling()) {
		if (origChild->NodeType() != ELEMENT_NODE)
			continue;
		if (origChild->NodeName() == "blockquote") {
			// we've finished a directQuote (if there was one)
			directQuote = NULL;
			// just copy the quote over
			body->AppendChild(CloneNode(origChild, replyDoc));
			}
		else if (origChild->NodeName() == "signature") {
			// don't copy signatures
			}
		else {
			// any other kind of element needs to get quoted
			if (directQuote == NULL) {
				directQuote = dynamic_cast<Element*>(attributionTemplate->CloneNode(true));
				body->AppendChild(directQuote);
				}
			// copy it into the current quote
			directQuote->AppendChild(CloneNode(origChild, replyDoc));
			}
		}

	// finally, add a new blank paragraph at the end
	Element* blankPara = replyDoc->CreateElement("p");
	blankPara->AppendChild(replyDoc->CreateTextNode(""));
	body->AppendChild(blankPara);

	// open the new window
	new EnglishEditorWind(new MessageFileSource(replyDoc, true));
}


Node* MessageFileSource::CloneNode(Node* origNode, Document* replyDoc)
{
	int i;

	// create the node
	DOMString nodeName = origNode->NodeName();
	nodeName.detach();
	Node* newNode = NULL;
	bool copyChildren = true;
	switch (origNode->NodeType()) {
		case ELEMENT_NODE:
			{
			Element* newElement = replyDoc->CreateElement(nodeName);
			NamedNodeMap* attributes = origNode->Attributes();
			if (attributes) {
				int numAttributes = attributes->Length();
				for (i=0; i<numAttributes; ++i) {
					Node* newAttrNode = CloneNode(attributes->Item(i), replyDoc);
					if (newAttrNode == NULL)
						throw InternalException("MessageFileSource::CloneNode: Failed to clone attribute.");
					if (newAttrNode->NodeType() != ATTRIBUTE_NODE)
						throw InternalException("MessageFileSource::CloneNode: Clone of attribute is not an attribute!");
					Attr* newAttr = dynamic_cast<Attr*>(newAttrNode);
					newElement->SetAttributeNode(newAttr);
					}
				}
			newNode = newElement;
			}
			break;

		case ATTRIBUTE_NODE:
			newNode = replyDoc->CreateAttribute(nodeName);
			break;

		case TEXT_NODE:
			newNode = replyDoc->CreateTextNode(origNode->NodeValue().detach());
			break;

		case CDATA_SECTION_NODE:
			newNode = replyDoc->CreateCDATASection(origNode->NodeValue().detach());
			break;

		case ENTITY_REFERENCE_NODE:
			newNode = replyDoc->CreateEntityReference(nodeName);
			copyChildren = false;
			break;
		}

	// copy the children
	if (copyChildren) {
		for (Node* child = origNode->FirstChild(); child; child = child->NextSibling()) {
			// ignore "signature" elements
			if (child->NodeType() == ELEMENT_NODE && child->NodeName() == "signature")
				continue;
			// copy
			newNode->AppendChild(CloneNode(child, replyDoc));
			}
		}

	return newNode;
}


void MessageFileSource::Post()
{
	// make sure it's saved first
	if (director->IsDirty())
		director->Save();
	if (filePath.empty())
		return;

	// post it
	System::PostNewsMessage(filePath);

	// mark as sent (& display it)
	isSent = true;
	director->RefreshAll();
}


void MessageFileSource::Send()
{
	// make sure it's saved first
	if (director->IsDirty())
		director->Save();
	if (filePath.empty())
		return;

	// post it
	Element* header = document->DocumentElement()->GetElementByTagName("head");
	System::SendMailMessage(filePath, header);

	// mark as sent (& display it)
	isSent = true;
	director->RefreshAll();
}


void MessageFileSource::Trash()
{
	String filePath = this->filePath;	// in case closing the window makes "this" go away
	director->RequestClose();
	System::TrashFile(filePath);
}


DOMString MessageFileSource::FunctionCall(DOMString function, DOMString arg, StyleScriptable* target)
{
	if (function == "actionAllowed") {
		bool result = false;
		arg = target->Eval(arg).trim();
		if (arg == "EmailReply()")
			result = true;	/***/
		else if (arg == "PostReply()")
			result = true;	/***/
		else if (arg == "Send()" || arg == "Post()" || arg == "Trash()")
			result = true;
		return (result ? "true" : "");
		}
	else if (function == "Send")
		Send();
	else if (function == "Post")
		Post();
	else if (function == "EmailReply")
		Reply(false);
	else if (function == "PostReply")
		Reply(true);
	else if (function == "Trash")
		Trash();
	else
		return StyleScriptable::FunctionCall(function, arg, target);

	return DOMString();
}


void MessageFileSource::InitStatics()
{
}


