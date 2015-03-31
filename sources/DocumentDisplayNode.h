/* DocumentDisplayNode.h */

#ifndef _H_DocumentDisplayNode_
#define _H_DocumentDisplayNode_

#include "CompositeDisplayNode.h"

class Document;
class DisplayDirector;

class DocumentDisplayNode : public CompositeDisplayNode {
public:
	DocumentDisplayNode(Document* documentIn, DisplayDirector* directorIn);
	~DocumentDisplayNode();
	DisplayType     	DeclaredDisplayType();
	bool            	IsDisposable();
	void            	Load(DisplayDirector* director);
	void            	UpdateProperties();
	DOMString       	GetProperty(string_slice propertyName);
	Font*           	CurFont();
	int             	Width();
	int             	LeftSpace();
	void            	BlockLayout(bool force = false);
	Node*           	FirstNode();
	Node*           	LastNode();
	bool            	CanSelectAcross();
	void            	ChildReplaced(Node* newChild, Node* oldChild);
	DisplayDirector*	Director();

protected:
	Document*       	document;
	DisplayDirector*	displayDirector;
	Font*           	font;
};


#endif
