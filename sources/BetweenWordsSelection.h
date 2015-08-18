/* BetweenWordsSelection.h */

#ifndef _H_BetweenWordsSelection_
#define _H_BetweenWordsSelection_

#include "PointSelection.h"

class TextDisplayNode;
class Space;

class BetweenWordsSelection : public PointSelection {
public:
	BetweenWordsSelection(TextDisplayNode* textNodeIn, Space* spaceIn);
	void              	AcceptKey(string_slice key, DisplayDirector* director);
	virtual Selection*	ExtendTo(BPoint point);
	Action*           	GetRestoreAction();
	bool              	CanPaste();
	void              	Paste(String pasteText, DisplayDirector* director);
	BPoint        	Point();
	void              	InsertTag(String tagName, DisplayDirector* director);
	void              	SelectForward(DisplayDirector* director);
	void              	SelectBackward(DisplayDirector* director);

protected:
	Space*	space;
};


#endif
