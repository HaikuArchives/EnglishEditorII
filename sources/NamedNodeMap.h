/* NamedNodeMap.h */

#ifndef _H_NamedNodeMap_
#define _H_NamedNodeMap_

#include "DOMString.h"

class Node;

class NamedNodeMap {
public:
	virtual~NamedNodeMap();
	virtual Node*        	GetNamedItem(DOMString name) = 0;
	virtual Node*        	SetNamedItem(Node* arg) = 0;
	virtual Node*        	RemoveNamedItem(DOMString name) = 0;
	virtual Node*        	Item(unsigned long index) = 0;
	virtual unsigned long	Length() = 0;
	virtual NamedNodeMap*	Clone() = 0;
};


#endif
