/* ChangeElementTagNameAction.h */

#ifndef _H_ChangeElementTagNameAction_
#define _H_ChangeElementTagNameAction_

#include "Action.h"
#include "String.h"

class Element;

class ChangeElementTagNameAction : public Action {
public:
	ChangeElementTagNameAction(Element* element, String newTagName);
	~ChangeElementTagNameAction();
	void	Do(DisplayDirector* director);
	void	Undo(DisplayDirector* director);

protected:
	Element*	oldElement;
	Element*	newElement;
};


#endif
