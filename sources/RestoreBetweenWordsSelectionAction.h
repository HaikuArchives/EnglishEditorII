/* RestoreBetweenWordsSelectionAction.h */

#ifndef _H_RestoreBetweenWordsSelectionAction_
#define _H_RestoreBetweenWordsSelectionAction_

#include "Action.h"

class Text;

class RestoreBetweenWordsSelectionAction : public Action {
public:
	RestoreBetweenWordsSelectionAction(Text* textIn, unsigned long offsetIn);
	void	Do(DisplayDirector* director);
	void	Undo(DisplayDirector* director);

protected:
	Text*        	text;
	unsigned long	offset;
};


#endif
