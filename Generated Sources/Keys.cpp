/* Keys.cpp */

#include "Keys.h"
#include <InterfaceDefs.h>

string_slice Keys::PageUpKey;
string_slice Keys::PageDownKey;
string_slice Keys::HomeKey;
string_slice Keys::EndKey;
string_slice Keys::UpArrowKey;
string_slice Keys::DownArrowKey;
string_slice Keys::LeftArrowKey;
string_slice Keys::RightArrowKey;
string_slice Keys::EscKey;
string_slice Keys::DelKey;
Keys Keys::initializer;


Keys::Keys()
{
	static const char specialKeys[] = {
		B_PAGE_UP,
		B_PAGE_DOWN,
		B_HOME,
		B_END,
		B_UP_ARROW,
		B_DOWN_ARROW,
		B_LEFT_ARROW,
		B_RIGHT_ARROW,
		B_ESCAPE,
		B_DELETE
	};

	PageUpKey = string_slice(&specialKeys[0], &specialKeys[1]);
	PageDownKey = string_slice(&specialKeys[1], &specialKeys[2]);
	HomeKey = string_slice(&specialKeys[2], &specialKeys[3]);
	EndKey = string_slice(&specialKeys[3], &specialKeys[4]);
	UpArrowKey = string_slice(&specialKeys[4], &specialKeys[5]);
	DownArrowKey = string_slice(&specialKeys[5], &specialKeys[6]);
	LeftArrowKey = string_slice(&specialKeys[6], &specialKeys[7]);
	RightArrowKey = string_slice(&specialKeys[7], &specialKeys[8]);
	EscKey = string_slice(&specialKeys[8], &specialKeys[9]);
	DelKey = string_slice(&specialKeys[9], &specialKeys[10]);
}


