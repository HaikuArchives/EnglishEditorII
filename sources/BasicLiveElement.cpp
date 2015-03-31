/* BasicLiveElement.cpp */

#include "BasicLiveElement.h"
#include "DOMChangeClient.h"


BasicLiveElement::BasicLiveElement(DOMString name)
	: BasicElement(name)
{
}


BasicLiveElement::BasicLiveElement(const BasicLiveElement& other, bool deep)
	: BasicElement(other, deep)
{
}


Node* BasicLiveElement::InsertBefore(Node* newChild, Node* refChild)
{
	Node* result = BasicElement::InsertBefore(newChild, refChild);
	if (changeClient)
		changeClient->ChildInsertedBefore(newChild, refChild);
	return result;
}


Node* BasicLiveElement::ReplaceChild(Node* newChild, Node* oldChild)
{
	Node* result = BasicElement::ReplaceChild(newChild, oldChild);
	if (changeClient)
		changeClient->ChildReplaced(newChild, oldChild);
	return result;
}


Node* BasicLiveElement::RemoveChild(Node* oldChild)
{
	Node* result = BasicElement::RemoveChild(oldChild);
	if (changeClient)
		changeClient->ChildRemoved(oldChild);
	return result;
}


Node* BasicLiveElement::AppendChild(Node* newChild)
{
	Node* result = BasicElement::AppendChild(newChild);
	if (changeClient)
		changeClient->ChildInsertedBefore(newChild, NULL);
	return result;
}


Attr* BasicLiveElement::SetAttributeNode(Attr* newAttr)
{
	Attr* result = BasicElement::SetAttributeNode(newAttr);
	if (changeClient)
		changeClient->AttributeSet(newAttr);
	return result;
}


Attr* BasicLiveElement::RemoveAttributeNode(Attr* oldAttr)
{
	Attr* result = BasicElement::RemoveAttributeNode(oldAttr);
	if (changeClient)
		changeClient->AttributeRemoved(oldAttr);
	return result;
}


Node* BasicLiveElement::CloneNode(bool deep)
{
	return new BasicLiveElement(*this, deep);
}


