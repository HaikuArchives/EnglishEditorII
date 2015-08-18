/* Control.h */

#ifndef _H_Control_
#define _H_Control_

#include <GraphicsDefs.h>
#include <Rect.h>

class DisplayDirector;

class Control {
public:
	virtual~Control();
	virtual BRect	GetRect() = 0;
	virtual void     	Draw(DisplayDirector* director) = 0;
	virtual void     	MouseDown(int x, int y, DisplayDirector* director) = 0;
	int              	CurAlpha(DisplayDirector* director);

protected:
	static const rgb_color	controlColor;
	static const float	visibleZone;
};


#endif
