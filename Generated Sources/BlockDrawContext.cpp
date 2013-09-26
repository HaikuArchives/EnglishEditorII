/* BlockDrawContext.cpp */

#include "BlockDrawContext.h"



BlockDrawContext::BlockDrawContext(DisplayDirector* directorIn, View* viewIn, int originXIn, int originYIn, int startYIn, int endYIn)
	: DisplayContext(directorIn), view(viewIn), originX(originXIn), originY(originYIn),
	  startY(startYIn), endY(endYIn), curY(0)
{
}


