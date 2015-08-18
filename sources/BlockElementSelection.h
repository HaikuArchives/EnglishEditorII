/* BlockElementSelection.h */

#ifndef _H_BlockElementSelection_
#define _H_BlockElementSelection_

#include "TagSelection.h"

class ElementDisplayNode;

class BlockElementSelection : public TagSelection {
public:
	BlockElementSelection(ElementDisplayNode* displayNodeIn);
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
	BRect          	HiliteBounds();
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
