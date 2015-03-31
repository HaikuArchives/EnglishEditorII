/* MergeElementsAction.cpp */

#include "MergeElementsAction.h"
#include "Node.h"
#include "Text.h"
#include "DisplayDirector.h"



MergeElementsAction::MergeElementsAction(Node* mergedElement)
	: bottomMergedElement(mergedElement)
{
	// figure out how much deep the merge goes
	for (topMergedElement = bottomMergedElement; topMergedElement;
	     	topMergedElement = topMergedElement->ParentNode()) {
		retainedElement = topMergedElement->PreviousSibling();
		// skip ignorable whitespace, comments, etc.
		while (retainedElement && retainedElement->NodeType() != ELEMENT_NODE)
			retainedElement = retainedElement->PreviousSibling();
		if (retainedElement)
			break;
		}
	if (topMergedElement == NULL)
		return;

	// we want to put the text in the last paragraph of retainedElement
	Node* lastChild = retainedElement->LastChild();
	while (lastChild) {
		int nodeType = lastChild->NodeType();
		if (nodeType == TEXT_NODE) {
			Text* text = dynamic_cast<Text*>(lastChild);
			if (!text->IsIgnorableWhitespace())
				break;
			}
		if (nodeType == ELEMENT_NODE) {
			retainedElement = lastChild;
			lastChild = retainedElement->LastChild();
			}
		else {
			// it's a comment or something; check before it
			lastChild = lastChild->LastChild();
			}
		}

	// we need to know how to put topMergedElement back
	srcParent = topMergedElement->ParentNode();
	srcNext = topMergedElement->NextSibling();
}


MergeElementsAction::~MergeElementsAction()
{
	if (done)
		delete topMergedElement;
}


bool MergeElementsAction::IsValid()
{
	return (topMergedElement != NULL);
}


void MergeElementsAction::Do(DisplayDirector* director)
{
	if (topMergedElement == NULL)
		return;

	// move all the children of bottomMergedElement to retainedElement
	firstMergedNode = bottomMergedElement->FirstChild();
	for (Node* child = firstMergedNode; child;
	     	child = bottomMergedElement->FirstChild()) {
		retainedElement->AppendChild(bottomMergedElement->RemoveChild(child));
		}

	// remove topMergedElement
	srcParent->RemoveChild(topMergedElement);

	Action::Do(director);
}


void MergeElementsAction::Undo(DisplayDirector* director)
{
	if (topMergedElement == NULL)
		return;

	// move the merged nodes back to bottomMergedElement
	Node* nextChild;
	for (Node* child = firstMergedNode; child; child = nextChild) {
		nextChild = child->NextSibling();
		bottomMergedElement->AppendChild(retainedElement->RemoveChild(child));
		}

	// put topMergedElement back
	srcParent->InsertBefore(topMergedElement, srcNext);

	Action::Undo(director);
}


