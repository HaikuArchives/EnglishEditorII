/* NodeList.h */

#ifndef _H_NodeList_
#define _H_NodeList_

#include "String.h"

class Node;
class Element;

class NodeList {
public:
	virtual~NodeList();
	virtual Node*        	Item(unsigned long index) = 0;
	virtual unsigned long	Length() = 0;
	virtual NodeList*    	GetElementsByTagName(String name) = 0;
	virtual Element*     	GetElementByTagName(String name) = 0;
	virtual NodeList*    	Clone() = 0;
};


#endif
