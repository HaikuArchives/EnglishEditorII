/* CharacterSelection.h */

#ifndef _H_CharacterSelection_
#define _H_CharacterSelection_

#include "PointSelection.h"

class TextDisplayNode;
class Word;

class CharacterSelection : public PointSelection {
public:
	CharacterSelection(TextDisplayNode* textNodeIn, Word* wordIn, int charIndexIn);
	void              	AcceptKey(string_slice key, DisplayDirector* director);
	virtual Selection*	ExtendTo(CoordPoint point);
	void              	Promote(DisplayDirector* director);
	Action*           	GetRestoreAction();
	bool              	CanPaste();
	void              	Paste(String pasteText, DisplayDirector* director);
	CoordPoint        	Point();
	void              	InsertTag(DOMString tagName, DisplayDirector* director);

protected:
	Word*	word;
	int  	charIndex;
};


#endif
