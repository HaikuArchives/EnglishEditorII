/* System.h */

#ifndef _H_System_
#define _H_System_

#include "String.h"

class EnglishEditorApp;
class Element;

class System {
public:
	static String        	AppDirectory();
	static String        	DocumentsDirectory();
	static String        	OutgoingMailDirectory();
	static String        	OutgoingNewsDirectory();
	static void             	SetFileMimeType(String filePathIn, String mimeTypeIn);
	static void             	OpenURL(String url);
	static void             	PostNewsMessage(String filePathIn);
	static void             	SendMailMessage(String filePathIn, Element* header);
	static void             	MarkMailMessageRead(String filePathIn);
	static void             	TrashFile(String filePathIn);
	static EnglishEditorApp*	GetApplication();
	static String        	AdjustFileName(String fileNameIn);
	static void             	CopyToClipboard(String xmlData, String textData);
	static String           	GetClipboardText();
	static String        	BuildRecipients(String toStr);
	static String        	MakeRecipient(String address);

protected:
	static const unsigned long	defaultDirPerms;
};


#endif
