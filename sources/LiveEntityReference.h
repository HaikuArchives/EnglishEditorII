/* LiveEntityReference.h */

#ifndef _H_LiveEntityReference_
#define _H_LiveEntityReference_

#include "EntityReference.h"
#include "DOMLiveNode.h"

class LiveEntityReference : virtual public EntityReference, public DOMLiveNode {
public:
	inline	LiveEntityReference();
};

inline LiveEntityReference::LiveEntityReference()
{
	// just to trick CY into generating class ref
}




#endif
