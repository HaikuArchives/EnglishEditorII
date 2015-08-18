/* SpaceDestination.h */

#ifndef _H_SpaceDestination_
#define _H_SpaceDestination_

#include "Destination.h"

class TextDisplayNode;
class Space;
class InlineArrow;

class SpaceDestination : public Destination {
public:
	SpaceDestination(Selection* selectionIn, TextDisplayNode* textNodeIn, Space* spaceIn);
	~SpaceDestination();
	BRect	ArrowBounds();
	void     	DrawArrow(View* view, BPoint origin);
	Action*  	GetMoveAction();
	Action*  	GetCopyAction();
	Action*  	MakeMoveAction(bool copying);

protected:
	TextDisplayNode*	textNode;
	Space*          	space;
	InlineArrow*    	arrow;
};


#endif
