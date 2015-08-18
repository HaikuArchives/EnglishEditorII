/* StyleParser.cpp */

#include "StyleParser.h"
#include "StyleScriptable.h"


rgb_color StyleParser::ParseColor(DOMString stringIn)
{
	static const rgb_color aqua = { 0, 255, 255, 255 };
	static const rgb_color black = { 0, 0, 0, 255 };
	static const rgb_color blue = { 0, 0, 255, 255 };
	static const rgb_color fuchsia = { 255, 0, 255, 255 };
	static const rgb_color gray = { 128, 128, 128, 255 };
	static const rgb_color green = { 0, 128, 0, 255 };
	static const rgb_color lime = { 0, 255, 0, 255 };
	static const rgb_color maroon = { 128, 0, 0, 255 };
	static const rgb_color navy = { 0, 0, 127, 255 };
	static const rgb_color olive = { 127, 127, 0, 255 };
	static const rgb_color purple = { 127, 0, 127, 255 };
	static const rgb_color red = { 255, 0, 0, 255 };
	static const rgb_color silver = { 192, 192, 192, 255 };
	static const rgb_color teal = { 0, 127, 127, 255 };
	static const rgb_color yellow = { 255, 255, 0, 255 };
	static const rgb_color white = { 255, 255, 255, 255 };

	string_slice str = stringIn;
	str = str.trim();
	if (str.startsWith("#")) {
		rgb_color color;
		str.eraseFirst(1);
		if (str.length() == 6) {
			color.red = ParseHex(str.substr(0, 2));
			color.green = ParseHex(str.substr(2, 2));
			color.blue = ParseHex(str.substr(4, 2));
			color.alpha = 255;
			return color;
			}
		else if (str.length() == 3) {
			color.red = DoubleHexDigit(ParseHex(str.substr(0, 1)));
			color.green = DoubleHexDigit(ParseHex(str.substr(1, 1)));
			color.blue = DoubleHexDigit(ParseHex(str.substr(2, 1)));
			color.alpha = 255;
			return color;
			}
		}
	else if (str.startsWith("rgb(")) {
		rgb_color color;
		str = str.substr(4, str.length() - 5);	// get rid of "rgb(" and ")"
		string_slice component = str.separate(",", &str).trim();
		color.red = ParseColorComponent(component);
		component = str.separate(",", &str).trim();
		color.green = ParseColorComponent(component);
		component = str.separate(",", &str).trim();
		color.blue = ParseColorComponent(component);
		color.alpha = 255;
		return color;
		}
	else if (str.startsWith("rgba(")) {
		rgb_color color;
		str = str.substr(5, str.length() - 6);	// get rid of "rgba(" and ")"
		string_slice component = str.separate(",", &str).trim();
		color.red = ParseColorComponent(component);
		component = str.separate(",", &str).trim();
		color.green = ParseColorComponent(component);
		component = str.separate(",", &str).trim();
		color.blue = ParseColorComponent(component);
		component = str.separate(",", &str).trim();
		color.alpha = ParseColorComponent(component);
		return color;
		}
	else if (str == "aqua")
		return aqua;
	else if (str == "black")
		return black;
	else if (str == "blue")
		return blue;
	else if (str == "fuchsia")
		return fuchsia;
	else if (str == "gray" || str == "grey")
		return gray;
	else if (str == "green")
		return green;
	else if (str == "lime")
		return lime;
	else if (str == "maroon")
		return maroon;
	else if (str == "navy")
		return navy;
	else if (str == "olive")
		return olive;
	else if (str == "purple")
		return purple;
	else if (str == "red")
		return red;
	else if (str == "silver")
		return silver;
	else if (str == "teal")
		return teal;
	else if (str == "yellow")
		return yellow;
	else if (str == "white")
		return white;

	// default: black
	return black;
}


unsigned long StyleParser::ParseHex(string_slice str)
{
	unsigned long val = 0;
	const char* stopper = str.end();
	for (const char* p = str.begin(); p < stopper; ++p) {
		char c = *p;
		if (c >= '0' && c <= '9') {
			val *= 16;
			val += c - '0';
			}
		else if (c >= 'A' && c <= 'F') {
			val *= 16;
			val += 0x0A + c - 'A';
			}
		else if (c >= 'a' && c <= 'f') {
			val *= 16;
			val += 0x0A + c - 'a';
			}
		}
	return val;
}


long StyleParser::ParseInt(string_slice str)
{
	long val = 0;
	const char* stopper = str.end();
	int sign = 1;
	for (const char* p = str.begin(); p < stopper; ++p) {
		char c = *p;
		if (c >= '0' && c <= '9') {
			val *= 10;
			val += c - '0';
			}
		else if (c == '-')
			sign = -sign;
		}
	return sign * val;
}


StyleScriptable* StyleParser::ParseObj(DOMString str)
{
	if (str.startsWith("obj(")) {
		int addr = string_slice(str.substr(4, str.length() - 5)).asInt();
		return dynamic_cast<StyleScriptable*>((StyleScriptable*) addr);
		}
	else
		return NULL;
}


DOMString StyleParser::Eval(DOMString expr, StyleScriptable* target)
{
	int pos;

	StyleScriptable* obj = target;
	const char* p = expr.begin();
	const char* stopper = expr.end();
	DOMString result;
	while (p < stopper) {
		char c = *p++;
		if (c == '.') {
			pos = p - expr.begin() - 1;
			DOMString propertyName = expr.substr(0, pos).trim();
			expr = expr.substr(pos + 1, expr.length() - pos - 1);
			DOMString propVal = obj->GetScriptProperty(propertyName);
			StyleScriptable* nextObj = ParseObj(propVal);
			if (nextObj)
				obj = nextObj;
			else {
				result += propVal;
				result += ".";
				}
			}
		else if (c == '(') {
			int parenPos = p - expr.begin() - 1;
			DOMString funcName = expr.substr(0, parenPos).trim();
			// find the arg
			int parenLevel = 1;
			while (p < stopper) {
				c = *p++;
				if (c == '(')
					++parenLevel;
				else if (c == ')') {
					--parenLevel;
					if (parenLevel <= 0)
						break;
					}
				}
			int pos = p - expr.begin() - 1;
			DOMString arg = expr.substr(parenPos + 1, pos - parenPos - 1);
			expr = expr.substr(pos + 1, expr.length() - pos - 1);
			// call
			DOMString funcResult = obj->FunctionCall(funcName, arg, target);
			StyleScriptable* nextObj = ParseObj(funcResult);
			if (nextObj)
				obj = nextObj;
			else
				result += funcResult;
			}
		}

	// at the last bit; treat it as a property
	DOMString propVal = obj->GetScriptProperty(expr.trim());
	result += propVal;
	return result.trim();
}


DOMString StyleParser::NextArg(DOMString& args)
{
	const char* p = args.begin();
	const char* stopper = args.end();
	int parenLevel = 0;
	while (p < stopper) {
		char c = *p++;
		if (c == ',' && parenLevel <= 0) {
			int pos = p - args.begin() - 1;
			DOMString result = args.substr(0, pos);
			args = args.substr(pos + 1, args.length() - pos - 1);
			return result;
			}
		else if (c == '(')
			++parenLevel;
		else if (c == ')')
			--parenLevel;
		}

	// fell thru; it's the last arg
	DOMString result = args;
	args = "";
	return result;
}


unsigned long StyleParser::DoubleHexDigit(unsigned long digit)
{
	return (digit | digit << 4);
}


int StyleParser::ParseColorComponent(string_slice component)
{
	if (component[component.length() - 1] == '%') {
		// percentage
		return (component.substr(0, component.length() - 1).asInt() * 255) / 100;
		}
	else
		return component.asInt();
}


const char* StyleParser::SkipQuote(const char* p, const char* stopper)
{
	while (p < stopper) {
		char c = *p++;
		if (c == '\\')
			p++;
		else if (c == '"')
			break;
		}
	return p;
}


