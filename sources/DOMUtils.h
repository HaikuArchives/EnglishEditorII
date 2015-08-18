/* DOMUtils.h */

#ifndef _H_DOMUtils_
#define _H_DOMUtils_

#include "String.h"

class Node;

class DOMUtils {
public:
	static String	NodeContents(Node* node);
};


#endif
