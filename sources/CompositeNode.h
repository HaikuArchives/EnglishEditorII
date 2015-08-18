/* CompositeNode.h */

#ifndef _H_CompositeNode_
#define _H_CompositeNode_

#include "Node.h"

class Element;

class CompositeNode : public Node {
public:
	CompositeNode();
	~CompositeNode();
	NodeList*	ChildNodes();
	Node*    	FirstChild();
	Node*    	LastChild();
	Node*    	InsertBefore(Node* newChild, Node* refChild);
	Node*    	ReplaceChild(Node* newChild, Node* oldChild);
	Node*    	RemoveChild(Node* oldChild);
	Node*    	AppendChild(Node* newChild);
	bool     	HasChildNodes();
	NodeList*	GetElementsByTagName(String name);
	Element* 	GetElementByTagName(String name);
	void     	CloneChildrenFrom(const CompositeNode* other);
	void     	SetParents(Node* firstNewChild, Node* lastNewChild);

protected:
	Node*	firstChild;
	Node*	lastChild;
};


#endif
