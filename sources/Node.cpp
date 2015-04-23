/* Node.cpp */

#include "Node.h"
#include "Document.h"
#include "EmptyNodeList.h"
#include "DOMChangeClient.h"
#include "DOMException.h"



Node::Node()
	: parent(NULL), prevSibling(NULL), nextSibling(NULL), changeClient(NULL)
{
}


Node::~Node()
{
	if (changeClient)
		changeClient->NodeDying();
}


DOMString Node::NodeValue()
{
	/* to be overridden */
	return DOMString();
}


NodeList* Node::ChildNodes()
{
	/* to be overridden */
	return new EmptyNodeList();
}


Node* Node::FirstChild()
{
	return NULL;
}


Node* Node::LastChild()
{
	return NULL;
}


NamedNodeMap* Node::Attributes()
{
	/* to be overridden */
	return NULL;
}


Document* Node::OwnerDocument()
{
	Node* node = parent;
	while (node->NodeType() != DOCUMENT_NODE) {
		node = node->ParentNode();
		if (node == NULL)
			return NULL;
		}
	return dynamic_cast<Document*>(node);
}


bool Node::HasChildNodes()
{
	/* to be overridden */
	return false;
}


void Node::AddChangeClient(DOMChangeClient* newChangeClient)
{
	/*** someday support multiple clients through DOMChangeSplitter ***/
	changeClient = newChangeClient;
}


void Node::RemoveChangeClient(DOMChangeClient* oldChangeClient)
{
	/*** someday support multiple clients through DOMChangeSplitter ***/
	if (changeClient == oldChangeClient)
		changeClient = NULL;
}


DOMChangeClient* Node::GetChangeClient()
{
	return changeClient;
}


void Node::SpliceBefore(Node* otherNode)
{
	nextSibling = otherNode;
	prevSibling = otherNode->prevSibling;
	if (prevSibling)
		prevSibling->nextSibling = this;
	otherNode->prevSibling = this;
}


void Node::SpliceAfter(Node* otherNode)
{
	prevSibling = otherNode;
	nextSibling = otherNode->nextSibling;
	if (nextSibling)
		nextSibling->prevSibling = this;
	otherNode->nextSibling = this;
}


void Node::HalfSpliceBefore(Node* otherNode)
{
	nextSibling = otherNode;
	if (otherNode)
		otherNode->prevSibling = this;
}


void Node::HalfSpliceAfter(Node* otherNode)
{
	prevSibling = otherNode;
	if (otherNode)
		otherNode->nextSibling = this;
}


void Node::Unsplice()
{
	if (prevSibling)
		prevSibling->nextSibling = nextSibling;
	if (nextSibling)
		nextSibling->prevSibling = prevSibling;
	prevSibling = nextSibling = NULL;
}


