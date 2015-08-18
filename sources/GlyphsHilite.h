/* GlyphsHilite.h */

#ifndef _H_GlyphsHilite_
#define _H_GlyphsHilite_

#include <Rect.h>

#include "GlyphRef.h"
#include <Point.h>
#include "Selection.h"

class View;

class GlyphsHilite {
public:
	GlyphsHilite(GlyphRef firstGlyphIn, GlyphRef lastGlyphIn,
             rgb_color colorIn = Selection::selectionColor);
	void      	Draw(View* view, BPoint origin, BRect tagBounds = BRect());
	BRect 	Bounds();
	bool      	IsVisible(BRect pageRect);
	bool      	ContainsPoint(BPoint point);
	BPoint	DragStartPoint(BPoint destPoint);
	BRect 	TagSpecRect();

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
