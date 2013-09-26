/* ScrollerControl.h */

#ifndef _H_ScrollerControl_
#define _H_ScrollerControl_

#include "Control.h"
#include "Color.h"

class WindowDirector;

class ScrollerControl : public Control {
public:
	ScrollerControl(WindowDirector* directorIn);
	Rectangle	GetRect();
	void     	Draw(DisplayDirector* directorIn);
	void     	MouseDown(int x, int y, DisplayDirector* directorIn);

protected:
	WindowDirector* 	director;

public:
	static const int	width;
	static const int	height;

protected:
	static Color    	thumbColor;
};


#endif
