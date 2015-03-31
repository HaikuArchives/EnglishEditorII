/* ChangeElementTagNameAction.h */

#ifndef _H_ChangeElementTagNameAction_
#define _H_ChangeElementTagNameAction_

#include "Action.h"
#include "DOMString.h"

class Element;

class ChangeElementTagNameAction : public Action {
public:
	ChangeElementTagNameAction(Element* element, DOMString newTagName);
	~ChangeElementTagNameAction();
	void	Do(DisplayDirector* director);
	void	Undo(DisplayDirector* director);

protected:
	Element*	oldElement;
	Element*	newElement;
};


#endif
