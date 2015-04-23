/* EmptyNodeList.cpp */

#include "EmptyNodeList.h"
#include "DOMException.h"


EmptyNodeList::EmptyNodeList()
{
}


Node* EmptyNodeList::Item(unsigned long index)
{
	return NULL;
}


unsigned long EmptyNodeList::Length()
{
	return 0;
}


Node* EmptyNodeList::AppendChild(Node* newChild)
{
	throw DOMException(NO_MODIFICATION_ALLOWED_ERR);
}


NodeList* EmptyNodeList::GetElementsByTagName(DOMString name)
{
	return Clone();		// not "this", since caller will own the NodeList
}


Element* EmptyNodeList::GetElementByTagName(DOMString name)
{
	return NULL;
}


NodeList* EmptyNodeList::Clone()
{
	return new EmptyNodeList();
}


