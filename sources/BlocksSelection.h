/* BlocksSelection.h */

#ifndef _H_BlocksSelection_
#define _H_BlocksSelection_

#include "Selection.h"

class BlockableDisplayNode;

class BlocksSelection : public Selection {
public:
	BlocksSelection(BlockableDisplayNode* firstNodeIn, BlockableDisplayNode* lastNodeIn);
	void                 	Draw(View* view, CoordPoint origin);
	void                 	AcceptKey(string_slice key, DisplayDirector* director);
	void                 	Promote(DisplayDirector* director);
	Selection*           	ExtendTo(CoordPoint point);
	Action*              	GetRestoreAction();
	Rectangle            	Bounds();
	bool                 	IsVisible(Rectangle pageRect);
	bool                 	NeedsClip();
	CoordPoint           	DragStartPoint(CoordPoint destPoint);
	bool                 	CanCopy();
	DOMString            	GetXMLCopy();
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
