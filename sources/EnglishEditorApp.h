/* EnglishEditorApp.h */

#ifndef _H_EnglishEditorApp_
#define _H_EnglishEditorApp_

#include "StyleScriptable.h"
#include <Application.h>

class BFile;
class BMessage;
class BEntry;
class BWindow;
class NewDocWind;
class EnglishEditorWind;

class EnglishEditorApp : public BApplication, public StyleScriptable {
public:
	EnglishEditorApp();
	~EnglishEditorApp();
	void       	RefsReceived(BMessage* message);
	void       	ReadyToRun();
	BResources*	Resources();
	void       	OpenFile(BFile* file, const BEntry* entry, BMessage* message = NULL);
	void       	NewFile();
	void       	WindowClosing(BWindow* window);
	String  	FunctionCall(String function, String arg, StyleScriptable* target);

protected:
	bool              	haveOpenedFiles;
	NewDocWind*       	newDocWind;
	EnglishEditorWind*	introWind;
};


#endif
