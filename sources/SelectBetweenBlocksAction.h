/* SelectBetweenBlocksAction.h */

#ifndef _H_SelectBetweenBlocksAction_
#define _H_SelectBetweenBlocksAction_

#include "Action.h"

class Node;

class SelectBetweenBlocksAction : public Action {
public:
	SelectBetweenBlocksAction(Node* parentNodeIn, Node* beforeNodeIn);
	void	Do(DisplayDirector* director);
	void	Undo(DisplayDirector* director);

protected:
	Node*	parentNode;
	Node*	beforeNode;
};


#endif
