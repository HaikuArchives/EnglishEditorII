/* CharacterData.h */

#ifndef _H_CharacterData_
#define _H_CharacterData_

#include "Node.h"

class CharacterData : public Node {
public:
	CharacterData(String dataIn);
	String    	Data();
	void         	SetData(String arg);
	unsigned long	Length();
	String    	SubstringData(unsigned long offset, unsigned long count);
	void         	AppendData(String arg);
	void         	InsertData(unsigned long offset, String arg);
	void         	DeleteData(unsigned long offset, unsigned long count);
	void         	ReplaceData(unsigned long offset, unsigned long count, String arg);
	Node*        	InsertBefore(Node* newChild, Node* refChild);
	Node*        	ReplaceChild(Node* newChild, Node* oldChild);
	Node*        	RemoveChild(Node* oldChild);
	Node*        	AppendChild(Node* newChild);

protected:
	String	data;
};


#endif
