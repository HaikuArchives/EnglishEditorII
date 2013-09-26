/* RestoreInlineElementSelectionAction.h */

#ifndef _H_RestoreInlineElementSelectionAction_
#define _H_RestoreInlineElementSelectionAction_

#include "Action.h"

class Element;

class RestoreInlineElementSelectionAction : public Action {
public:
	RestoreInlineElementSelectionAction();
	void	Do(DisplayDirector* director);
	void	Undo(DisplayDirector* director);

protected:
	Element*	element;
};


#endif
