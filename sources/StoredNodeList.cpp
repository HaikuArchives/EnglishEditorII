/* StoredNodeList.cpp */

#include "StoredNodeList.h"
#include "Node.h"
#include "Element.h"
#include "DOMException.h"
#include <vector>

class NodesVector : public vector<Node*> {};



StoredNodeList::StoredNodeList(bool ownsNodesIn)
	: ownsNodes(ownsNodesIn)
{
	nodes = new NodesVector();
}


StoredNodeList::~StoredNodeList()
{
	if (ownsNodes) {
		for (NodesVector::iterator n = nodes->begin(); n != nodes->end(); n++)
			delete *n;
		}
	delete nodes;
}


Node* StoredNodeList::Item(unsigned long index)
{
	if (index >= nodes->size())
		return NULL;
	return (*nodes)[index];

	// we were using "at", but on the Intel side vector::at() is
	// somehow missing!!!
/***
	try {
		return nodes->at((size_type) index);
		}
	catch (out_of_range) {
		return NULL;
		}
***/
}


unsigned long StoredNodeList::Length()
{
	return nodes->size();
}


Node* StoredNodeList::InsertBefore(Node* newChild, Node* refChild)
{
	if (refChild == NULL)
		return AppendChild(newChild);

	typedef NodesVector::iterator NodeIter;
	for (NodeIter n = nodes->begin(); n != nodes->end(); n++) {
		if (*n == refChild) {
			nodes->insert(n, newChild);
			return newChild;
			}
		}

	// if we fell thru, refChild wasn't there
	throw DOMException(NOT_FOUND_ERR);
}


Node* StoredNodeList::ReplaceChild(Node* newChild, Node* oldChild)
{
	typedef NodesVector::iterator NodeIter;
	for (NodeIter n = nodes->begin(); n != nodes->end(); n++) {
		if (*n == oldChild) {
			*n = newChild;
			return oldChild;
			}
		}

	// if we fell thru, oldChild wasn't there
	throw DOMException(NOT_FOUND_ERR);
}


Node* StoredNodeList::RemoveChild(Node* oldChild)
{
	typedef NodesVector::iterator NodeIter;
	for (NodeIter n = nodes->begin(); n != nodes->end(); n++) {
		if (*n == oldChild) {
			nodes->erase(n);
			return oldChild;
			}
		}

	// if we fell thru, oldChild wasn't there
	throw DOMException(NOT_FOUND_ERR);
}


Node* StoredNodeList::AppendChild(Node* newChild)
{
	nodes->push_back(newChild);
	return newChild;
}


NodeList* StoredNodeList::GetElementsByTagName(DOMString name)
	/*** CALLER WILL OWN RETURNED LIST ***/
{
	StoredNodeList* elements = new StoredNodeList(false);	//*** should be a live list

	for (NodesVector::iterator n = nodes->begin(); n != nodes->end(); n++) {
		Element* element = dynamic_cast<Element*>(*n);
		if (element && element->TagName() == name)
			elements->AppendChild(element);
		}

	return elements;
}


Element* StoredNodeList::GetElementByTagName(DOMString name)
{
	for (NodesVector::iterator n = nodes->begin(); n != nodes->end(); n++) {
		Element* element = dynamic_cast<Element*>(*n);
		if (element && element->TagName() == name)
			return element;
		}

	return NULL;
}


NodeList* StoredNodeList::Clone()
{
	StoredNodeList* clone = new StoredNodeList();
	for (NodesVector::iterator n = nodes->begin(); n != nodes->end(); n++)
		clone->AppendChild((*n)->CloneNode(true));
	return clone;
}


