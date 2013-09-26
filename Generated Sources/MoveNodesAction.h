/* MoveNodesAction.h */

#ifndef _H_MoveNodesAction_
#define _H_MoveNodesAction_

#include "Action.h"

class Node;

/*
	You can move a single node to or from a NULL parent, but you won't
	be able to do this to multiple nodes until DocumentFragments are implemented.
*/

class MoveNodesAction : public Action {
public:
	MoveNodesAction(Node* firstNodeIn, Node* lastNodeIn,
                Node* destParentIn, Node* destNextNodeIn);
	void	Do(DisplayDirector* director);
	void	Undo(DisplayDirector* director);

protected:
	Node*	firstNode;
	Node*	lastNode;
	Node*	srcParent;
	Node*	srcNextNode;
	Node*	destParent;
	Node*	destNextNode;
};


#endif
