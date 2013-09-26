/* BlockElementSelection.h */

#ifndef _H_BlockElementSelection_
#define _H_BlockElementSelection_

#include "TagSelection.h"

class ElementDisplayNode;

class BlockElementSelection : public TagSelection {
public:
	BlockElementSelection(ElementDisplayNode* displayNodeIn);
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
	Rectangle          	HiliteBounds();
	void               	ReplaceContents(String newContents, DisplayDirector* director);

protected:
	ElementDisplayNode*	displayNode;
	static const int   	lineWidth;
	static const int   	xOutset;
	static const int   	yOutset;
	static const int   	cornerRadius;
	static const int   	tagXInset;
	static const int   	tagYInset;
	static const int   	lineSpill;
};


#endif
