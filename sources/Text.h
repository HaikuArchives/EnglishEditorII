/* Text.h */

#ifndef _H_Text_
#define _H_Text_

#include "CharacterData.h"

class Text : public CharacterData {
public:
	Text(DOMString textIn);
	Text(const Text& other);
	DOMString     	NodeName();
	DOMString     	NodeValue();
	unsigned short	NodeType();
	Node*         	CloneNode(bool deep);
	virtual Text* 	SplitText(unsigned long offset);
	bool          	IsIgnorableWhitespace();
};


#endif
