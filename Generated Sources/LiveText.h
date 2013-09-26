/* LiveText.h */

#ifndef _H_LiveText_
#define _H_LiveText_

#include "Text.h"
#include "DOMLiveNode.h"

class LiveText : virtual public Text, public DOMLiveNode {
public:
	inline	LiveText();
};

inline LiveText::LiveText()
{
	// not need for C++; just to trick CY into generating
}




#endif
