/* Node.h */

#ifndef _H_Node_
#define _H_Node_

#include "DOMString.h"

class Document;
class NodeList;
class NamedNodeMap;
class DOMChangeClient;

// DOM node types
enum {
	ELEMENT_NODE = 1,
	ATTRIBUTE_NODE,
	TEXT_NODE,
	CDATA_SECTION_NODE,
	ENTITY_REFERENCE_NODE,
	ENTITY_NODE,
	PROCESSING_INSTRUCTION_NODE,
	COMMENT_NODE,
	DOCUMENT_NODE,
	DOCUMENT_TYPE_NODE,
	DOCUMENT_FRAGMENT_NODE,
	NOTATION_NODE
};

class Node {
public:
	Node();
	virtual~Node();
	virtual DOMString     	NodeName() = 0;
	virtual DOMString     	NodeValue();
	virtual unsigned short	NodeType() = 0;
	inline Node*          	ParentNode();
	virtual NodeList*     	ChildNodes();
	virtual Node*         	FirstChild();
	virtual Node*         	LastChild();
	inline Node*          	PreviousSibling();
	inline Node*          	NextSibling();
	virtual NamedNodeMap* 	Attributes();
	Document*             	OwnerDocument();
	virtual Node*         	InsertBefore(Node* newChild, Node* refChild) = 0;
	virtual Node*         	ReplaceChild(Node* newChild, Node* oldChild) = 0;
	virtual Node*         	RemoveChild(Node* oldChild) = 0;
	virtual Node*         	AppendChild(Node* newChild) = 0;
	virtual bool          	HasChildNodes();
	virtual Node*         	CloneNode(bool deep) = 0;
	void                  	AddChangeClient(DOMChangeClient* newChangeClient);
	void                  	RemoveChangeClient(DOMChangeClient* oldChangeClient);
	DOMChangeClient*      	GetChangeClient();
	inline void           	SetParent(Node* newParent);
	void                  	SpliceBefore(Node* otherNode);
	void                  	SpliceAfter(Node* otherNode);
	void                  	HalfSpliceBefore(Node* otherNode);
	void                  	HalfSpliceAfter(Node* otherNode);
	void                  	Unsplice();

protected:
	Node*           	parent;
	Node*           	prevSibling;
	Node*           	nextSibling;
	DOMChangeClient*	changeClient;
};

inline Node* Node::ParentNode()
{
	return parent;
}



inline Node* Node::PreviousSibling()
{
	return prevSibling;
}



inline Node* Node::NextSibling()
{
	return nextSibling;
}



inline void Node::SetParent(Node* newParent)
{
	parent = newParent;
}




#endif
