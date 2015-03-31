/* LiveElement.h */

#ifndef _H_LiveElement_
#define _H_LiveElement_

#include "Element.h"
#include "DOMLiveNode.h"

class LiveElement : virtual public Element, public DOMLiveNode {
public:
	inline	LiveElement();
};

inline LiveElement::LiveElement()
{
	// just to trick CY into generating
}




#endif
