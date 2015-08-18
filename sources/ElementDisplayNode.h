/* ElementDisplayNode.h */

#ifndef _H_ElementDisplayNode_
#define _H_ElementDisplayNode_

#include "CompositeDisplayNode.h"
#include "StyleScriptable.h"
#include "qstring.h"	//*** REMOVE FOR FINAL

class Element;

class ElementDisplayNode : public CompositeDisplayNode, public StyleScriptable {
public:
	ElementDisplayNode(Element* elementIn);
	~ElementDisplayNode();
	DisplayType	DeclaredDisplayType();
	bool       	IsDisposable();
	void       	Load(DisplayDirector* director);
	void       	UpdateProperties();
	String  	GetProperty(string_slice propertyName);
	Element*   	GetElement();
	Selection* 	GetSelection();
	Hotspot*   	GetHotspot(DisplayDirector* director);
	Font*      	CurFont();
	Node*      	FirstNode();
	Node*      	LastNode();
	bool       	CanSelectAcross();
	String  	GetScriptProperty(String property);
	String  	FunctionCall(String function, String arg, StyleScriptable* target);

protected:
	Element*	element;
	Font*   	font;
	qstring 	debugTagName;
};


#endif
