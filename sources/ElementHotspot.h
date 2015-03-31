/* ElementHotspot.h */

#ifndef _H_ElementHotspot_
#define _H_ElementHotspot_

#include "Hotspot.h"

class ElementDisplayNode;

class ElementHotspot : public Hotspot {
public:
	ElementHotspot(ElementDisplayNode* displayNodeIn);
	void	Clicked(DisplayDirector* director);

protected:
	ElementDisplayNode*	displayNode;
};


#endif
