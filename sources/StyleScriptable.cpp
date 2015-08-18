/* StyleScriptable.cpp */

#include "StyleScriptable.h"
#include "StyleParser.h"
#include <stdio.h>


StyleScriptable::~StyleScriptable()
{
}


String StyleScriptable::GetScriptProperty(String property)
{
	// default: no property of this type; return the property string
	return property;
}


String StyleScriptable::FunctionCall(String function, String arg, StyleScriptable* target)
{
	if (function == "if") {
		String cond = NextArg(arg);
		String ifClause = NextArg(arg);
		if (!target->Eval(cond).empty())
			return target->Eval(ifClause);
		else
			return target->Eval(NextArg(arg));
		}

	else if (function == "quote")
		return arg;

	else {
		String result = function;
		result += "(";
		result += arg;
		result += ")";
		return result;
		}
}


String StyleScriptable::Eval(String expr)
{
	return StyleParser::Eval(expr, this);
}


String StyleScriptable::NextArg(String& args)
{
	return StyleParser::NextArg(args);
}


String StyleScriptable::AsString()
{
	char result[64];
	sprintf(result, "obj(%d)", (int) this);
	String resultStr = result;
	resultStr.detach();
	return resultStr;
}


