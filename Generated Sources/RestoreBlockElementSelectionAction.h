/* RestoreBlockElementSelectionAction.h */

#ifndef _H_RestoreBlockElementSelectionAction_
#define _H_RestoreBlockElementSelectionAction_

#include "Action.h"

class Element;

class RestoreBlockElementSelectionAction : public Action {
public:
	RestoreBlockElementSelectionAction(Element* elementIn);
	void	Do(DisplayDirector* director);
	void	Undo(DisplayDirector* director);

protected:
	Element*	element;
};


#endif
