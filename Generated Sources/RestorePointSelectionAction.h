/* RestorePointSelectionAction.h */

#ifndef _H_RestorePointSelectionAction_
#define _H_RestorePointSelectionAction_

#include "Action.h"

class Text;

class RestorePointSelectionAction : public Action {
public:
	RestorePointSelectionAction(Text* textIn, unsigned long offsetIn);
	void	Do(DisplayDirector* director);
	void	Undo(DisplayDirector* director);

protected:
	Text*        	text;
	unsigned long	offset;
};


#endif
