/* EmptyNodeList.h */

#ifndef _H_EmptyNodeList_
#define _H_EmptyNodeList_

#include "NodeList.h"

class EmptyNodeList : public NodeList {
public:
	EmptyNodeList();
	Node*        	Item(unsigned long index);
	unsigned long	Length();
	Node*        	AppendChild(Node* newChild);
	NodeList*    	GetElementsByTagName(String name);
	Element*     	GetElementByTagName(String name);
	NodeList*    	Clone();
};


#endif
