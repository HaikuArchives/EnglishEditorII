/* ProcessingInstruction.h */

#ifndef _H_ProcessingInstruction_
#define _H_ProcessingInstruction_

#include "Node.h"

class ProcessingInstruction : public Node {
public:
	ProcessingInstruction(DOMString targetIn, DOMString dataIn);
	ProcessingInstruction(const ProcessingInstruction& other);
	DOMString     	NodeName();
	DOMString     	NodeValue();
	unsigned short	NodeType();
	Node*         	InsertBefore(Node* newChild, Node* refChild);
	Node*         	ReplaceChild(Node* newChild, Node* oldChild);
	Node*         	RemoveChild(Node* oldChild);
	Node*         	AppendChild(Node* newChild);
	Node*         	CloneNode(bool deep);
	DOMString     	Target();
	DOMString     	Data();

protected:
	DOMString	target;
	DOMString	data;
};


#endif
