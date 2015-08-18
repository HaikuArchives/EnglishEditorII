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
	BRect	Bounds();
	bool     	ContainsPoint(BPoint point);
	void     	Draw(View* view, BPoint origin);

protected:
	GlyphsHilite*	hilite;
};


#endif
