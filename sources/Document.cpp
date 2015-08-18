/* Document.cpp */

#include "Document.h"
#include "Element.h"
#include "Text.h"
#include "Comment.h"
#include "CDATASection.h"
#include "ProcessingInstruction.h"
#include "Attr.h"
#include "EntityReference.h"


Document::Document()
{
}


Document::Document(const Document& other, bool deep)
{
	if (deep)
		CloneChildrenFrom(&other);
}


String Document::NodeName()
{
	return "#document";
}


String Document::NodeValue()
{
	return "";
}


unsigned short Document::NodeType()
{
	return DOCUMENT_NODE;
}


Node* Document::CloneNode(bool deep)
{
	return new Document(*this, deep);
}


Element* Document::DocumentElement()
{
	for (Node* child = firstChild; child; child = child->NextSibling()) {
		if (child == NULL)
			break;
		if (child->NodeType() == ELEMENT_NODE)
			return dynamic_cast<Element*>(child);
		}
	return NULL;
}


Element* Document::CreateElement(String tagName)
{
	return new Element(tagName);
}


Text* Document::CreateTextNode(String data)
{
	return new Text(data);
}


Comment* Document::CreateComment(String data)
{
	return new Comment(data);
}


CDATASection* Document::CreateCDATASection(String data)
{
	return new CDATASection(data);
}


ProcessingInstruction* Document::CreateProcessingInstruction(String target, String data)
{
	return new ProcessingInstruction(target, data);
}


Attr* Document::CreateAttribute(String name)
{
	return new Attr(name);
}


EntityReference* Document::CreateEntityReference(String name)
{
	return new EntityReference(name);
}


