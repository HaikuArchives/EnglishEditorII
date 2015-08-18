/* CDATASection.h */

#ifndef _H_CDATASection_
#define _H_CDATASection_

#include "Text.h"

class CDATASection : public Text {
public:
	CDATASection(String cdata);
	CDATASection(const CDATASection& other);
	String     	NodeName();
	unsigned short	NodeType();
	Node*         	CloneNode(bool deep);
	Text*         	SplitText(unsigned long offset);
};


#endif
