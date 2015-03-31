/* Hotspot.h */

#ifndef _H_Hotspot_
#define _H_Hotspot_

#include "CoordPoint.h"
#include "Rectangle.h"
#include "Color.h"

class View;
class DisplayDirector;

class Hotspot {
public:
	virtual~Hotspot();
	virtual Rectangle	Bounds() = 0;
	virtual bool     	ContainsPoint(CoordPoint point) = 0;
	virtual void     	Draw(View* view, CoordPoint origin) = 0;
	virtual void     	Clicked(DisplayDirector* director) = 0;
	void             	FadeDocument(View* view, CoordPoint origin);

protected:
	static const Color	hotspotColor;
};


#endif
