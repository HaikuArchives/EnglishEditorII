/* MessageFileSource.h */

#ifndef _H_MessageFileSource_
#define _H_MessageFileSource_

#include "DocumentSource.h"
#include "String.h"

class Document;
class Node;

class MessageFileSource : public DocumentSource {
public:
	MessageFileSource(const string_slice filePathIn, bool isNewMessageIn = false);
	MessageFileSource(Document* documentIn, bool isNewMessageIn = false);
	~MessageFileSource();
	Document*      	GetDocument();
	string_slice   	GetWindowTitle();
	DocumentSource*	MakeMenuDocSource();
	void           	AttachedToDirector(DisplayDirector* directorIn);
	void           	Save();
	bool           	CanCloseOnSpacebar();
	bool           	IsSent();
	bool           	IsNewMessage();
	void           	Reply(bool toNews);
	Node*          	CloneNode(Node* origNode, Document* replyDoc);
	void           	Post();
	void           	Send();
	void           	Trash();
	String      	FunctionCall(String function, String arg, StyleScriptable* target);
	static void    	InitStatics();

protected:
	String          	filePath;
	char*           	fileText;
	Document*       	document;
	DisplayDirector*	director;
	bool            	isNewMessage;
	bool            	isSent;
};


#endif
