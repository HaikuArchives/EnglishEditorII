/* BlockElementHotspot.h */

#ifndef _H_BlockElementHotspot_
#define _H_BlockElementHotspot_

#include "ElementHotspot.h"

class ElementDisplayNode;

class BlockElementHotspot : public ElementHotspot {
public:
	BlockElementHotspot(ElementDisplayNode* displayNodeIn);
	Rectangle	Bounds();
	bool     	ContainsPoint(CoordPoint point);
	void     	Draw(View* view, CoordPoint origin);

protected:
	static const int	lineWidth;
	static const int	xOutset;
	static const int	yOutset;
	static const int	cornerRadius;
	static const int	lineSpill;
};


#endif
