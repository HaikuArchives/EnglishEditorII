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
	String             	NodeName();
	String             	NodeValue();
	unsigned short        	NodeType();
	Node*                 	CloneNode(bool deep);
	virtual Element*      	DocumentElement();
	Element*              	CreateElement(String tagName);
	Text*                 	CreateTextNode(String data);
	Comment*              	CreateComment(String data);
	CDATASection*         	CreateCDATASection(String data);
	ProcessingInstruction*	CreateProcessingInstruction(String target, String data);
	Attr*                 	CreateAttribute(String name);
	EntityReference*      	CreateEntityReference(String name);
};


#endif
