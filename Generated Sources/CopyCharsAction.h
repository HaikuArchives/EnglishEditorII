/* CopyCharsAction.h */

#ifndef _H_CopyCharsAction_
#define _H_CopyCharsAction_

#include "Action.h"

class Text;

class CopyCharsAction : public Action {
public:
	CopyCharsAction(Text* srcTextIn, unsigned long srcOffsetIn, unsigned long lengthIn,
                Text* destTextIn, unsigned long destOffsetIn);
	void	Do(DisplayDirector* director);
	void	Undo(DisplayDirector* director);

protected:
	Text*        	srcText;
	unsigned long	srcOffset;
	unsigned long	length;
	Text*        	destText;
	unsigned long	destOffset;
};


#endif
