/* StyleScriptable.cpp */

#include "StyleScriptable.h"
#include "StyleParser.h"
#include <stdio.h>


StyleScriptable::~StyleScriptable()
{
}


DOMString StyleScriptable::GetScriptProperty(DOMString property)
{
	// default: no property of this type; return the property string
	return property;
}


DOMString StyleScriptable::FunctionCall(DOMString function, DOMString arg, StyleScriptable* target)
{
	if (function == "if") {
		DOMString cond = NextArg(arg);
		DOMString ifClause = NextArg(arg);
		if (!target->Eval(cond).empty())
			return target->Eval(ifClause);
		else
			return target->Eval(NextArg(arg));
		}

	else if (function == "quote")
		return arg;

	else {
		DOMString result = function;
		result += "(";
		result += arg;
		result += ")";
		return result;
		}
}


DOMString StyleScriptable::Eval(DOMString expr)
{
	return StyleParser::Eval(expr, this);
}


DOMString StyleScriptable::NextArg(DOMString& args)
{
	return StyleParser::NextArg(args);
}


DOMString StyleScriptable::AsString()
{
	char result[64];
	sprintf(result, "obj(%d)", (int) this);
	DOMString resultStr = result;
	resultStr.detach();
	return resultStr;
}


