/* GlyphDrawContext.cpp */

#include "GlyphDrawContext.h"



GlyphDrawContext::GlyphDrawContext(int originXIn, int originYIn, View* viewIn, Font* fontIn, int startYIn, int endYIn)
	: originX(originXIn), originY(originYIn), view(viewIn), font(fontIn),
	  startY(startYIn), endY(endYIn)
{
}


