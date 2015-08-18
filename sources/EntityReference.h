/* EntityReference.h */

#ifndef _H_EntityReference_
#define _H_EntityReference_

#include "Node.h"

class EntityReference : public Node {
public:
	EntityReference(String nameIn);
	EntityReference(const EntityReference& other);
	String     	NodeName();
	unsigned short	NodeType();
	Node*         	CloneNode(bool deep);
	Node*         	InsertBefore(Node* newChild, Node* refChild);
	Node*         	ReplaceChild(Node* newChild, Node* oldChild);
	Node*         	RemoveChild(Node* oldChild);
	Node*         	AppendChild(Node* newChild);

protected:
	String	name;
};


#endif
