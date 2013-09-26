/* PointSelection.h */

#ifndef _H_PointSelection_
#define _H_PointSelection_

#include "TagSelection.h"

class TextDisplayNode;
class Action;
class Element;
class Glyph;

class PointSelection : public TagSelection {
public:
	PointSelection(TextDisplayNode* textNodeIn);
	~PointSelection();
	virtual CoordPoint	Point() = 0;
	virtual void      	InsertTag(DOMString tagName, DisplayDirector* director) = 0;
	virtual bool      	CaretOutLeft();
	virtual bool      	CaretOutRight();
	void              	Draw(View* view, CoordPoint origin);
	void              	AcceptKey(string_slice key, DisplayDirector* director);
	Rectangle         	Bounds();
	bool              	IsVisible(Rectangle pageRect);
	bool              	NeedsClip();
	DOMString         	TagName();
	Rectangle         	TagSpecRect();
	void              	TagNameChanged(DOMString newTagName, DisplayDirector* director);
	void              	TagEditDone(DisplayDirector* director);
	void              	SelectPastText(DisplayDirector* director);
	void              	SelectBeforeText(DisplayDirector* director);
	void              	SelectPastWord(Glyph* word, DisplayDirector* director);
	void              	SelectBeforeWord(Glyph* word, DisplayDirector* director);
	void              	HandleReturn(unsigned long offset, DisplayDirector* director);
	Action*           	DeleteCharBefore(/***/);
	Action*           	DeleteCharAfter(/***/);
	bool              	IsInFirstTextOfBlock();
	bool              	IsInLastTextOfBlock();
	Element*          	EnclosingLeafBlockElement();

protected:
	TextDisplayNode*	textNode;
	int             	ascent;
	static const int	caretWidth;
	static const int	caretHeight;
	static const int	tagXOffset;
	static const int	tagYOffset;
};


#endif
