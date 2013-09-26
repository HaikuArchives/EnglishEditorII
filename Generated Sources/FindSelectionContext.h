/* FindSelectionContext.h */

#ifndef _H_FindSelectionContext_
#define _H_FindSelectionContext_

class FindSelectionContext {
public:
	FindSelectionContext(int mouseXIn, int mouseYIn);

	int	mouseX;
	int	mouseY;
	int	curY;
	int	xIndent;
};


#endif
