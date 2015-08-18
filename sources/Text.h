/* Text.h */

#ifndef _H_Text_
#define _H_Text_

#include "CharacterData.h"

class Text : public CharacterData {
public:
	Text(String textIn);
	Text(const Text& other);
	String     	NodeName();
	String     	NodeValue();
	unsigned short	NodeType();
	Node*         	CloneNode(bool deep);
	virtual Text* 	SplitText(unsigned long offset);
	bool          	IsIgnorableWhitespace();
};


#endif
