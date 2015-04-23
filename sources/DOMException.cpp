/* DOMException.cpp */

#include "DOMException.h"



DOMException::DOMException(unsigned short codeIn)
	: code(codeIn)
{
}


unsigned short DOMException::Code()
{
	return code;
}


const char* DOMException::Name()
{
	static const char* const errNames[] = {
		"Invalid index or size",
		"Text too big for DOMString",
		"Hierarchy request error",
		"Wrong document error",
		"Invalid character error",
		"No data allowed",
		"No modification allowed",
		"Not found error",
		"Not supported error",
		"Attribute is in use"
		};

	if (code >= INDEX_SIZE_ERR && code <= INUSE_ATTRIBUTE_ERR)
		return errNames[code - INDEX_SIZE_ERR];
	else
		return "Unknown DOMException";
}


