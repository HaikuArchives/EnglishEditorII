/* CopyNodesAction.cpp */

#include "CopyNodesAction.h"
#include "Node.h"

#include <vector>

class CopiedNodesList : public std::vector<Node*> {};


CopyNodesAction::CopyNodesAction(Node* firstNode, Node* lastNode, Node* destParentIn, Node* destBeforeNodeIn)
	: destParent(destParentIn), destBeforeNode(destBeforeNodeIn)
{
	copiedNodes = new CopiedNodesList();
	for (Node* srcNode = firstNode; ; srcNode = srcNode->NextSibling()) {
		copiedNodes->push_back(srcNode->CloneNode(true));
		if (srcNode == lastNode)
			break;
		}
}


CopyNodesAction::~CopyNodesAction()
{
	if (!done) {
		for (CopiedNodesList::iterator n = copiedNodes->begin(); n != copiedNodes->end(); ++n)
			delete *n;
		}
	delete copiedNodes;
}


void CopyNodesAction::Do(DisplayDirector* director)
{
	for (CopiedNodesList::iterator n = copiedNodes->begin(); n != copiedNodes->end(); ++n) {
		destParent->InsertBefore(*n, destBeforeNode);
		}

	Action::Do(director);
}


void CopyNodesAction::Undo(DisplayDirector* director)
{
	for (CopiedNodesList::iterator n = copiedNodes->begin(); n != copiedNodes->end(); ++n) {
		destParent->RemoveChild(*n);
		}

	Action::Undo(director);
}


Node* CopyNodesAction::FirstNode()
{
	return copiedNodes->front();
}


Node* CopyNodesAction::LastNode()
{
	return copiedNodes->back();
}


