/* SelectBlockElementsAction.h */

#ifndef _H_SelectBlockElementsAction_
#define _H_SelectBlockElementsAction_

#include "Action.h"

class Element;

class SelectBlockElementsAction : public Action {
public:
	SelectBlockElementsAction(Element* firstElementIn, Element* lastElementIn);
	void	Do(DisplayDirector* director);
	void	Undo(DisplayDirector* director);

protected:
	Element*	firstElement;
	Element*	lastElement;
};


#endif
