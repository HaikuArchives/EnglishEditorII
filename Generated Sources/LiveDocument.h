/* LiveDocument.h */

#ifndef _H_LiveDocument_
#define _H_LiveDocument_

#include "Document.h"
#include "DOMLiveNode.h"

class LiveDocument : virtual public Document, public DOMLiveNode {
public:
	inline	LiveDocument();
};

inline LiveDocument::LiveDocument()
{
	// just to trick CY into generating class def
}




#endif
