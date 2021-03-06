/* Destination.h */

#ifndef _H_Destination_
#define _H_Destination_

#include <GraphicsDefs.h>
#include <Rect.h>

#include <Point.h>

class Selection;
class View;
class Action;

class Destination {
public:
	Destination(Selection* selectionIn);
	virtual~Destination();
	virtual BRect	ArrowBounds() = 0;
	virtual void     	DrawArrow(View* view, BPoint origin) = 0;
	virtual Action*  	GetMoveAction() = 0;
	virtual Action*  	GetCopyAction() = 0;

protected:
	Selection*        	selection;
	static const rgb_color	arrowColor;
	static const float	arrowheadWidth;
	static const float	arrowheadHeight;
	static const float	arrowLineWidth;
};


#endif
