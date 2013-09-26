/* LiveCDATASection.h */

#ifndef _H_LiveCDATASection_
#define _H_LiveCDATASection_

#include "CDATASection.h"
#include "DOMLiveNode.h"

class LiveCDATASection : public CDATASection, public DOMLiveNode {
public:
	inline	LiveCDATASection();
};

inline LiveCDATASection::LiveCDATASection()
{
	// just for CY
}




#endif
