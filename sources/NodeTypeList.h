/* NodeTypeList.h */

#ifndef _H_NodeTypeList_
#define _H_NodeTypeList_

#include "DOMString.h"

class Node;
class NodeTypeInternalList;

class NodeTypeList {
public:
	NodeTypeList();
	NodeTypeList(const NodeTypeList& other);
	~NodeTypeList();
	void     	AppendNode(DOMString nodeType);
	void     	AppendNode(Node* node);
	int      	NumNodeTypes();
	DOMString	NodeTypeAt(int index);
	void     	Rewind();
	void     	Step();
	bool     	IteratorValid();
	DOMString	CurNodeType();

protected:
	NodeTypeInternalList*	nodeTypes;
	int                  	curIndex;
};


#endif
