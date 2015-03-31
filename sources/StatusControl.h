/* StatusControl.h */

#ifndef _H_StatusControl_
#define _H_StatusControl_

#include "Control.h"

class WindowDirector;
class Font;

class StatusControl : public Control {
public:
	StatusControl(WindowDirector* directorIn);
	~StatusControl();
	Rectangle	GetRect();
	void     	Draw(DisplayDirector* director);
	void     	MouseDown(int x, int y, DisplayDirector* director);

protected:
	WindowDirector*   	director;
	Font*             	font;
	static const int  	xPos;
	static const int  	yPos;
	static const char*	changedStr;
	static const char*	unsentStr;
	static const char*	sentStr;
};


#endif
