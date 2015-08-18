/* SplitElementAction.cpp */

#include "SplitElementAction.h"
#include "Element.h"
#include "BlockableDisplayNode.h"
#include "BetweenBlocksSelection.h"
#include "DisplayDirector.h"
#include "InternalException.h"

#include <vector>

class NewElementsStack : public std::vector<Node*> {};



SplitElementAction::SplitElementAction(Element* splittingElementIn, Node* afterNodeIn)
	: splittingElement(splittingElementIn), afterNode(afterNodeIn), doneOnce(false)
{
	newElements = new NewElementsStack();
}


SplitElementAction::~SplitElementAction()
{
	if (!done) {
		for (NewElementsStack::iterator e = newElements->begin(); e != newElements->end(); ++e)
			delete *e;
		}
	delete newElements;
}


void SplitElementAction::Do(DisplayDirector* director)
{
	Node* curElement = afterNode->ParentNode();
	if (curElement == NULL)
		throw InternalException("SplitElementAction::Do: Attempt to split non-element");
	Node* curAfterNode = afterNode;
	NewElementsStack::iterator e = newElements->begin();
	while (true) {
		// split the current element
		// create (or retrieve) and install the new element
		Node* newElement;
		if (!doneOnce) {
			newElement = curElement->CloneNode(false);
			newElements->push_back(newElement);
			}
		else
			newElement = *e;
		curElement->ParentNode()->InsertBefore(newElement, curElement->NextSibling());
		// move the children
		for (Node* child = curAfterNode->NextSibling(); child; ) {
			Node* nextChild = child->NextSibling();
			curElement->RemoveChild(child);
			newElement->AppendChild(child);
			child = nextChild;
			}

		// see if we're done
		if (curElement == splittingElement)
			break;

		// move up
		curAfterNode = curElement;
		curElement = curElement->ParentNode();
		++e;
		}

	// try to select between the (block) elements
	BlockableDisplayNode* beforeNode =
		dynamic_cast<BlockableDisplayNode*>(director->DisplayNodeFor(newElements->back()));
	if (beforeNode) {
		BlockableDisplayNode* parentNode =
			dynamic_cast<BlockableDisplayNode*>(beforeNode->Parent());
		if (parentNode)
			director->SetSelection(new BetweenBlocksSelection(parentNode, beforeNode));
		}

	doneOnce = true;
	Action::Do(director);
}


void SplitElementAction::Undo(DisplayDirector* director)
{
	director->SetSelection(NULL);

	Node* curElement = splittingElement;
	while (curElement != afterNode) {
		Node* splitChild = curElement->LastChild();

		// move the children back
		Node* splitElement = curElement->NextSibling();
		for (Node* child = splitElement->FirstChild(); child; ) {
			Node* nextChild = child->NextSibling();
			splitElement->RemoveChild(child);
			curElement->AppendChild(child);
			child = nextChild;
			}

		// remove the splitElement
		curElement->ParentNode()->RemoveChild(splitElement);

		// go 'round again
		curElement = splitChild;
		}

	Action::Undo(director);
}


