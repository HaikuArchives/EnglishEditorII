/* ProcessingInstruction.h */

#ifndef _H_ProcessingInstruction_
#define _H_ProcessingInstruction_

#include "Node.h"

class ProcessingInstruction : public Node {
public:
	ProcessingInstruction(String targetIn, String dataIn);
	ProcessingInstruction(const ProcessingInstruction& other);
	String     	NodeName();
	String     	NodeValue();
	unsigned short	NodeType();
	Node*         	InsertBefore(Node* newChild, Node* refChild);
	Node*         	ReplaceChild(Node* newChild, Node* oldChild);
	Node*         	RemoveChild(Node* oldChild);
	Node*         	AppendChild(Node* newChild);
	Node*         	CloneNode(bool deep);
	String     	Target();
	String     	Data();

protected:
	String	target;
	String	data;
};


#endif
