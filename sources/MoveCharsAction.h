/* MoveCharsAction.h */

#ifndef _H_MoveCharsAction_
#define _H_MoveCharsAction_

#include "Action.h"
#include "String.h"

class Text;

class MoveCharsAction : public Action {
public:
	MoveCharsAction(Text* srcTextIn, unsigned long srcOffsetIn, unsigned long srcLenIn,
                Text* destTextIn, unsigned long destOffsetIn, unsigned long destLen = 0,
                bool addPreSpaceIn = false, bool addPostSpaceIn = false,
                bool selectIn = false);
	void	Do(DisplayDirector* director);
	void	Undo(DisplayDirector* director);

protected:
	Text*        	srcText;
	unsigned long	srcOffset;
	unsigned long	srcLen;
	Text*        	destText;
	unsigned long	destOffset;
	String    	destDeleted;
	bool         	addPreSpace;
	bool         	addPostSpace;
	bool         	select;
};


#endif
