/* EncloseTextAction.h */

#ifndef _H_EncloseTextAction_
#define _H_EncloseTextAction_

#include "Action.h"
#include "DOMString.h"

class Text;

class EncloseTextAction : public Action {
public:
	EncloseTextAction(Text* textIn, DOMString tagNameIn);
	void	Do(DisplayDirector* director);
	void	Undo(DisplayDirector* director);

protected:
	Text*    	text;
	DOMString	tagName;
};


#endif
