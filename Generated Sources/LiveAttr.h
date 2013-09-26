/* LiveAttr.h */

#ifndef _H_LiveAttr_
#define _H_LiveAttr_

#include "Attr.h"
#include "DOMLiveNode.h"

class LiveAttr : virtual public Attr, public DOMLiveNode {
public:
	inline	LiveAttr();
};

inline LiveAttr::LiveAttr()
{
	// just to trick CY into generating class def
}




#endif
