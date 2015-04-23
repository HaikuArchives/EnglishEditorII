/* Comment.h */

#ifndef _H_Comment_
#define _H_Comment_

#include "CharacterData.h"

class Comment : public CharacterData {
public:
	Comment(DOMString commentIn);
	Comment(const Comment& other);
	DOMString     	NodeName();
	DOMString     	NodeValue();
	unsigned short	NodeType();
	Node*         	InsertBefore(Node* newChild, Node* refChild);
	Node*         	ReplaceChild(Node* newChild, Node* oldChild);
	Node*         	RemoveChild(Node* oldChild);
	Node*         	AppendChild(Node* newChild);
	Node*         	CloneNode(bool deep);
	DOMString     	Data();
	void          	SetData(DOMString arg);
	unsigned long 	Length();
	DOMString     	SubstringData(unsigned long offset, unsigned long count);
	void          	AppendData(DOMString arg);
	void          	InsertData(unsigned long offset, DOMString arg);
	void          	DeleteData(unsigned long offset, unsigned long count);
	void          	ReplaceData(unsigned long offset, unsigned long count, DOMString arg);

protected:
	DOMString	comment;
};


#endif
