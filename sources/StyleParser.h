/* StyleParser.h */

#ifndef _H_StyleParser_
#define _H_StyleParser_

#include "DOMString.h"

class StyleScriptable;

class StyleParser {
public:
	static rgb_color           	ParseColor(DOMString stringIn);
	static unsigned long   	ParseHex(string_slice str);
	static long            	ParseInt(string_slice str);
	static StyleScriptable*	ParseObj(DOMString str);
	static DOMString       	Eval(DOMString expr, StyleScriptable* target);
	static DOMString       	NextArg(DOMString& args);
	static unsigned long   	DoubleHexDigit(unsigned long digit);
	static int             	ParseColorComponent(string_slice component);
	static const char*     	SkipQuote(const char* p, const char* stopper);
};


#endif
