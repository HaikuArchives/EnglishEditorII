/* Attr.h */

#ifndef _H_Attr_
#define _H_Attr_

#include "CompositeNode.h"

class Attr : public CompositeNode {
public:
	Attr(DOMString nameIn);
	Attr(const Attr& other, bool deep);
	DOMString     	NodeName();
	DOMString     	NodeValue();
	unsigned short	NodeType();
	DOMString     	Name();
	bool          	Specified();
	DOMString     	Value();
	Node*         	InsertBefore(Node* newChild, Node* refChild);
	Node*         	ReplaceChild(Node* newChild, Node* oldChild);
	Node*         	AppendChild(Node* newChild);
	Node*         	CloneNode(bool deep);

protected:
	DOMString	name;
};


#endif
