/* IntroDocSource.cpp */

#include "IntroDocSource.h"
#include "Document.h"
#include "Element.h"
#include "Text.h"
#include "XMLParser.h"
#include "System.h"
#include "qstring.h"
#include "OSException.h"
#include <stdio.h>
#include <errno.h>



IntroDocSource::IntroDocSource()
{
	// read in the file
	// figure out the path
	qstring filePath = string_slice(System::AppDirectory());
	filePath += "Aux-Docs/intro-window";
	// open it
	FILE* file = fopen(filePath.c_str(), "r");
	if (file == NULL)
		throw OSException("Couldn't open intro-window file.", errno);
	// find out how big it is
	fpos_t fileLen;
	if (fseek(file, 0, SEEK_END) != 0 || fgetpos(file, &fileLen) != 0) {
		fclose(file);
		throw OSException("Couldn't get size of file.", errno);
		}
	// read it
	rewind(file);
	fileText = new char[fileLen];
	fread(fileText, 1, fileLen, file);
	if (ferror(file)) {
		fclose(file);
		throw OSException("Couldn't read intro-window file.", errno);
		}
	fclose(file);
	// parse it
	XMLParser parser(string_slice(fileText, fileText + fileLen));
	document = parser.Parse();

	// build the "about" section of the document
	Element* docElement = document->DocumentElement();
	Element* aboutElement = document->CreateElement("about-section");
	docElement->InsertBefore(aboutElement, docElement->FirstChild());
	// app-name
	Element* appNameElement = document->CreateElement("app-name");
	appNameElement->AppendChild(document->CreateTextNode("The English Editor II"));
	aboutElement->AppendChild(appNameElement);
	// version
	Element* versionElement = document->CreateElement("pre-version");
	versionElement->AppendChild(document->CreateTextNode("Pre-release"));
	aboutElement->AppendChild(versionElement);
	// copyright
	Element* copyrightElement = document->CreateElement("copyright");
	copyrightElement->AppendChild(document->CreateTextNode("© 1999 Steve Folta"));
	aboutElement->AppendChild(copyrightElement);
	// credits
	Element* creditsElement = document->CreateElement("credits");
	creditsElement->AppendChild(document->CreateTextNode("Math by Drew Folta"));
	aboutElement->AppendChild(creditsElement);
	// build-date
	const char* buildDate = "Build date: " __DATE__ " " __TIME__;
	Element* buildDateElement = document->CreateElement("build-date");
	buildDateElement->AppendChild(document->CreateTextNode(buildDate));
	aboutElement->AppendChild(buildDateElement);

/***
	// create the "new document" links
	qstring protosDirName = System::AppDirectory();
	protosDirName += "Prototype Documents";
	DIR* protosDir = opendir(protosDirName.c_str());
	if (protosDir) {
		while (true) {
			struct dirent* dirEntry = readdir(protosDir);
			if (dirEntry == NULL)
				break;

			String fileName = dirEntry->d_name;
			if (fileName.startsWith("."))
				continue;
			fileName.detach();
			Element* newDocElement = document->CreateElement("new-document");
			newDocElement->AppendChild(document->CreateTextNode(fileName));
			docElement->AppendChild(newDocElement);
			}

		closedir(protosDir);
		}
***/
}


IntroDocSource::~IntroDocSource()
{
	delete document;
	delete fileText;
}


Document* IntroDocSource::GetDocument()
{
	return document;
}


string_slice IntroDocSource::GetWindowTitle()
{
	return "Create New Document";
}


void IntroDocSource::Save()
{
	// nothing to do
}


