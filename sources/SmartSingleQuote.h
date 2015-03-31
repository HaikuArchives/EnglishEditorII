/* SmartSingleQuote.h */

#ifndef _H_SmartSingleQuote_
#define _H_SmartSingleQuote_

#include "SmartQuote.h"

class SmartSingleQuote : public SmartQuote {
public:
	SmartSingleQuote(bool openingIn);
	string_slice	DisplayString();
};


#endif
