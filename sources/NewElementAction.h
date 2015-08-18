/* NewElementAction.h */

#ifndef _H_NewElementAction_
#define _H_NewElementAction_

#include "Action.h"
#include "String.h"

class Node;
class Element;

class NewElementAction : public Action {
public:
	NewElementAction(String tagName, Node* parentNodeIn, Node* beforeNodeIn);
	~NewElementAction();
	Element*	GetNewElement();
	void    	Do(DisplayDirector* director);
	void    	Undo(DisplayDirector* director);
	bool    	CanIncorporateNext(Action* nextActionIn);
	void    	IncorporateNext(Action* nextAction);

protected:
	Node*   	parentNode;
	Node*   	beforeNode;
	Element*	newElement;
};


#endif
