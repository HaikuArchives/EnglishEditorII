/* SmartDoubleQuote.h */

#ifndef _H_SmartDoubleQuote_
#define _H_SmartDoubleQuote_

#include "SmartQuote.h"

class SmartDoubleQuote : public SmartQuote {
public:
	SmartDoubleQuote(bool openingIn);
	string_slice	DisplayString();
};


#endif
