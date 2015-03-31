/* RemoveNodesAction.h */

#ifndef _H_RemoveNodesAction_
#define _H_RemoveNodesAction_

#include "Action.h"

class Node;
class RemovedNodesList;

class RemoveNodesAction : public Action {
public:
	RemoveNodesAction(Node* firstNode, Node* lastNode);
	~RemoveNodesAction();
	void	Do(DisplayDirector* director);
	void	Undo(DisplayDirector* director);

protected:
	RemovedNodesList*	nodes;
	Node*            	parent;
	Node*            	beforeNode;
};


#endif
