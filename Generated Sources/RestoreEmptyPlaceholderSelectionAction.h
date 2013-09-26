/* RestoreEmptyPlaceholderSelectionAction.h */

#ifndef _H_RestoreEmptyPlaceholderSelectionAction_
#define _H_RestoreEmptyPlaceholderSelectionAction_

#include "Action.h"

class Element;
class DisplayNode;
class PlaceholderTextDisplayNode;

class RestoreEmptyPlaceholderSelectionAction : public Action {
public:
	RestoreEmptyPlaceholderSelectionAction(Element* elementIn);
	void                       	Do(DisplayDirector* director);
	void                       	Undo(DisplayDirector* director);
	PlaceholderTextDisplayNode*	FindPlaceholderIn(DisplayNode* node);

protected:
	Element*	element;
};


#endif
