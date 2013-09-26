/* FindDestinationContext.h */

#ifndef _H_FindDestinationContext_
#define _H_FindDestinationContext_


class Selection;

class FindDestinationContext {
public:
	FindDestinationContext(Selection* sourceIn, int mouseXIn, int mouseYIn);

	Selection*	source;
	int       	mouseX;
	int       	mouseY;
	int       	curY;
	int       	xIndent;
};


#endif
