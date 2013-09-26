/* FindHotspotContext.h */

#ifndef _H_FindHotspotContext_
#define _H_FindHotspotContext_


class DisplayDirector;

class FindHotspotContext {
public:
	FindHotspotContext(DisplayDirector* directorIn, int mouseXIn, int mouseYIn);

	DisplayDirector*	director;
	int             	mouseX;
	int             	mouseY;
	int             	curY;
	int             	xIndent;
};


#endif
