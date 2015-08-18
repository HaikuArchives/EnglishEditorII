/* FrameRateControl.h */

#ifndef _H_FrameRateControl_
#define _H_FrameRateControl_

#include "Control.h"

class Font;
class WindowDirector;

class FrameRateControl : public Control {
public:
	FrameRateControl(WindowDirector* directorIn);
	~FrameRateControl();
	BRect	GetRect();
	void     	Draw(DisplayDirector* directorIn);
	void     	MouseDown(int x, int y, DisplayDirector* director);

protected:
	WindowDirector* 	director;
	Font*           	font;
	int             	displaying;
	static const int	xPos;
	static const int	yPos;
};


#endif
