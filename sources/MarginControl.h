/* MarginControl.h */

#ifndef _H_MarginControl_
#define _H_MarginControl_

#include "Control.h"

class WindowDirector;

class MarginControl : public Control {
public:
	MarginControl(WindowDirector* directorIn);

protected:
	WindowDirector*   	director;
	static const float	barLength;
	static const float	arrowLength;
	static const float	arrowXOffset;
	static const float	arrowYOffset;
	static const float	minHMargin;
	static const float	minVMargin;
};


#endif
