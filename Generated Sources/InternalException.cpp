/* InternalException.cpp */

#include "InternalException.h"
#include "Error.h"



InternalException::InternalException(string_slice descIn)
	: desc(descIn)
{
}


string_slice InternalException::Description()
{
	return desc;
}


void InternalException::Display()
{
	Error::ShowError(desc);
}


