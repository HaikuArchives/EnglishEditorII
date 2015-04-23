/* NodeList.h */

#ifndef _H_NodeList_
#define _H_NodeList_

#include "DOMString.h"

class Node;
class Element;

class NodeList {
public:
	virtual~NodeList();
	virtual Node*        	Item(unsigned long index) = 0;
	virtual unsigned long	Length() = 0;
	virtual NodeList*    	GetElementsByTagName(DOMString name) = 0;
	virtual Element*     	GetElementByTagName(DOMString name) = 0;
	virtual NodeList*    	Clone() = 0;
};


#endif
