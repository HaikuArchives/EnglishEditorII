/* InvalidXMLException.h */

#ifndef _H_InvalidXMLException_
#define _H_InvalidXMLException_

#include "string_slice.h"

class InvalidXMLException {
public:
	InvalidXMLException(string_slice textIn, const char* locationIn);
	string_slice	Text();
	const char* 	Location();

protected:
	string_slice	text;
	const char* 	location;
};


#endif
