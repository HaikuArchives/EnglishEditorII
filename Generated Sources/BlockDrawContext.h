/* BlockDrawContext.h */

#ifndef _H_BlockDrawContext_
#define _H_BlockDrawContext_

#include "DisplayContext.h"

class View;

class BlockDrawContext : public DisplayContext {
public:
	BlockDrawContext(DisplayDirector* directorIn, View* viewIn, int originXIn, int originYIn,
                 int startYIn, int endYIn);

	View*	view;
	int  	originX;
	int  	originY;
	int  	startY;
	int  	endY;
	int  	curY;
};


#endif
