/* System.cpp */

#include "System.h"
#include "EnglishEditorApp.h"
#include "Element.h"
#include "DOMUtils.h"
#include "qstring.h"
#include "OSException.h"
#include "InternalException.h"
#include <E-mail.h>
#include <Clipboard.h>
#include <Roster.h>
#include <Directory.h>
#include <Entry.h>
#include <Path.h>
#include <NodeInfo.h>
#include <Node.h>
#include <Volume.h>
#include <VolumeRoster.h>
#include <FindDirectory.h>
#include <Query.h>
#include <fs_attr.h>
#include <memory>

const unsigned long System::defaultDirPerms = (S_IRUSR | S_IWUSR | S_IXUSR | S_IRGRP | S_IXGRP);


String System::AppDirectory()
{
	// find out where the app lives
	app_info appInfo;
	be_app->GetAppInfo(&appInfo);
	BDirectory parentDir;
	BEntry(&appInfo.ref).GetParent(&parentDir);
	BPath parentPath(&parentDir, NULL);		// note: can't do this in the next expression, 'cause then it's destroyed before "result" has a chance to detach() itself (which happens during the "+=")
	String result = parentPath.Path();
	result += "/";
	return result;
}


String System::DocumentsDirectory()
{
	static const char* docDirName = "/boot/home/Documents";
	static const String docDirPath = "/boot/home/Documents/";

	// make sure it exists
	mkdir(docDirName, defaultDirPerms);

	return docDirPath;
}


String System::OutgoingMailDirectory()
{
	BPath path;
	status_t result = find_directory(B_USER_DIRECTORY, &path, true);
	if (result != B_NO_ERROR)
		throw OSException("Couldn't find the outgoing mail folder.", result);
	path.Append("mail/out");

	// make sure it exists
	mkdir(path.Path(), defaultDirPerms);

	// return it
	String mailDirPath = path.Path();
	mailDirPath += "/";
	mailDirPath.detach();
	return mailDirPath;
}


String System::OutgoingNewsDirectory()
{
	static const char* newsDirName = "Usenet Posts";

	// start setting up the path
	String newsDirPath = DocumentsDirectory();
	newsDirPath += newsDirName;

	// make sure it exists
	mkdir(newsDirPath.c_str(), defaultDirPerms);

	// return it (with the trailing slash)
	newsDirPath += "/";
	return newsDirPath;
}


void System::SetFileMimeType(String filePathIn, String mimeTypeIn)
{
	qstring filePath(filePathIn);
	qstring mimeType(mimeTypeIn);
	BNode node(filePath.c_str());
	BNodeInfo(&node).SetType(mimeType.c_str());
}


void System::OpenURL(String url)
{
	// strip of beginning "URL:"
	if (url.startsWith("URL:"))
		url = url.substr(4, url.length() - 4);

	// null-terminate the URL
	static const char* nullStr = "\0\0";
	string_slice nullSlice(&nullStr[0], &nullStr[1]);
	url += nullSlice;

	BMessage msg(B_ARGV_RECEIVED);
	msg.AddString("argv", "NetPositive");
	msg.AddString("argv", url.begin());
	msg.AddInt32("argc", 2);

	BMessenger messenger("application/x-vnd.Be-NPOS", -1, NULL);
	if (messenger.IsValid())
		messenger.SendMessage(&msg);
	else
		be_roster->Launch("application/x-vnd.Be-NPOS", &msg);
}


void System::PostNewsMessage(String filePathIn)
{
	// launch the app and send it the message
	String filePath = filePathIn;
	BEntry fileEntry(filePath.c_str());
	if (fileEntry.InitCheck() != B_NO_ERROR)
		throw OSException("Couldn't find file to post it.", fileEntry.InitCheck());
	entry_ref fileRef;
	fileEntry.GetRef(&fileRef);
	BMessage postMessage('Post');
	postMessage.AddRef("refs", &fileRef);
	be_roster->Launch("application/x-vnd.Folta-Item", &postMessage);
}


void System::SendMailMessage(String filePathIn, Element* header)
{
	ssize_t result;

	// get ready to set up the attributes
	String filePath = filePathIn;
	BNode node(filePath.c_str());
	if (node.InitCheck() != B_OK)
		throw OSException("Couldn't find the mail message", node.InitCheck());

	// set up the info attributes the mail daemon needs
	// from
	mail_pop_account mailInfo;
	status_t err = get_pop_account(&mailInfo);
	if (err != B_NO_ERROR)
		throw InternalException("Couldn't get your email info.");
	String fromAddr = mailInfo.reply_to;
	if (fromAddr.empty())
		fromAddr = mailInfo.pop_name;
	if (fromAddr.empty())
		throw InternalException("You need to configure your email.");
	String from;
	if (mailInfo.real_name[0]) {
		from += "\"";
		from += mailInfo.real_name;
		from += "\" ";
		}
	from += "<";
	from += fromAddr;
	from += ">";
	from.c_str();
	result = node.WriteAttr(B_MAIL_ATTR_FROM, B_STRING_TYPE, 0,
	                        from.data(), from.length() + 1);
	if (result < 0)
		throw OSException("Couldn't set the \"From\" attribute of the email message.", result);
	// to
	Element* toElement = header->GetElementByTagName("to");
	String to;
	if (toElement)
		to = DOMUtils::NodeContents(toElement).trim();
	if (to.empty())
		throw InternalException("Email message has no recipient.");
	to.c_str();
	result = node.WriteAttr(B_MAIL_ATTR_TO, B_STRING_TYPE, 0,
	                        to.data(), to.length() + 1);
	if (result < 0)
		throw OSException("Couldn't set the \"To\" attribute of the email message.", result);
	// recipients
	String recipients = BuildRecipients(to);
	recipients.c_str();
	result = node.WriteAttr(B_MAIL_ATTR_RECIPIENTS, B_STRING_TYPE, 0,
	                        recipients.data(), recipients.length() + 1);
	if (result < 0)
		throw OSException("Couldn't set the \"Recipients\" attribute of the email message.", result);
	// subject
	Element* titleElement = header->GetElementByTagName("title");
	String subject;
	if (titleElement)
		subject = DOMUtils::NodeContents(titleElement).trim();
	subject.c_str();
	result = node.WriteAttr(B_MAIL_ATTR_SUBJECT, B_STRING_TYPE, 0,
	                        subject.data(), subject.length() + 1);
	// when
	time_t when = real_time_clock();
		// despite what the BeBook says, B_TIME_TYPE is a time_t, not a bigtime_t
	result = node.WriteAttr(B_MAIL_ATTR_WHEN, B_TIME_TYPE, 0, &when, sizeof(when));

	// mark the message as ready to be sent
	string_slice pendingStr = "Pending";
	result = node.WriteAttr(B_MAIL_ATTR_STATUS, B_STRING_TYPE, 0,
	                        pendingStr.begin(), pendingStr.length());
	if (result < 0)
		throw OSException("Couldn't mark mail message as Pending.", result);
	int32 flags = B_MAIL_PENDING | B_MAIL_SAVE;
	result = node.WriteAttr(B_MAIL_ATTR_FLAGS, B_INT32_TYPE, 0, &flags, sizeof(flags));
	if (result < 0)
		throw OSException("Couldn't mark mail message to be sent.", result);

	// send it
	//result = send_queued_mail();
	result = -1; // TODO
	if (result < 0)
		throw OSException("Couldn't send the email.", result);
}


void System::MarkMailMessageRead(String filePathIn)
{
	ssize_t result;

	// find the node
	String filePath = filePathIn;
	BNode node(filePath.c_str());
	if (node.InitCheck() != B_OK)
		throw OSException("Couldn't find the mail message", node.InitCheck());

	// check if it's read; we'll only mark "New" messages (or ones without any
	// mail-status attribute)
	attr_info attrInfo;
	status_t err = node.GetAttrInfo(B_MAIL_ATTR_STATUS, &attrInfo);
	if (err == B_NO_ERROR) {
		char* statusStr = new char[attrInfo.size];
		auto_ptr<char> statusStrDeleter(statusStr);
		result = node.ReadAttr(B_MAIL_ATTR_STATUS, B_STRING_TYPE, 0,
		                       statusStr, attrInfo.size);
		if (result < 0)
			return;
		if (string_slice(statusStr) != "New")
			return;
		}

	// mark as read
	qstring readStr = "Read";
	result = node.WriteAttr(B_MAIL_ATTR_STATUS, B_STRING_TYPE, 0,
	                        readStr.c_str(), readStr.length() + 1);
	// don't complain if it fails
}


void System::TrashFile(String filePathIn)
{
	String filePath = filePathIn;
	BEntry entry(filePath.c_str());
	BVolume volume;
	status_t result = entry.GetVolume(&volume);
	if (result != B_NO_ERROR)
		throw OSException("Couldn't trash the file.", result);
	BPath trashPath;
	result = find_directory(B_TRASH_DIRECTORY, &trashPath, true, &volume);
	if (result != B_NO_ERROR)
		throw OSException("Couldn't find the trash to trash the file.", result);
	BDirectory trashDir(trashPath.Path());
	result = entry.MoveTo(&trashDir, NULL, true);
	if (result != B_NO_ERROR)
		throw OSException("Couldn't trash the file.", result);
}


EnglishEditorApp* System::GetApplication()
{
	return ((EnglishEditorApp*) be_app);
}


String System::AdjustFileName(String fileNameIn)
{
	String adjustedFileName;
	const char* p = fileNameIn.begin();
	const char* stopper = fileNameIn.end();
	const char* runStart = p;
	for (; p < stopper; ++p) {
		if (*p == '/') {
			if (p > runStart)
				adjustedFileName += string_slice(runStart, p);
			adjustedFileName += "\\";
			runStart = p + 1;
			}
		}
	if (p > runStart)
		adjustedFileName += string_slice(runStart, p);
	return adjustedFileName;
}


void System::CopyToClipboard(String xmlData, String textData)
{
	if (!be_clipboard->Lock())
		return;
	be_clipboard->Clear();
	BMessage* clip = be_clipboard->Data();
	if (clip) {
		clip->AddData("text/xml", B_MIME_TYPE, xmlData.begin(), xmlData.length());
		clip->AddData("text/plain", B_MIME_TYPE, textData.begin(), textData.length());
		be_clipboard->Commit();
		}
	be_clipboard->Unlock();
}


String System::GetClipboardText()
{
	if (!be_clipboard->Lock())
		return String();
	BMessage* clip = be_clipboard->Data();
	be_clipboard->Unlock();
	if (clip == NULL)
		return String();
	const char* text;
	int32 textLen;
	status_t result =
		clip->FindData("text/plain", B_MIME_TYPE, (const void**) &text, &textLen);
	String clipText;
	if (result == B_NO_ERROR) {
		clipText = string_slice(text, text + textLen);
		clipText.detach();
		}
	return clipText;
}


String System::BuildRecipients(String toStr)
{
	// get the recipients, one-by-one
	String recipients;
	const char* p = toStr.begin();
	const char* stopper = toStr.end();
	const char* recipientStart = p;
	for (;; ++p) {
		if (p >= stopper || *p == ',') {
			string_slice recipient = string_slice(recipientStart, p).trim();
			if (!recipient.empty()) {
				if (!recipients.empty())
					recipients += ", ";
				String cookedRecipient = MakeRecipient(recipient);
				if (cookedRecipient.empty())
					throw InternalException("One of the recipients was invalid.");
				recipients += cookedRecipient;
				}
			recipientStart = p + 1;
			if (p >= stopper)
				break;
			}
		}
	return recipients;
}


String System::MakeRecipient(String address)
{
	// first, find a token with an '@' in it
	const char* p = address.begin();
	const char* stopper = address.end();
	const char* tokenStart = p;
	bool hasAt = false;
	for (;; ++p) {
		char c = (p < stopper ? *p : '\0');
		switch (c) {
			case ' ':
			case '\t':
			case '\r':
			case '\n':
			case '<':
			case '>':
			case '(':
			case ')':
			case '\0':
				// any one of these represent the end of a token
				if (hasAt) {
					// this is the token we want; return it
					return string_slice(tokenStart, p);
					}
				// set up to process the next token
				tokenStart = p + 1;
				break;

			case '@':
				hasAt = true;
				break;
			}
		if (c == '\0')
			break;
		}

	// didn't find an email address; treat it as a nickname
	BQuery query;
	query.PushAttr("META:nickname");
	query.PushString(qstring(address).c_str(), true);
	query.PushOp(B_EQ);
	BVolume bootVolume;
	BVolumeRoster().GetBootVolume(&bootVolume);
	query.SetVolume(&bootVolume);
	status_t result = query.Fetch();
	if (result != B_NO_ERROR)
		return "";
	BEntry entry;
	result = query.GetNextEntry(&entry, true);
	if (result != B_NO_ERROR)
		return "";
	BNode node(&entry);
	if (node.InitCheck() != B_NO_ERROR)
		return "";
	const int bufLen = 256;
	char email[bufLen];
	ssize_t bytesRead = node.ReadAttr("META:email", B_STRING_TYPE, 0, email, bufLen);
	if (bytesRead < 0)
		return "";
	return String(email).detach();
}


