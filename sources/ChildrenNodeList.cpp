/* ChildrenNodeList.cpp */

#include "ChildrenNodeList.h"
#include "Node.h"
#include "Element.h"
#include "StoredNodeList.h"
#include "DOMException.h"



ChildrenNodeList::ChildrenNodeList(Node* parentIn)
	: parent(parentIn)
{
}


Node* ChildrenNodeList::Item(unsigned long index)
{
	for (Node* child = parent->FirstChild(); child; child = child->NextSibling()) {
		if (index == 0)
			return child;
		--index;
		}
	return NULL;
}


unsigned long ChildrenNodeList::Length()
{
	unsigned long numChildren = 0;
	for (Node* child = parent->FirstChild(); child; child = child->NextSibling())
		++numChildren;
	return numChildren;
}


NodeList* ChildrenNodeList::GetElementsByTagName(String name)
{
	StoredNodeList* results = new StoredNodeList();
	for (Node* child = parent->FirstChild(); child; child = child->NextSibling()) {
		if (child->NodeType() == ELEMENT_NODE && child->NodeName() == name)
			results->AppendChild(child);
		}
	return results;
}


Element* ChildrenNodeList::GetElementByTagName(String name)
{
	for (Node* child = parent->FirstChild(); child; child = child->NextSibling()) {
		if (child->NodeType() == ELEMENT_NODE && child->NodeName() == name)
			return dynamic_cast<Element*>(child);
		}
	return NULL;
}


NodeList* ChildrenNodeList::Clone()
{
	throw DOMException(NOT_SUPPORTED_ERR);
}


