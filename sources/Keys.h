/* Keys.h */

#ifndef _H_Keys_
#define _H_Keys_

#include "string_slice.h"

class Keys {
public:
	Keys();

	static string_slice	PageUpKey;
	static string_slice	PageDownKey;
	static string_slice	HomeKey;
	static string_slice	EndKey;
	static string_slice	UpArrowKey;
	static string_slice	DownArrowKey;
	static string_slice	LeftArrowKey;
	static string_slice	RightArrowKey;
	static string_slice	EscKey;
	static string_slice	DelKey;

protected:
	static Keys        	initializer;
};


#endif
