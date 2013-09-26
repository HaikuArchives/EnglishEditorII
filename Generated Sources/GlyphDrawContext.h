/* GlyphDrawContext.h */

#ifndef _H_GlyphDrawContext_
#define _H_GlyphDrawContext_


class View;
class Font;

class GlyphDrawContext {
public:
	GlyphDrawContext(int originXIn, int originYIn, View* viewIn, Font* fontIn,
                 int startYIn, int endYIn);

	int  	originX;
	int  	originY;
	View*	view;
	Font*	font;
	int  	startY;
	int  	endY;
};


#endif
