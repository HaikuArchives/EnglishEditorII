/* Element.cpp */

#include "Element.h"
#include "Attr.h"
#include "Text.h"
#include "StoredNamedNodeMap.h"
#include "Document.h"
#include "DOMChangeClient.h"



Element::Element(String nameIn)
	: name(nameIn)
{
	attributes = new StoredNamedNodeMap();
}


Element::Element(const Element& other, bool deep)
	: name(other.name)
{
	attributes = other.attributes->Clone();

	if (deep)
		CloneChildrenFrom(&other);
}


Element::~Element()
{
	delete attributes;
}


String Element::NodeName()
{
	return name;
}


unsigned short Element::NodeType()
{
	return ELEMENT_NODE;
}


NamedNodeMap* Element::Attributes()
{
	return attributes;
}


Node* Element::CloneNode(bool deep)
{
	return new Element(*this, deep);
}


String Element::TagName()
{
	return name;
}


String Element::GetAttribute(String name)
{
	Attr* attr = dynamic_cast<Attr*>(attributes->GetNamedItem(name));
	return (attr ? attr->Value() : String());
}


void Element::SetAttribute(String name, String value)
{
	Document* document = OwnerDocument();
	Attr* newAttr = document->CreateAttribute(name);
	newAttr->AppendChild(document->CreateTextNode(value));
	attributes->SetNamedItem(newAttr);
	if (changeClient)
		changeClient->AttributeSet(newAttr);
}


Attr* Element::GetAttributeNode(String name)
{
	return dynamic_cast<Attr*>(attributes->GetNamedItem(name));
}


Attr* Element::SetAttributeNode(Attr* newAttr)
{
	if (changeClient)
		changeClient->AttributeSet(newAttr);
	return dynamic_cast<Attr*>(attributes->SetNamedItem(newAttr));
}


Attr* Element::RemoveAttributeNode(Attr* oldAttr)
{
	return dynamic_cast<Attr*>(attributes->RemoveNamedItem(oldAttr->NodeName()));
}


