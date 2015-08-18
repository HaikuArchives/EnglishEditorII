/* ChildrenNodeList.h */

#ifndef _H_ChildrenNodeList_
#define _H_ChildrenNodeList_

#include "NodeList.h"

class ChildrenNodeList : public NodeList {
public:
	ChildrenNodeList(Node* parentIn);
	Node*           	Item(unsigned long index);
	unsigned long   	Length();
	NodeList*       	GetElementsByTagName(String name);
	virtual Element*	GetElementByTagName(String name);
	NodeList*       	Clone();

protected:
	Node*	parent;
};


#endif
