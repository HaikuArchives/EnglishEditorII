/* Hotspot.h */

#ifndef _H_Hotspot_
#define _H_Hotspot_

#include <GraphicsDefs.h>
#include <Rect.h>

#include <Point.h>

class View;
class DisplayDirector;

class Hotspot {
public:
	virtual~Hotspot();
	virtual BRect	Bounds() = 0;
	virtual bool     	ContainsPoint(BPoint point) = 0;
	virtual void     	Draw(View* view, BPoint origin) = 0;
	virtual void     	Clicked(DisplayDirector* director) = 0;
	void             	FadeDocument(View* view, BPoint origin);

protected:
	static const rgb_color	hotspotColor;
};


#endif
