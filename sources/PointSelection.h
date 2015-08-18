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
	virtual BPoint	Point() = 0;
	virtual void      	InsertTag(String tagName, DisplayDirector* director) = 0;
	virtual bool      	CaretOutLeft();
	virtual bool      	CaretOutRight();
	void              	Draw(View* view, BPoint origin);
	void              	AcceptKey(string_slice key, DisplayDirector* director);
	BRect         	Bounds();
	bool              	IsVisible(BRect pageRect);
	bool              	NeedsClip();
	String         	TagName();
	BRect         	TagSpecRect();
	void              	TagNameChanged(String newTagName, DisplayDirector* director);
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
