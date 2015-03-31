/* DOMLiveNode.h */

#ifndef _H_DOMLiveNode_
#define _H_DOMLiveNode_

#include "Node.h"

class DOMChangeClient;

class DOMLiveNode : virtual public Node {
public:
	DOMLiveNode();
	~DOMLiveNode();
	void            	AddChangeClient(DOMChangeClient* newChangeClient);
	void            	RemoveChangeClient(DOMChangeClient* oldChangeClient);
	DOMChangeClient*	GetChangeClient();

protected:
	DOMChangeClient*	changeClient;
};


#endif
