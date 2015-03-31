/* PlaceholderTextDisplayNode.h */

#ifndef _H_PlaceholderTextDisplayNode_
#define _H_PlaceholderTextDisplayNode_

#include "TextDisplayNode.h"

class ElementDisplayNode;

class PlaceholderTextDisplayNode : public TextDisplayNode {
public:
	PlaceholderTextDisplayNode(DOMString placeholderText);
	~PlaceholderTextDisplayNode();
	bool               	IsEmpty();
	DOMString          	GetProperty(string_slice propertyName);
	Selection*         	StartSelection();
	Selection*         	EndSelection();
	void               	InlineLayout(InlineLayoutContext* layoutContext);
	Selection*         	InlineFindSelection(FindSelectionContext* context);
	Destination*       	InlineFindDestination(FindDestinationContext* context);
	ElementDisplayNode*	ParentElementNode();
};


#endif
