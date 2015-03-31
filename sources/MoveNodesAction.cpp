/* MoveNodesAction.cpp */

#include "MoveNodesAction.h"
#include "Node.h"
#include "DisplayDirector.h"



MoveNodesAction::MoveNodesAction(Node* firstNodeIn, Node* lastNodeIn, Node* destParentIn, Node* destNextNodeIn)
	: firstNode(firstNodeIn), lastNode(lastNodeIn),
	  destParent(destParentIn), destNextNode(destNextNodeIn)
{
	srcParent = firstNode->ParentNode();
	srcNextNode = lastNode->NextSibling();
}


void MoveNodesAction::Do(DisplayDirector* director)
{
	director->SetSelection(NULL);

	Node* nextNode = NULL;
	for (Node* node = firstNode; ; node = nextNode) {
		nextNode = node->NextSibling();

		// move the node
		if (srcParent)
			srcParent->RemoveChild(node);
		if (destParent)
			destParent->InsertBefore(node, destNextNode);

		if (node == lastNode)
			break;
		}

	Action::Do(director);
}


void MoveNodesAction::Undo(DisplayDirector* director)
{
	director->SetSelection(NULL);

	Node* nextNode = NULL;
	for (Node* node = firstNode; ; node = nextNode) {
		nextNode = node->NextSibling();

		// move the node
		if (destParent)
			destParent->RemoveChild(node);
		if (srcParent)
			srcParent->InsertBefore(node, srcNextNode);

		if (node == lastNode)
			break;
		}

	Action::Undo(director);
}


