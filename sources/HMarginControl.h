/* HMarginControl.h */

#ifndef _H_HMarginControl_
#define _H_HMarginControl_

#include "MarginControl.h"

class HMarginControl : public MarginControl {
public:
	HMarginControl(WindowDirector* directorIn);
	BRect	GetRect();
	void     	Draw(DisplayDirector* directorIn);
	void     	MouseDown(int x, int y, DisplayDirector* directorIn);
};


#endif
