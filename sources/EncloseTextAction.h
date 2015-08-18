/* EncloseTextAction.h */

#ifndef _H_EncloseTextAction_
#define _H_EncloseTextAction_

#include "Action.h"
#include "String.h"

class Text;

class EncloseTextAction : public Action {
public:
	EncloseTextAction(Text* textIn, String tagNameIn);
	void	Do(DisplayDirector* director);
	void	Undo(DisplayDirector* director);

protected:
	Text*    	text;
	String	tagName;
};


#endif
