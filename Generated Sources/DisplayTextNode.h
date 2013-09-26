/* DisplayTextNode.h */

#ifndef _H_DisplayTextNode_
#define _H_DisplayTextNode_

#include "TextDisplayNode.h"

class DisplayTextNode : public TextDisplayNode {
public:
	DisplayTextNode(Text* textIn);
	bool      	IsDisposable();
	Selection*	StartSelection();
	Selection*	EndSelection();
	bool      	CanSelectAcross();
	Selection*	InlineFindSelection(FindSelectionContext* context);
};


#endif
