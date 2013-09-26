/* CopyNodesAction.h */

#ifndef _H_CopyNodesAction_
#define _H_CopyNodesAction_

#include "Action.h"

class Node;
class CopiedNodesList;

class CopyNodesAction : public Action {
public:
	CopyNodesAction(Node* firstNode, Node* lastNode,
                Node* destParentIn, Node* destBeforeNodeIn);
	~CopyNodesAction();
	void 	Do(DisplayDirector* director);
	void 	Undo(DisplayDirector* director);
	Node*	FirstNode();
	Node*	LastNode();

protected:
	CopiedNodesList*	copiedNodes;
	Node*           	destParent;
	Node*           	destBeforeNode;
};


#endif
