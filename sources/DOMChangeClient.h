/* DOMChangeClient.h */

#ifndef _H_DOMChangeClient_
#define _H_DOMChangeClient_

#include "String.h"

class Node;
class Attr;

class DOMChangeClient {
public:
	virtual void	ChildInsertedBefore(Node* newChild, Node* refChild);
	virtual void	ChildReplaced(Node* newChild, Node* oldChild);
	virtual void	ChildRemoved(Node* oldChild);
	virtual void	NodeDying();
	virtual void	DataInserted(unsigned long offset, String arg);
	virtual void	DataDeleted(unsigned long offset, unsigned long count);
	virtual void	DataSet(String newData);
	virtual void	AttributeSet(Attr* newAttr);
	virtual void	AttributeRemoved(Attr* oldAttr);
};


#endif
