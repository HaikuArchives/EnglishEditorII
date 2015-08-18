/* GlyphsSelection.h */

#ifndef _H_GlyphsSelection_
#define _H_GlyphsSelection_

#include "TagSelection.h"
#include "GlyphRef.h"
#include "InternalException.h"

class GlyphsHilite;

class GlyphsSelection : public TagSelection {
public:
	GlyphsSelection(GlyphRef firstGlyphIn, GlyphRef lastGlyphIn);
	~GlyphsSelection();
	bool             	HasEmbeddedElement();
	GlyphRef         	FirstGlyph();
	GlyphRef         	LastGlyph();
	static Selection*	SelectionFromTo(GlyphRef firstGlyph, GlyphRef lastGlyph);
	void             	Draw(View* view, BPoint origin);
	void             	AcceptKey(string_slice key, DisplayDirector* director);
	void             	Promote(DisplayDirector* director);
	Selection*       	ExtendTo(BPoint point);
	Action*          	GetRestoreAction();
	BRect        	Bounds();
	bool             	IsVisible(BRect pageRect);
	bool             	NeedsClip();
	bool             	ContainsPoint(BPoint point);
	BPoint       	DragStartPoint(BPoint destPoint);
	bool             	CanCopy();
	String        	GetXMLCopy();
	bool             	CanPaste();
	void             	Paste(String pasteText, DisplayDirector* director);
	String        	TagName();
	BRect        	TagSpecRect();
	void             	TagNameChanged(String newTagName, DisplayDirector* director);
	void             	TagEditDone(DisplayDirector* director);
	void             	ReplaceText(String newText, DisplayDirector* director);

protected:
	GlyphRef     	firstGlyph;
	GlyphRef     	lastGlyph;
	GlyphsHilite*	hilite;
};


#endif
