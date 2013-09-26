/* RestoreBlocksSelectionAction.h */

#ifndef _H_RestoreBlocksSelectionAction_
#define _H_RestoreBlocksSelectionAction_

#include "Action.h"

class Element;

class RestoreBlocksSelectionAction : public Action {
public:
	RestoreBlocksSelectionAction(Element* firstElementIn, Element* lastElementIn);
	void	Do(DisplayDirector* director);
	void	Undo(DisplayDirector* director);

protected:
	Element*	firstElement;
	Element*	lastElement;
};


#endif
