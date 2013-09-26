/* DOMUtils.h */

#ifndef _H_DOMUtils_
#define _H_DOMUtils_

#include "DOMString.h"

class Node;

class DOMUtils {
public:
	static DOMString	NodeContents(Node* node);
};


#endif
