/* Error.cpp */

#include "Error.h"
#include "qstring.h"
#include <Alert.h>


void Error::ShowError(string_slice error)
{
	qstring errStr(error);
	BAlert* alert = new BAlert("Error", errStr.c_str(), "OK");
	alert->SetShortcut(0, B_ESCAPE);
	alert->Go();
}


