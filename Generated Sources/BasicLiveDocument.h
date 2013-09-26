/* BasicLiveDocument.h */

#ifndef _H_BasicLiveDocument_
#define _H_BasicLiveDocument_

#include "LiveDocument.h"
#include "BasicDocument.h"

class BasicLiveDocument : public LiveDocument, public BasicDocument {
public:
	BasicLiveDocument(XMLNodeFactory* nodeFactoryIn);
	BasicLiveDocument(const BasicLiveDocument& other, bool deep);
	Node*	InsertBefore(Node* newChild, Node* refChild);
	Node*	ReplaceChild(Node* newChild, Node* oldChild);
	Node*	RemoveChild(Node* oldChild);
	Node*	AppendChild(Node* newChild);
	Node*	CloneNode(bool deep);
};


#endif
