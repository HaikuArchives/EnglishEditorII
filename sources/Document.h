/* Document.h */

#ifndef _H_Document_
#define _H_Document_

#include "CompositeNode.h"

class Element;
class Text;
class Comment;
class CDATASection;
class ProcessingInstruction;
class Attr;
class EntityReference;

class Document : public CompositeNode {
public:
	Document();
	Document(const Document& other, bool deep);
	DOMString             	NodeName();
	DOMString             	NodeValue();
	unsigned short        	NodeType();
	Node*                 	CloneNode(bool deep);
	virtual Element*      	DocumentElement();
	Element*              	CreateElement(DOMString tagName);
	Text*                 	CreateTextNode(DOMString data);
	Comment*              	CreateComment(DOMString data);
	CDATASection*         	CreateCDATASection(DOMString data);
	ProcessingInstruction*	CreateProcessingInstruction(DOMString target, DOMString data);
	Attr*                 	CreateAttribute(DOMString name);
	EntityReference*      	CreateEntityReference(DOMString name);
};


#endif
