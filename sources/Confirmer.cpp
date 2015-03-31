/* Confirmer.cpp */

#include "Confirmer.h"
#include <Alert.h>


int Confirmer::Confirm(const char* prompt, const char* buttonLabel1, const char* buttonLabel2, const char* buttonLabel3)
{
	BAlert* alert =
		new BAlert("Confirmer", prompt, buttonLabel1, buttonLabel2, buttonLabel3,
		           B_WIDTH_AS_USUAL, B_OFFSET_SPACING, B_WARNING_ALERT);
	alert->SetShortcut(0, B_ESCAPE);
	if (buttonLabel2 && buttonLabel2[0]) {
		char shortcut = buttonLabel2[0];
		if (shortcut >= 'A' && shortcut <= 'Z')
			shortcut += 'a' - 'A';
		alert->SetShortcut(1, shortcut);
		}
	return alert->Go();
}


