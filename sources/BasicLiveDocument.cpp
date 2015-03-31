/* BasicLiveDocument.cpp */

#include "BasicLiveDocument.h"
#include "DOMChangeClient.h"


BasicLiveDocument::BasicLiveDocument(XMLNodeFactory* nodeFactoryIn)
	: BasicDocument(nodeFactoryIn)
{
}


BasicLiveDocument::BasicLiveDocument(const BasicLiveDocument& other, bool deep)
	: BasicDocument(other, deep)
{
}


Node* BasicLiveDocument::InsertBefore(Node* newChild, Node* refChild)
{
	Node* result = BasicDocument::InsertBefore(newChild, refChild);
	if (changeClient)
		changeClient->ChildInsertedBefore(newChild, refChild);
	return result;
}


Node* BasicLiveDocument::ReplaceChild(Node* newChild, Node* oldChild)
{
	Node* result = BasicDocument::ReplaceChild(newChild, oldChild);
	if (changeClient)
		changeClient->ChildReplaced(newChild, oldChild);
	return result;
}


Node* BasicLiveDocument::RemoveChild(Node* oldChild)
{
	Node* result = BasicDocument::RemoveChild(oldChild);
	if (changeClient)
		changeClient->ChildRemoved(oldChild);
	return result;
}


Node* BasicLiveDocument::AppendChild(Node* newChild)
{
	Node* result = BasicDocument::AppendChild(newChild);
	if (changeClient)
		changeClient->ChildInsertedBefore(newChild, NULL);
	return result;
}


Node* BasicLiveDocument::CloneNode(bool deep)
{
	return new BasicLiveDocument(*this, deep);
}


