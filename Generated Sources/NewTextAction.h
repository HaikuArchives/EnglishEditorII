/* NewTextAction.h */

#ifndef _H_NewTextAction_
#define _H_NewTextAction_

#include "Action.h"

class Document;
class Text;

class NewTextAction : public Action {
public:
	NewTextAction(Document* document);
	~NewTextAction();
	void 	Do(DisplayDirector* director);
	void 	Undo(DisplayDirector* director);
	Text*	GetText();

protected:
	Text*	text;
};


#endif
