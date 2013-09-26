/* BasicLiveElement.h */

#ifndef _H_BasicLiveElement_
#define _H_BasicLiveElement_

#include "LiveElement.h"
#include "BasicElement.h"

class BasicLiveElement : public LiveElement, public BasicElement {
public:
	BasicLiveElement(DOMString name);
	BasicLiveElement(const BasicLiveElement& other, bool deep);
	Node*	InsertBefore(Node* newChild, Node* refChild);
	Node*	ReplaceChild(Node* newChild, Node* oldChild);
	Node*	RemoveChild(Node* oldChild);
	Node*	AppendChild(Node* newChild);
	Attr*	SetAttributeNode(Attr* newAttr);
	Attr*	RemoveAttributeNode(Attr* oldAttr);
	Node*	CloneNode(bool deep);
};


#endif
