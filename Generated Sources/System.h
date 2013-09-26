/* System.h */

#ifndef _H_System_
#define _H_System_

#include "DOMString.h"

class EnglishEditorApp;
class Element;

class System {
public:
	static DOMString        	AppDirectory();
	static DOMString        	DocumentsDirectory();
	static DOMString        	OutgoingMailDirectory();
	static DOMString        	OutgoingNewsDirectory();
	static void             	SetFileMimeType(DOMString filePathIn, DOMString mimeTypeIn);
	static void             	OpenURL(DOMString url);
	static void             	PostNewsMessage(String filePathIn);
	static void             	SendMailMessage(String filePathIn, Element* header);
	static void             	MarkMailMessageRead(String filePathIn);
	static void             	TrashFile(DOMString filePathIn);
	static EnglishEditorApp*	GetApplication();
	static DOMString        	AdjustFileName(DOMString fileNameIn);
	static void             	CopyToClipboard(DOMString xmlData, DOMString textData);
	static String           	GetClipboardText();
	static DOMString        	BuildRecipients(DOMString toStr);
	static DOMString        	MakeRecipient(DOMString address);

protected:
	static const unsigned long	defaultDirPerms;
};


#endif
