/* StyleScriptable.h */

#ifndef _H_StyleScriptable_
#define _H_StyleScriptable_

#include "DOMString.h"

class StyleScriptable {
public:
	virtual~StyleScriptable();
	virtual DOMString	GetScriptProperty(DOMString property);
	virtual DOMString	FunctionCall(DOMString function, DOMString arg, StyleScriptable* target);
	DOMString        	Eval(DOMString expr);
	static DOMString 	NextArg(DOMString& args);
	DOMString        	AsString();
};


#endif
