/* InlineArrow.h */

#ifndef _H_InlineArrow_
#define _H_InlineArrow_

#include <GraphicsDefs.h>

#include "CoordPoint.h"
#include "Rectangle.h"

class View;

class InlineArrow {
public:
	InlineArrow(CoordPoint fromPointIn, CoordPoint toPointIn, bool above, int ascent);
	Rectangle	Bounds();
	void     	Draw(View* view, CoordPoint origin);

protected:
	CoordPoint        	fromPoint;
	CoordPoint        	toPoint;
	bool              	loops;
	bool              	arrowPointsDown;
	static const rgb_color	arrowColor;
	static const int  	arrowheadWidth;
	static const int  	arrowheadHeight;
	static const int  	arrowLineWidth;
	static const int  	loopZone;
	static const int  	loopHeight;
	static const int  	lineSlop;
};


#endif
