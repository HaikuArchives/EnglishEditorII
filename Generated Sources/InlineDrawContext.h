/* InlineDrawContext.h */

#ifndef _H_InlineDrawContext_
#define _H_InlineDrawContext_


class View;
class BlockDrawContext;

class InlineDrawContext {
public:
	InlineDrawContext(BlockDrawContext* blockContext, int extraIndent);

	View*	view;
	int  	originX;
	int  	originY;
	int  	startY;
	int  	endY;
};


#endif
