/* InvalidXMLException.cpp */

#include "InvalidXMLException.h"



InvalidXMLException::InvalidXMLException(string_slice textIn, const char* locationIn)
	: text(textIn), location(locationIn)
{
}


string_slice InvalidXMLException::Text()
{
	return text;
}


const char* InvalidXMLException::Location()
{
	return location;
}


