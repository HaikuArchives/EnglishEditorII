/* VMarginControl.h */

#ifndef _H_VMarginControl_
#define _H_VMarginControl_

#include "MarginControl.h"

class VMarginControl : public MarginControl {
public:
	VMarginControl(WindowDirector* directorIn);
	Rectangle	GetRect();
	void     	Draw(DisplayDirector* directorIn);
	void     	MouseDown(int x, int y, DisplayDirector* directorIn);
};


#endif
