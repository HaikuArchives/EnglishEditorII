/* DocumentFragment.h */

#ifndef _H_DocumentFragment_
#define _H_DocumentFragment_

#include "CompositeNode.h"

class DocumentFragment : public CompositeNode {
public:
	DocumentFragment();
	DocumentFragment(const DocumentFragment& other, bool deep);
	String     	NodeName();
	String     	NodeValue();
	unsigned short	NodeType();
	Node*         	CloneNode(bool deep);
};


#endif
