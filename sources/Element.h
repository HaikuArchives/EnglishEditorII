/* Element.h */

#ifndef _H_Element_
#define _H_Element_

#include "CompositeNode.h"

class Attr;
class NodeList;
class NamedNodeMap;

class Element : public CompositeNode {
public:
	Element(DOMString nameIn);
	Element(const Element& other, bool deep);
	~Element();
	DOMString     	NodeName();
	unsigned short	NodeType();
	NamedNodeMap* 	Attributes();
	Node*         	CloneNode(bool deep);
	DOMString     	TagName();
	DOMString     	GetAttribute(DOMString name);
	void          	SetAttribute(DOMString name, DOMString value);
	Attr*         	GetAttributeNode(DOMString name);
	Attr*         	SetAttributeNode(Attr* newAttr);
	Attr*         	RemoveAttributeNode(Attr* oldAttr);

protected:
	DOMString    	name;
	NamedNodeMap*	attributes;
};


#endif
