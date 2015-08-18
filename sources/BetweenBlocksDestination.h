/* BetweenBlocksDestination.h */

#ifndef _H_BetweenBlocksDestination_
#define _H_BetweenBlocksDestination_

#include "Destination.h"
#include <Point.h>

class BlockableDisplayNode;

class BetweenBlocksDestination : public Destination {
public:
	BetweenBlocksDestination(Selection* selectionIn, BlockableDisplayNode* parentIn,
                         BlockableDisplayNode* afterNodeIn);
	BRect	ArrowBounds();
	void     	DrawArrow(View* view, BPoint origin);
	Action*  	GetMoveAction();
	Action*  	GetCopyAction();
	Action*  	MakeMoveAction(bool copying);

protected:
	BlockableDisplayNode*	parent;
	BlockableDisplayNode*	afterNode;
	BPoint           	fromPoint;
	BPoint           	toPoint;
	static const int     	arrowheadWidth;
	static const int     	arrowheadHeight;
	static const int     	arrowLoopWidth;
	static const int     	arrowStem;
	static const int     	lineSlop;
};


#endif
