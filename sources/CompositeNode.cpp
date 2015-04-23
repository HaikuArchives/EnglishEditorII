/* CompositeNode.cpp */

#include "CompositeNode.h"
#include "Element.h"
#include "ChildrenNodeList.h"
#include "DocumentFragment.h"
#include "DOMChangeClient.h"
#include "DOMException.h"



CompositeNode::CompositeNode()
	: firstChild(NULL), lastChild(NULL)
{
}


CompositeNode::~CompositeNode()
{
	// delete all children
	Node* nextChild = firstChild;
	for (Node* child = nextChild; child; child = nextChild) {
		nextChild = child->NextSibling();
		delete child;
		}
}


NodeList* CompositeNode::ChildNodes()
{
	return new ChildrenNodeList(this);
}


Node* CompositeNode::FirstChild()
{
	return firstChild;
}


Node* CompositeNode::LastChild()
{
	return lastChild;
}


Node* CompositeNode::InsertBefore(Node* newChild, Node* refChild)
{
	// special-case appending
	if (refChild == NULL)
		return AppendChild(newChild);

	// set up
	if (refChild->ParentNode() != this)
		throw DOMException(NOT_FOUND_ERR);
	Node* firstNewChild = newChild;
	Node* lastNewChild = firstNewChild;
	// handle DocumentFragment specially--insert children rt the Node itself
	if (firstNewChild->NodeType() == DOCUMENT_FRAGMENT_NODE) {
		firstNewChild = newChild->FirstChild();
		lastNewChild = newChild->LastChild();
		}

	// move the nodes
	firstNewChild->HalfSpliceAfter(refChild->PreviousSibling());
	lastNewChild->HalfSpliceBefore(refChild);
	if (firstChild == refChild)
		firstChild = firstNewChild;
	SetParents(firstNewChild, lastNewChild);

	// notify change client
	if (changeClient)
		changeClient->ChildInsertedBefore(newChild, refChild);

	return newChild;
}


Node* CompositeNode::ReplaceChild(Node* newChild, Node* oldChild)
{
	if (oldChild->ParentNode() != this)
		throw DOMException(HIERARCHY_REQUEST_ERR);

	Node* firstNewChild = newChild;
	Node* lastNewChild = firstNewChild;	// until we determine otherwise
	if (firstNewChild->NodeType() == DOCUMENT_FRAGMENT_NODE) {
		firstNewChild = firstNewChild->FirstChild();
		lastNewChild = lastNewChild->LastChild();
		}

	if (oldChild == firstChild) {
		if (oldChild == lastChild) {
			// replacing the only child
			oldChild->Unsplice();
			firstChild = firstNewChild;
			lastChild = lastNewChild;
			}
		else {
			// replacing the first child
			Node* secondChild = oldChild->NextSibling();
			oldChild->Unsplice();
			lastNewChild->HalfSpliceBefore(secondChild);
			firstChild = firstNewChild;
			}
		}
	else {
		// replacing after the first child
		Node* prevChild = oldChild->PreviousSibling();
		Node* nextChild = oldChild->NextSibling();
		oldChild->Unsplice();
		firstNewChild->HalfSpliceAfter(prevChild);
		lastNewChild->HalfSpliceBefore(nextChild);
		if (oldChild == lastChild)
			lastChild = lastNewChild;
		}

	// set parents
	SetParents(firstNewChild, lastNewChild);
	oldChild->SetParent(NULL);

	// notify change client
	if (changeClient)
		changeClient->ChildReplaced(newChild, oldChild);

	return oldChild;
}


Node* CompositeNode::RemoveChild(Node* oldChild)
{
	if (oldChild == NULL || oldChild->ParentNode() != this)
		throw DOMException(HIERARCHY_REQUEST_ERR);

	if (firstChild == oldChild)
		firstChild = oldChild->NextSibling();
	if (lastChild == oldChild)
		lastChild = oldChild->PreviousSibling();
	oldChild->Unsplice();
	oldChild->SetParent(NULL);

	// notify change client
	if (changeClient)
		changeClient->ChildRemoved(oldChild);

	return oldChild;
}


Node* CompositeNode::AppendChild(Node* newChild)
{
	// set up
	Node* firstNewChild = newChild;
	Node* lastNewChild = newChild;
	// handle DocumentFragment specially, inserting its children instead
	if (firstNewChild->NodeType() == DOCUMENT_FRAGMENT_NODE) {
		firstNewChild = newChild->FirstChild();
		lastNewChild = newChild->LastChild();
		}

	// append the nodes
	firstNewChild->HalfSpliceAfter(lastChild);
	lastChild = lastNewChild;
	if (firstChild == NULL)
		firstChild = firstNewChild;
	SetParents(firstNewChild, lastNewChild);

	// notify change client
	if (changeClient)
		changeClient->ChildInsertedBefore(newChild, NULL);

	return newChild;
}


bool CompositeNode::HasChildNodes()
{
	return (firstChild != NULL);
}


NodeList* CompositeNode::GetElementsByTagName(DOMString name)
	/*** CALLER WILL OWN RETURNED LIST ***/
	/*** DOESN'T RETURN A "LIVE" LIST AS THE DOM SPEC REQUIRES ***/
{
	// leech off ChildrenNodeList
	ChildrenNodeList nodeList(this);
	return nodeList.GetElementsByTagName(name);
}


Element* CompositeNode::GetElementByTagName(DOMString name)
{
	// leech off ChildrenNodeList
	ChildrenNodeList nodeList(this);
	return nodeList.GetElementByTagName(name);
}


void CompositeNode::CloneChildrenFrom(const CompositeNode* other)
{
	for (Node* child = other->firstChild; child; child = child->NextSibling())
		AppendChild(child->CloneNode(true));
}


void CompositeNode::SetParents(Node* firstNewChild, Node* lastNewChild)
{
	for (Node* child = firstNewChild; child; child = child->NextSibling()) {
		child->SetParent(this);
		if (child == lastNewChild)
			break;
		}
}


