/* RestoreBetweenBlocksSelectionAction.h */

#ifndef _H_RestoreBetweenBlocksSelectionAction_
#define _H_RestoreBetweenBlocksSelectionAction_

#include "Action.h"

class Node;

class RestoreBetweenBlocksSelectionAction : public Action {
public:
	RestoreBetweenBlocksSelectionAction(Node* parentNodeIn, Node* beforeNodeIn);
	void	Do(DisplayDirector* director);
	void	Undo(DisplayDirector* director);

protected:
	Node*	parentNode;
	Node*	beforeNode;
};


#endif
