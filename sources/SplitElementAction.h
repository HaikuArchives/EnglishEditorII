/* SplitElementAction.h */

#ifndef _H_SplitElementAction_
#define _H_SplitElementAction_

#include "Action.h"

class Element;
class Node;
class NewElementsStack;

class SplitElementAction : public Action {
public:
	SplitElementAction(Element* splittingElementIn, Node* afterNodeIn);
	~SplitElementAction();
	void	Do(DisplayDirector* director);
	void	Undo(DisplayDirector* director);

protected:
	Element*         	splittingElement;
	Node*            	afterNode;
	NewElementsStack*	newElements;
	bool             	doneOnce;
};


#endif
