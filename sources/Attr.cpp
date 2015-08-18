/* Attr.cpp */

#include "Attr.h"
#include "DOMException.h"



Attr::Attr(String nameIn)
	: name(nameIn)
{
}


Attr::Attr(const Attr& other, bool deep)
	: name(other.name)
{
	if (deep)
		CloneChildrenFrom(&other);
}


String Attr::NodeName()
{
	return name;
}


String Attr::NodeValue()
{
	return Value();
}


unsigned short Attr::NodeType()
{
	return ATTRIBUTE_NODE;
}


String Attr::Name()
{
	return name;
}


bool Attr::Specified()
{
	return true;
}


String Attr::Value()
{
	String value;
	for (Node* child = firstChild; child; child = child->NextSibling()) {
		if (child->NodeType() == ENTITY_REFERENCE_NODE) {
			for (Node* grandchild = child->FirstChild(); grandchild; grandchild = grandchild->NextSibling())
				value += grandchild->NodeValue();
			}
		else
			value += child->NodeValue();
		}
	return value;
}


Node* Attr::InsertBefore(Node* newChild, Node* refChild)
{
	unsigned short childType = newChild->NodeType();
	if (childType != TEXT_NODE && childType != ENTITY_REFERENCE_NODE)
		throw DOMException(HIERARCHY_REQUEST_ERR);

	return CompositeNode::InsertBefore(newChild, refChild);
}


Node* Attr::ReplaceChild(Node* newChild, Node* oldChild)
{
	unsigned short childType = newChild->NodeType();
	if (childType != TEXT_NODE && childType != ENTITY_REFERENCE_NODE)
		throw DOMException(HIERARCHY_REQUEST_ERR);

	return CompositeNode::ReplaceChild(newChild, oldChild);
}


Node* Attr::AppendChild(Node* newChild)
{
	unsigned short childType = newChild->NodeType();
	if (childType != TEXT_NODE && childType != ENTITY_REFERENCE_NODE)
		throw DOMException(HIERARCHY_REQUEST_ERR);

	return CompositeNode::AppendChild(newChild);
}


Node* Attr::CloneNode(bool deep)
{
	return new Attr(*this, deep);
}


