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
	void             	Draw(View* view, CoordPoint origin);
	void             	AcceptKey(string_slice key, DisplayDirector* director);
	void             	Promote(DisplayDirector* director);
	Selection*       	ExtendTo(CoordPoint point);
	Action*          	GetRestoreAction();
	Rectangle        	Bounds();
	bool             	IsVisible(Rectangle pageRect);
	bool             	NeedsClip();
	bool             	ContainsPoint(CoordPoint point);
	CoordPoint       	DragStartPoint(CoordPoint destPoint);
	bool             	CanCopy();
	DOMString        	GetXMLCopy();
	bool             	CanPaste();
	void             	Paste(String pasteText, DisplayDirector* director);
	DOMString        	TagName();
	Rectangle        	TagSpecRect();
	void             	TagNameChanged(DOMString newTagName, DisplayDirector* director);
	void             	TagEditDone(DisplayDirector* director);
	void             	ReplaceText(String newText, DisplayDirector* director);

protected:
	GlyphRef     	firstGlyph;
	GlyphRef     	lastGlyph;
	GlyphsHilite*	hilite;
};


#endif
