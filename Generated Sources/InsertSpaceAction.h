/* InsertSpaceAction.h */

#ifndef _H_InsertSpaceAction_
#define _H_InsertSpaceAction_

#include "Action.h"

class Text;

class InsertSpaceAction : public Action {
public:
	InsertSpaceAction(Text* textIn, unsigned long offsetIn);
	void         	Do(DisplayDirector* director);
	void         	Undo(DisplayDirector* director);
	unsigned long	SpaceLength();

protected:
	Text*        	text;
	unsigned long	offset;
};


#endif
