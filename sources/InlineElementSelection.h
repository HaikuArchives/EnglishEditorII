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
	void               	Draw(View* view, BPoint origin);
	void               	AcceptKey(string_slice key, DisplayDirector* director);
	void               	Promote(DisplayDirector* director);
	Selection*         	ExtendTo(BPoint point);
	Action*            	GetRestoreAction();
	BRect          	Bounds();
	bool               	IsVisible(BRect pageRect);
	bool               	NeedsClip();
	BPoint         	DragStartPoint(BPoint destPoint);
	bool               	CanCopy();
	String          	GetXMLCopy();
	bool               	CanPaste();
	void               	Paste(String pasteText, DisplayDirector* director);
	String          	TagName();
	BRect          	TagSpecRect();
	void               	TagNameChanged(String newTagName, DisplayDirector* director);
	void               	ReplaceContents(String newContents, DisplayDirector* director);

protected:
	ElementDisplayNode*	displayNode;
	GlyphsHilite*      	hilite;
};


#endif
