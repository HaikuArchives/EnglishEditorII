/* Shape.h */

#ifndef _H_Shape_
#define _H_Shape_

#include <Point.h>
#include <Shape.h>

/*
	This differs a bit from the other cross-platform classes in that it's not an
	exact analog of its equivalent BeOS class (BShape)
*/

class Shape {
public:
	Shape();
	void   	MoveTo(BPoint point);
	void   	MoveTo(int x, int y);
	void   	LineTo(BPoint point);
	void   	LineTo(int x, int y);
	void   	HorizontalCornerTo(BPoint point);
	void   	HorizontalCornerTo(int x, int y);
	void   	VerticalCornerTo(BPoint point);
	void   	VerticalCornerTo(int x, int y);
	void   	Close();
	void   	Clear();
	BShape*	NativeShape();

protected:
	BShape    	shape;
	BPoint	curPoint;
};


#endif
