/* StyleParser.h */

#ifndef _H_StyleParser_
#define _H_StyleParser_

#include <GraphicsDefs.h>

#include "String.h"

class StyleScriptable;

class StyleParser {
public:
	static rgb_color           	ParseColor(String stringIn);
	static unsigned long   	ParseHex(string_slice str);
	static long            	ParseInt(string_slice str);
	static StyleScriptable*	ParseObj(String str);
	static String       	Eval(String expr, StyleScriptable* target);
	static String       	NextArg(String& args);
	static unsigned long   	DoubleHexDigit(unsigned long digit);
	static int             	ParseColorComponent(string_slice component);
	static const char*     	SkipQuote(const char* p, const char* stopper);
};


#endif
