/* StoredNodeList.h */

#ifndef _H_StoredNodeList_
#define _H_StoredNodeList_

#include "NodeList.h"

class NodesVector;

class StoredNodeList : public NodeList {
public:
	StoredNodeList(bool ownsNodesIn = true);
	~StoredNodeList();
	Node*        	Item(unsigned long index);
	unsigned long	Length();
	Node*        	InsertBefore(Node* newChild, Node* refChild);
	Node*        	ReplaceChild(Node* newChild, Node* oldChild);
	Node*        	RemoveChild(Node* oldChild);
	Node*        	AppendChild(Node* newChild);
	NodeList*    	GetElementsByTagName(DOMString name);
	Element*     	GetElementByTagName(DOMString name);
	NodeList*    	Clone();

protected:
	NodesVector*	nodes;
	bool        	ownsNodes;
};


#endif
