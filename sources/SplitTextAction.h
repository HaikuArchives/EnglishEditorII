/* SplitTextAction.h */

#ifndef _H_SplitTextAction_
#define _H_SplitTextAction_

#include "Action.h"

class Text;

class SplitTextAction : public Action {
public:
	SplitTextAction(Text* textIn, unsigned long splitOffsetIn);
	~SplitTextAction();
	void 	Do(DisplayDirector* director);
	void 	Undo(DisplayDirector* director);
	Text*	GetNewText();

protected:
	Text*        	text;
	unsigned long	splitOffset;
	Text*        	newText;
};


#endif
