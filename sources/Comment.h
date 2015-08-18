/* Comment.h */

#ifndef _H_Comment_
#define _H_Comment_

#include "CharacterData.h"

class Comment : public CharacterData {
public:
	Comment(String commentIn);
	Comment(const Comment& other);
	String     	NodeName();
	String     	NodeValue();
	unsigned short	NodeType();
	Node*         	InsertBefore(Node* newChild, Node* refChild);
	Node*         	ReplaceChild(Node* newChild, Node* oldChild);
	Node*         	RemoveChild(Node* oldChild);
	Node*         	AppendChild(Node* newChild);
	Node*         	CloneNode(bool deep);
	String     	Data();
	void          	SetData(String arg);
	unsigned long 	Length();
	String     	SubstringData(unsigned long offset, unsigned long count);
	void          	AppendData(String arg);
	void          	InsertData(unsigned long offset, String arg);
	void          	DeleteData(unsigned long offset, unsigned long count);
	void          	ReplaceData(unsigned long offset, unsigned long count, String arg);

protected:
	String	comment;
};


#endif
