/* NodeTypeList.h */

#ifndef _H_NodeTypeList_
#define _H_NodeTypeList_

#include "String.h"

class Node;
class NodeTypeInternalList;

class NodeTypeList {
public:
	NodeTypeList();
	NodeTypeList(const NodeTypeList& other);
	~NodeTypeList();
	void     	AppendNode(String nodeType);
	void     	AppendNode(Node* node);
	int      	NumNodeTypes();
	String	NodeTypeAt(int index);
	void     	Rewind();
	void     	Step();
	bool     	IteratorValid();
	String	CurNodeType();

protected:
	NodeTypeInternalList*	nodeTypes;
	int                  	curIndex;
};


#endif
