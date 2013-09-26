/* DisplayElementNode.h */

#ifndef _H_DisplayElementNode_
#define _H_DisplayElementNode_

#include "CompositeDisplayNode.h"

class Element;
class Font;

class DisplayElementNode : public CompositeDisplayNode {
public:
	DisplayElementNode(Element* templateElementIn, Element* sourceElementIn);
	~DisplayElementNode();
	DisplayType	DeclaredDisplayType();
	bool       	IsDisposable();
	DOMString  	GetProperty(string_slice propertyName);
	void       	Load(DisplayDirector* director);
	bool       	CanSelectAcross();
	Font*      	CurFont();
	Node*      	FirstNode();
	Node*      	LastNode();

protected:
	Element*	templateElement;
	Element*	sourceElement;
	Font*   	font;
};


#endif
