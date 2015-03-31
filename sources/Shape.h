/* Shape.h */

#ifndef _H_Shape_
#define _H_Shape_

#include "CoordPoint.h"
#include <Shape.h>

/*
	This differs a bit from the other cross-platform classes in that it's not an
	exact analog of its equivalent BeOS class (BShape)
*/

class Shape {
public:
	Shape();
	void   	MoveTo(CoordPoint point);
	void   	MoveTo(int x, int y);
	void   	LineTo(CoordPoint point);
	void   	LineTo(int x, int y);
	void   	HorizontalCornerTo(CoordPoint point);
	void   	HorizontalCornerTo(int x, int y);
	void   	VerticalCornerTo(CoordPoint point);
	void   	VerticalCornerTo(int x, int y);
	void   	Close();
	void   	Clear();
	BShape*	NativeShape();

protected:
	BShape    	shape;
	CoordPoint	curPoint;
};


#endif
