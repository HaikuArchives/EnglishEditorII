/* BasicLiveAttr.cpp */

#include "BasicLiveAttr.h"
#include "DOMChangeClient.h"


BasicLiveAttr::BasicLiveAttr(DOMString nameIn)
	: BasicAttr(nameIn)
{
}


BasicLiveAttr::BasicLiveAttr(const BasicLiveAttr& other, bool deep)
	: BasicAttr(other, deep)
{
}


Node* BasicLiveAttr::InsertBefore(Node* newChild, Node* refChild)
{
	Node* result = BasicAttr::InsertBefore(newChild, refChild);
	if (changeClient)
		changeClient->ChildInsertedBefore(newChild, refChild);
	return result;
}


Node* BasicLiveAttr::ReplaceChild(Node* newChild, Node* oldChild)
{
	Node* result = BasicAttr::ReplaceChild(newChild, oldChild);
	if (changeClient)
		changeClient->ChildReplaced(newChild, oldChild);
	return result;
}


Node* BasicLiveAttr::RemoveChild(Node* oldChild)
{
	Node* result = BasicAttr::RemoveChild(oldChild);
	if (changeClient)
		changeClient->ChildRemoved(oldChild);
	return result;
}


Node* BasicLiveAttr::AppendChild(Node* newChild)
{
	Node* result = BasicAttr::AppendChild(newChild);
	if (changeClient)
		changeClient->ChildInsertedBefore(newChild, NULL);
	return result;
}


Node* BasicLiveAttr::CloneNode(bool deep)
{
	return new BasicLiveAttr(*this, deep);
}


