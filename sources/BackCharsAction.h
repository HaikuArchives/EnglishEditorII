/* BackCharsAction.h */

#ifndef _H_BackCharsAction_
#define _H_BackCharsAction_

#include "Action.h"
#include "qstring.h"

class Text;

class BackCharsAction : public Action {
public:
	BackCharsAction(Text* textIn, int whichCharIn, int numBytes = 1);
	~BackCharsAction();
	void 	Do(DisplayDirector* director);
	void 	Undo(DisplayDirector* director);
	bool 	CanIncorporateNext(Action* nextAction);
	void 	IncorporateNext(Action* nextAction);
	Text*	GetText();
	int  	WhichChar();
	int  	NumChars();

protected:
	Text*  	text;
	int    	whichChar;
	qstring	chars;
};


#endif
