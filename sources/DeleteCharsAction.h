/* DeleteCharsAction.h */

#ifndef _H_DeleteCharsAction_
#define _H_DeleteCharsAction_

#include "Action.h"
#include "String.h"

class Text;

class DeleteCharsAction : public Action {
public:
	DeleteCharsAction(Text* textIn, unsigned long offsetIn, unsigned long lengthIn);
	void	Do(DisplayDirector* director);
	void	Undo(DisplayDirector* director);

protected:
	Text*        	text;
	unsigned long	offset;
	unsigned long	length;
	String    	data;
};


#endif
