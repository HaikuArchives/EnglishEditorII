/* SmartDoubleQuote.cpp */

#include "SmartDoubleQuote.h"


SmartDoubleQuote::SmartDoubleQuote(bool openingIn)
	: SmartQuote(openingIn)
{
}


string_slice SmartDoubleQuote::DisplayString()
{
	return (opening ? "“" : "”");
}


