/* CharacterData.h */

#ifndef _H_CharacterData_
#define _H_CharacterData_

#include "Node.h"

class CharacterData : public Node {
public:
	CharacterData(DOMString dataIn);
	DOMString    	Data();
	void         	SetData(DOMString arg);
	unsigned long	Length();
	DOMString    	SubstringData(unsigned long offset, unsigned long count);
	void         	AppendData(DOMString arg);
	void         	InsertData(unsigned long offset, DOMString arg);
	void         	DeleteData(unsigned long offset, unsigned long count);
	void         	ReplaceData(unsigned long offset, unsigned long count, DOMString arg);
	Node*        	InsertBefore(Node* newChild, Node* refChild);
	Node*        	ReplaceChild(Node* newChild, Node* oldChild);
	Node*        	RemoveChild(Node* oldChild);
	Node*        	AppendChild(Node* newChild);

protected:
	DOMString	data;
};


#endif
