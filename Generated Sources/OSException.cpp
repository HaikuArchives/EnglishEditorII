/* OSException.cpp */

#include "OSException.h"
#include "Error.h"
#include "DOMString.h"
#include <string.h>



OSException::OSException(string_slice descriptionIn, status_t errIn)
	: description(descriptionIn), err(errIn)
{
}


string_slice OSException::GetDescription()
{
	return description;
}


status_t OSException::GetError()
{
	return err;
}


void OSException::Display()
{
	DOMString msg = description;
	if (err != 0) {
		msg += " (";
		msg += strerror(err);
		msg += ")";
		}
	Error::ShowError(msg);
}


