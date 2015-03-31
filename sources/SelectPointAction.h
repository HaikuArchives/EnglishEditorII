/* SelectPointAction.h */

#ifndef _H_SelectPointAction_
#define _H_SelectPointAction_

#include "Action.h"

class Text;

class SelectPointAction : public Action {
public:
	SelectPointAction(Text* textIn, unsigned long offsetIn);
	void	Do(DisplayDirector* director);
	void	Undo(DisplayDirector* director);

protected:
	Text*        	text;
	unsigned long	offset;
};


#endif
