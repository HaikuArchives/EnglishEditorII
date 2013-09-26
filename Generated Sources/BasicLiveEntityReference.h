/* BasicLiveEntityReference.h */

#ifndef _H_BasicLiveEntityReference_
#define _H_BasicLiveEntityReference_

#include "LiveEntityReference.h"
#include "BasicEntityReference.h"

class BasicLiveEntityReference : public LiveEntityReference, public BasicEntityReference {
public:
	BasicLiveEntityReference(DOMString nameIn);
	BasicLiveEntityReference(const BasicLiveEntityReference& other);
	Node*	InsertBefore(Node* newChild, Node* refChild);
	Node*	ReplaceChild(Node* newChild, Node* oldChild);
	Node*	RemoveChild(Node* oldChild);
	Node*	AppendChild(Node* newChild);
	Node*	CloneNode(bool deep);
};


#endif
