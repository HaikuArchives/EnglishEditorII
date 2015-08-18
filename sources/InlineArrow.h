/* InlineArrow.h */

#ifndef _H_InlineArrow_
#define _H_InlineArrow_

#include <GraphicsDefs.h>
#include <Rect.h>

#include <Point.h>

class View;

class InlineArrow {
public:
	InlineArrow(BPoint fromPointIn, BPoint toPointIn, bool above, int ascent);
	BRect	Bounds();
	void     	Draw(View* view, BPoint origin);

protected:
	BPoint        	fromPoint;
	BPoint        	toPoint;
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
