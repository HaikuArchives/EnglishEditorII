/* AddCharsAction.h */

#ifndef _H_AddCharsAction_
#define _H_AddCharsAction_

#include "Action.h"
#include "string_slice.h"
#include "qstring.h"

class Text;

class AddCharsAction : public Action {
public:
	AddCharsAction(string_slice charsIn, Text* textIn, int atCharIn);
	void 	Do(DisplayDirector* director);
	void 	Undo(DisplayDirector* director);
	bool 	CanIncorporateNext(Action* nextActionIn);
	void 	IncorporateNext(Action* nextActionIn);
	bool 	IsNulled();
	Text*	GetText();
	int  	AtChar();
	int  	Length();

protected:
	qstring	chars;
	Text*  	text;
	int    	atChar;
};


#endif
