/* MergeElementsAction.h */

#ifndef _H_MergeElementsAction_
#define _H_MergeElementsAction_

#include "Action.h"

class Node;

class MergeElementsAction : public Action {
public:
	MergeElementsAction(Node* mergedElement);
	~MergeElementsAction();
	bool	IsValid();
	void	Do(DisplayDirector* director);
	void	Undo(DisplayDirector* director);

protected:
	Node*	retainedElement;
	Node*	topMergedElement;
	Node*	bottomMergedElement;
	Node*	srcParent;
	Node*	srcNext;
	Node*	firstMergedNode;
};


#endif
