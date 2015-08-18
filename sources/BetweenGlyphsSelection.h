/* BetweenGlyphsSelection.h */

#ifndef _H_BetweenGlyphsSelection_
#define _H_BetweenGlyphsSelection_

#include "PointSelection.h"

class TextDisplayNode;
class Glyph;

class BetweenGlyphsSelection : public PointSelection {
public:
	BetweenGlyphsSelection(TextDisplayNode* textNodeIn,
                       Glyph* firstGlyphIn, Glyph* secondGlyphIn,
                       bool promoteBeforeIn);
	void              	AcceptKey(string_slice key, DisplayDirector* director);
	virtual Selection*	ExtendTo(BPoint point);
	Action*           	GetRestoreAction();
	void              	Promote(DisplayDirector* director);
	bool              	CanPaste();
	void              	Paste(String pasteText, DisplayDirector* director);
	BPoint        	Point();
	void              	InsertTag(String tagName, DisplayDirector* director);
	bool              	CaretOutLeft();
	bool              	CaretOutRight();
	void              	SelectForward(DisplayDirector* director);
	void              	SelectBackward(DisplayDirector* director);

protected:
	Glyph*	firstGlyph;
	Glyph*	secondGlyph;
	bool  	promoteBefore;
};


#endif
