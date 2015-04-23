/* CDATASection.h */

#ifndef _H_CDATASection_
#define _H_CDATASection_

#include "Text.h"

class CDATASection : public Text {
public:
	CDATASection(DOMString cdata);
	CDATASection(const CDATASection& other);
	DOMString     	NodeName();
	unsigned short	NodeType();
	Node*         	CloneNode(bool deep);
	Text*         	SplitText(unsigned long offset);
};


#endif
