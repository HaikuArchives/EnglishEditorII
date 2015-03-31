/* InlineElementHotspot.h */

#ifndef _H_InlineElementHotspot_
#define _H_InlineElementHotspot_

#include "ElementHotspot.h"

class ElementDisplayNode;
class GlyphsHilite;

class InlineElementHotspot : public ElementHotspot {
public:
	InlineElementHotspot(ElementDisplayNode* displayNodeIn);
	~InlineElementHotspot();
	Rectangle	Bounds();
	bool     	ContainsPoint(CoordPoint point);
	void     	Draw(View* view, CoordPoint origin);

protected:
	GlyphsHilite*	hilite;
};


#endif
