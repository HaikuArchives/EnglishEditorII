/* InlineElementSelection.h */

#ifndef _H_InlineElementSelection_
#define _H_InlineElementSelection_

#include "TagSelection.h"
#include "GlyphRef.h"

class ElementDisplayNode;
class GlyphsHilite;
class Element;

class InlineElementSelection : public TagSelection {
public:
	InlineElementSelection(ElementDisplayNode* displayNodeIn);
	~InlineElementSelection();
	Element*           	GetElement();
	ElementDisplayNode*	GetDisplayNode();
	void               	Draw(View* view, CoordPoint origin);
	void               	AcceptKey(string_slice key, DisplayDirector* director);
	void               	Promote(DisplayDirector* director);
	Selection*         	ExtendTo(CoordPoint point);
	Action*            	GetRestoreAction();
	Rectangle          	Bounds();
	bool               	IsVisible(Rectangle pageRect);
	bool               	NeedsClip();
	CoordPoint         	DragStartPoint(CoordPoint destPoint);
	bool               	CanCopy();
	DOMString          	GetXMLCopy();
	bool               	CanPaste();
	void               	Paste(String pasteText, DisplayDirector* director);
	DOMString          	TagName();
	Rectangle          	TagSpecRect();
	void               	TagNameChanged(DOMString newTagName, DisplayDirector* director);
	void               	ReplaceContents(String newContents, DisplayDirector* director);

protected:
	ElementDisplayNode*	displayNode;
	GlyphsHilite*      	hilite;
};


#endif
