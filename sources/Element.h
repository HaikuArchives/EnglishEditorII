/* Element.h */

#ifndef _H_Element_
#define _H_Element_

#include "CompositeNode.h"

class Attr;
class NodeList;
class NamedNodeMap;

class Element : public CompositeNode {
public:
	Element(String nameIn);
	Element(const Element& other, bool deep);
	~Element();
	String     	NodeName();
	unsigned short	NodeType();
	NamedNodeMap* 	Attributes();
	Node*         	CloneNode(bool deep);
	String     	TagName();
	String     	GetAttribute(String name);
	void          	SetAttribute(String name, String value);
	Attr*         	GetAttributeNode(String name);
	Attr*         	SetAttributeNode(Attr* newAttr);
	Attr*         	RemoveAttributeNode(Attr* oldAttr);

protected:
	String    	name;
	NamedNodeMap*	attributes;
};


#endif
