/* Validator.h */

#ifndef _H_Validator_
#define _H_Validator_

#include "NodeTypeList.h"
#include "DOMString.h"

class Element;

class Validator {
public:
	virtual bool	AllowsContents(Element* parentNode, NodeTypeList nodeTypes) = 0;
	bool        	CanInsertBefore(Element* parent, NodeTypeList newNodeTypes, Node* refChild);
};


#endif
