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


DOMString Document::NodeName()
{
	return "#document";
}


DOMString Document::NodeValue()
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


Element* Document::CreateElement(DOMString tagName)
{
	return new Element(tagName);
}


Text* Document::CreateTextNode(DOMString data)
{
	return new Text(data);
}


Comment* Document::CreateComment(DOMString data)
{
	return new Comment(data);
}


CDATASection* Document::CreateCDATASection(DOMString data)
{
	return new CDATASection(data);
}


ProcessingInstruction* Document::CreateProcessingInstruction(DOMString target, DOMString data)
{
	return new ProcessingInstruction(target, data);
}


Attr* Document::CreateAttribute(DOMString name)
{
	return new Attr(name);
}


EntityReference* Document::CreateEntityReference(DOMString name)
{
	return new EntityReference(name);
}


