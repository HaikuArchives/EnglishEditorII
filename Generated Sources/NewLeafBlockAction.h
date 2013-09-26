/* NewLeafBlockAction.h */

#ifndef _H_NewLeafBlockAction_
#define _H_NewLeafBlockAction_

#include "Action.h"
#include "DOMString.h"

class Element;

class NewLeafBlockAction : public Action {
public:
	NewLeafBlockAction(Element* afterElementIn, DOMString tagName = DOMString());
	~NewLeafBlockAction();
	void	Do(DisplayDirector* director);
	void	Undo(DisplayDirector* director);

protected:
	Element*	afterElement;
	Element*	newElement;
};


#endif
