/* Attr.h */

#ifndef _H_Attr_
#define _H_Attr_

#include "CompositeNode.h"

class Attr : public CompositeNode {
public:
	Attr(String nameIn);
	Attr(const Attr& other, bool deep);
	String     	NodeName();
	String     	NodeValue();
	unsigned short	NodeType();
	String     	Name();
	bool          	Specified();
	String     	Value();
	Node*         	InsertBefore(Node* newChild, Node* refChild);
	Node*         	ReplaceChild(Node* newChild, Node* oldChild);
	Node*         	AppendChild(Node* newChild);
	Node*         	CloneNode(bool deep);

protected:
	String	name;
};


#endif
