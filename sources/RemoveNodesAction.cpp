/* RemoveNodesAction.cpp */

#include "RemoveNodesAction.h"
#include "Node.h"

#include <vector>

class RemovedNodesList : public std::vector<Node*> {};



RemoveNodesAction::RemoveNodesAction(Node* firstNode, Node* lastNode)
{
	nodes = new RemovedNodesList();
	for (Node* node = firstNode; node; node = node->NextSibling()) {
		nodes->push_back(node);
		if (node == lastNode)
			break;
		}

	parent = firstNode->ParentNode();
	beforeNode = lastNode->NextSibling();
}


RemoveNodesAction::~RemoveNodesAction()
{
	if (done) {
		for (RemovedNodesList::iterator n = nodes->begin(); n != nodes->end(); ++n)
			delete *n;
		}
	delete nodes;
}


void RemoveNodesAction::Do(DisplayDirector* director)
{
	// remove the nodes
	for (RemovedNodesList::iterator n = nodes->begin(); n != nodes->end(); ++n)
		parent->RemoveChild(*n);

	Action::Do(director);
}


void RemoveNodesAction::Undo(DisplayDirector* director)
{
	for (RemovedNodesList::iterator n = nodes->begin(); n != nodes->end(); ++n)
		parent->InsertBefore(*n, beforeNode);

	Action::Undo(director);
}


