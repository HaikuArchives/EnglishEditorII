/* MenuControl.h */

#ifndef _H_MenuControl_
#define _H_MenuControl_

#include "Control.h"
#include "Color.h"

class WindowDirector;
class Font;

class MenuControl : public Control {
public:
	MenuControl(WindowDirector* directorIn);
	~MenuControl();
	Rectangle	GetRect();
	void     	Draw(DisplayDirector* directorIn);
	void     	MouseDown(int x, int y, DisplayDirector* directorIn);

protected:
	WindowDirector*   	director;
	Font*             	font;
	static const int  	xPos;
	static const int  	yPos;
	static Color      	color;
	static const int  	diameter;
	static const int  	lineWidth;
	static const char*	asterisk;
	static const char*	fontFamily;
	static const int  	fontSize;
	static const int  	fontStyle;
};


#endif
