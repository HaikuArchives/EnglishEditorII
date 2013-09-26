/* BasicLiveAttr.h */

#ifndef _H_BasicLiveAttr_
#define _H_BasicLiveAttr_

#include "LiveAttr.h"
#include "BasicAttr.h"

class BasicLiveAttr : public LiveAttr, public BasicAttr {
public:
	BasicLiveAttr(DOMString nameIn);
	BasicLiveAttr(const BasicLiveAttr& other, bool deep);
	Node*	InsertBefore(Node* newChild, Node* refChild);
	Node*	ReplaceChild(Node* newChild, Node* oldChild);
	Node*	RemoveChild(Node* oldChild);
	Node*	AppendChild(Node* newChild);
	Node*	CloneNode(bool deep);
};


#endif
