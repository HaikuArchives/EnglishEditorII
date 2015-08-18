/* StyleScriptable.h */

#ifndef _H_StyleScriptable_
#define _H_StyleScriptable_

#include "String.h"

class StyleScriptable {
public:
	virtual~StyleScriptable();
	virtual String	GetScriptProperty(String property);
	virtual String	FunctionCall(String function, String arg, StyleScriptable* target);
	String        	Eval(String expr);
	static String 	NextArg(String& args);
	String        	AsString();
};


#endif
