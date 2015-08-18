/* BlocksSelection.h */

#ifndef _H_BlocksSelection_
#define _H_BlocksSelection_

#include "Selection.h"

class BlockableDisplayNode;

class BlocksSelection : public Selection {
public:
	BlocksSelection(BlockableDisplayNode* firstNodeIn, BlockableDisplayNode* lastNodeIn);
	void                 	Draw(View* view, BPoint origin);
	void                 	AcceptKey(string_slice key, DisplayDirector* director);
	void                 	Promote(DisplayDirector* director);
	Selection*           	ExtendTo(BPoint point);
	Action*              	GetRestoreAction();
	BRect            	Bounds();
	bool                 	IsVisible(BRect pageRect);
	bool                 	NeedsClip();
	BPoint           	DragStartPoint(BPoint destPoint);
	bool                 	CanCopy();
	String            	GetXMLCopy();
	BlockableDisplayNode*	FirstNode();
	BlockableDisplayNode*	LastNode();

protected:
	BlockableDisplayNode*	firstNode;
	BlockableDisplayNode*	lastNode;
	static const int     	lineWidth;
	static const int     	xOutset;
	static const int     	yOutset;
	static const int     	cornerRadius;
	static const int     	lineSpill;
};


#endif
