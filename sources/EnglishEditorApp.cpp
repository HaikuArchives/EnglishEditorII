/* EnglishEditorApp.cpp */

#include "EnglishEditorApp.h"
#include "Preferences.h"
#include "EnglishEditorWind.h"
#include "IntroDocWind.h"
//*** #include "TextFileSource.h"
#include "XMLFileSource.h"
#include "MessageFileSource.h"
#include "IntroDocSource.h"
#include "System.h"
#include "Error.h"
#include "DisplayableException.h"
#include "InternalException.h"
#include "InvalidXMLException.h"
#include "string_slice.h"
#include <Entry.h>
#include <File.h>
#include <Roster.h>
#include <Resources.h>
#include <Path.h>
#include <NodeInfo.h>

#if __profile__
#include "libprof.h"
#endif


int main(void)
{
	try {
#if __profile__
	PROFILE_INIT(200);
#endif

	EnglishEditorApp* app = new EnglishEditorApp;
	app->Run();

#if __profile__
	PROFILE_DUMP("/boot/Development/English Editor II/profile.dump");
#endif
	} catch (DisplayableException& e) {
		e.Display();
		return 1;
	}

	return 0;
}



EnglishEditorApp::EnglishEditorApp()
	: BApplication("application/x-vnd.SteveFolta.EnglishEditorII"),
	  haveOpenedFiles(false), newDocWind(NULL), introWind(NULL)
{
}


EnglishEditorApp::~EnglishEditorApp()
{
	delete Prefs();
}


void EnglishEditorApp::RefsReceived(BMessage* message)
{
	// sanity clauses
	if (message == NULL)
		return;
	uint32 type;
	int32 count;
	message->GetInfo("refs", &type, &count);
	if ( type != B_REF_TYPE )
		return;

	// process the files
	for (long i = count - 1; i >= 0; i--) {
		// get the ref & make sure it's a file
		entry_ref ref;
		if (message->FindRef("refs", i, &ref) != B_OK )
			continue;
		BEntry entry;
		entry.SetTo(&ref);
		if (!entry.IsFile())
			continue;

		// create the file & project
		BFile* file = new BFile();
		if (file->SetTo(&ref, B_READ_WRITE) != B_OK) {
			delete file;
			continue;
			}
		try {
			OpenFile(file, &entry, message);
			haveOpenedFiles = true;
			}
		catch (DisplayableException& e) {
			e.Display();
			}
		catch (InvalidXMLException& e) {
			qstring msg = "Invalid XML, somewhere near:\n\n";
			string_slice text(e.Location(), e.Text().end());
			msg += text.substr(0, 30);
			Error::ShowError(msg);
			}
		}
}


void EnglishEditorApp::ReadyToRun()
{
	if (CountWindows() == 0)
		NewFile();
}


BResources* EnglishEditorApp::Resources()
{
	app_info info;
	if (GetAppInfo(&info) != B_OK)
		return NULL;
	BFile rsrcFile(&info.ref, B_READ_ONLY);
	if (rsrcFile.InitCheck() != B_OK)
		return NULL;
	BResources* rsrcs = new BResources();
	if (rsrcs->SetTo(&rsrcFile) != B_OK) {
		delete rsrcs;
		rsrcs = NULL;
		}
	return rsrcs;
}


void EnglishEditorApp::OpenFile(BFile* file, const BEntry* entry, BMessage* message)
{
	// get the file path
	BPath filePath(entry);
	// get the MIME type
	BNodeInfo nodeInfo(file);
	char mimeTypeStr[B_MIME_TYPE_LENGTH];
	nodeInfo.GetType(mimeTypeStr);
	string_slice mimeType(mimeTypeStr);
	// open a wind, using the correct DocumentSource as determined from the MIME type
	if (mimeType == "text/xml" || mimeType == "text/xhtml")
		new EnglishEditorWind(new XMLFileSource(filePath.Path()));
	else if (mimeType == "text/x-email" || mimeType == "text/x-news" ||
	         mimeType == "message/news") {
		new EnglishEditorWind(new MessageFileSource(filePath.Path()));
		}
	else if (mimeType.startsWith("text/"))
		new EnglishEditorWind(new XMLFileSource(filePath.Path()));
	else
		throw InternalException("Can't open file of that type.");
}


void EnglishEditorApp::NewFile()
{
	if (introWind)
		introWind->Activate();
	else
		introWind = new IntroDocWind(new IntroDocSource());
/***
	if (newDocWind)
		newDocWind->Activate();
	else
		newDocWind = new NewDocWind();
***/
}


void EnglishEditorApp::WindowClosing(BWindow* window)
{
	if (window == introWind)
		introWind = NULL;
	if (CountWindows() == 1) {	// last window
		Lock();
		Quit();
		Unlock();
		}
}


DOMString EnglishEditorApp::FunctionCall(DOMString function, DOMString arg, StyleScriptable* target)
{
	if (function == "NewDocument") {
		DOMString protoPath = System::AppDirectory();
		protoPath += "Prototype Documents/";
		protoPath += target->Eval(arg);
		new EnglishEditorWind(new XMLFileSource(protoPath, true));
		if (introWind)
			introWind->Close();
		return "";
		}
	else if (function == "NewMessage") {
		DOMString protoPath = System::AppDirectory();
		protoPath += "Prototype Documents/";
		protoPath += arg;
		new EnglishEditorWind(new MessageFileSource(protoPath, true));
		if (introWind)
			introWind->Close();
		return "";
		}
	else
		return StyleScriptable::FunctionCall(function, arg, target);
}


