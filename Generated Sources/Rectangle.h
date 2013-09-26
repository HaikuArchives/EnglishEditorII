/* Rectangle.h */

#ifndef _H_Rectangle_
#define _H_Rectangle_

#include <Rect.h>

class Rectangle : public BRect {
public:
	inline	Rectangle(BRect rect);
	inline	Rectangle();
	inline	Rectangle(int left, int top, int bottom, int right);
};

inline Rectangle::Rectangle(BRect rect)
	: BRect(rect)
{
}



inline Rectangle::Rectangle()
	: BRect()
{
}



inline Rectangle::Rectangle(int left, int top, int bottom, int right)
	: BRect(left, top, bottom, right)
{
}




#endif
