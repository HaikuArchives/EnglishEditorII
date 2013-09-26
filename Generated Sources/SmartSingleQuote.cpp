/* SmartSingleQuote.cpp */

#include "SmartSingleQuote.h"


SmartSingleQuote::SmartSingleQuote(bool openingIn)
	: SmartQuote(openingIn)
{
}


string_slice SmartSingleQuote::DisplayString()
{
	return (opening ? "‘" : "’");
}


