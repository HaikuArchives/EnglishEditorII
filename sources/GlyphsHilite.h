/* GlyphsHilite.h */

#ifndef _H_GlyphsHilite_
#define _H_GlyphsHilite_

#include "GlyphRef.h"
#include "CoordPoint.h"
#include "Rectangle.h"
#include "Selection.h"

class View;

class GlyphsHilite {
public:
	GlyphsHilite(GlyphRef firstGlyphIn, GlyphRef lastGlyphIn,
             rgb_color colorIn = Selection::selectionColor);
	void      	Draw(View* view, CoordPoint origin, Rectangle tagBounds = Rectangle());
	Rectangle 	Bounds();
	bool      	IsVisible(Rectangle pageRect);
	bool      	ContainsPoint(CoordPoint point);
	CoordPoint	DragStartPoint(CoordPoint destPoint);
	Rectangle 	TagSpecRect();

protected:
	GlyphRef          	firstGlyph;
	GlyphRef          	lastGlyph;
	rgb_color             	color;
	static const int  	lineWidth;
	static const int  	xOutset;
	static const float	yOutset;
	static const float	xCornerLength;
	static const float	yCornerLength;
	static const int  	minCornerRadius;
	static const int  	tagXInset;
	static const int  	tagYInset;
	static const int  	lineSpill;
};


#endif
