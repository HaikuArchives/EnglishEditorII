/* BetweenBlocksDestination.h */

#ifndef _H_BetweenBlocksDestination_
#define _H_BetweenBlocksDestination_

#include "Destination.h"
#include "CoordPoint.h"

class BlockableDisplayNode;

class BetweenBlocksDestination : public Destination {
public:
	BetweenBlocksDestination(Selection* selectionIn, BlockableDisplayNode* parentIn,
                         BlockableDisplayNode* afterNodeIn);
	Rectangle	ArrowBounds();
	void     	DrawArrow(View* view, CoordPoint origin);
	Action*  	GetMoveAction();
	Action*  	GetCopyAction();
	Action*  	MakeMoveAction(bool copying);

protected:
	BlockableDisplayNode*	parent;
	BlockableDisplayNode*	afterNode;
	CoordPoint           	fromPoint;
	CoordPoint           	toPoint;
	static const int     	arrowheadWidth;
	static const int     	arrowheadHeight;
	static const int     	arrowLoopWidth;
	static const int     	arrowStem;
	static const int     	lineSlop;
};


#endif
