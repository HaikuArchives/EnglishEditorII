/* BasicLiveEntityReference.cpp */

#include "BasicLiveEntityReference.h"
#include "DOMChangeClient.h"


BasicLiveEntityReference::BasicLiveEntityReference(DOMString nameIn)
	: BasicEntityReference(nameIn)
{
}


BasicLiveEntityReference::BasicLiveEntityReference(const BasicLiveEntityReference& other)
	: BasicEntityReference(other)
{
}


Node* BasicLiveEntityReference::InsertBefore(Node* newChild, Node* refChild)
{
	Node* result = BasicEntityReference::InsertBefore(newChild, refChild);
	if (changeClient)
		changeClient->ChildInsertedBefore(newChild, refChild);
	return result;
}


Node* BasicLiveEntityReference::ReplaceChild(Node* newChild, Node* oldChild)
{
	Node* result = BasicEntityReference::ReplaceChild(newChild, oldChild);
	if (changeClient)
		changeClient->ChildReplaced(newChild, oldChild);
	return result;
}


Node* BasicLiveEntityReference::RemoveChild(Node* oldChild)
{
	Node* result = BasicEntityReference::RemoveChild(oldChild);
	if (changeClient)
		changeClient->ChildRemoved(oldChild);
	return result;
}


Node* BasicLiveEntityReference::AppendChild(Node* newChild)
{
	Node* result = BasicEntityReference::AppendChild(newChild);
	if (changeClient)
		changeClient->ChildInsertedBefore(newChild, NULL);
	return result;
}


Node* BasicLiveEntityReference::CloneNode(bool deep)
{
	return new BasicLiveEntityReference(*this);
}


