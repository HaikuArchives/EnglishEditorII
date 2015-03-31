/* GeneratedLeafBlock.h */

#ifndef _H_GeneratedLeafBlock_
#define _H_GeneratedLeafBlock_

#include "CompositeDisplayNode.h"

class GeneratedLeafBlock : public CompositeDisplayNode {
public:
	GeneratedLeafBlock(DisplayNode* parentIn);
	DisplayType	DeclaredDisplayType();
	bool       	IsLeafBlock();
	bool       	IsDisposable();
	int        	Width();
	Font*      	CurFont();
	Node*      	FirstNode();
	Node*      	LastNode();
	bool       	CanSelectAcross();
};


#endif
