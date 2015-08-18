/* XMLFileSource.cpp */

#include "XMLFileSource.h"
#include "Document.h"
#include "Element.h"
#include "XMLParser.h"
#include "XMLFileWriter.h"
#include "DOMUtils.h"
#include "Error.h"
#include "System.h"
#include "OSException.h"
#include <memory>	// for auto_ptr
#include <sys/stat.h>
#include <errno.h>



XMLFileSource::XMLFileSource(const string_slice filePathIn, bool isNewDoc)
	: filePath(filePathIn), fileText(NULL), document(NULL)
{
	// set up statics
	InitStatics();

	// read in the file
	// open it
	FILE* file = fopen(filePath.c_str(), "r");
	if (file == NULL)
		throw OSException("Couldn't open file.", errno);
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
		throw OSException("Couldn't read file.", errno);
		}
	fclose(file);

	// parse it
	XMLParser parser(string_slice(fileText, fileText + fileLen));
	document = parser.Parse();

	// if it's new, mark as such by clearing filePath
	if (isNewDoc)
		filePath = "";
}


XMLFileSource::~XMLFileSource()
{
	delete document;
	delete fileText;
}


Document* XMLFileSource::GetDocument()
{
	return document;
}


string_slice XMLFileSource::GetWindowTitle()
{
	if (filePath.empty())
		return "New Document";
	const char* fileName = strrchr(filePath.c_str(), '/');
	return (fileName ? fileName + 1 : "Unknown Document");
}


void XMLFileSource::Save()
{
	// if it's a new file, get the title
	bool isNewFile = false;
	if (filePath.empty()) {
		isNewFile = true;

		// find the "title" element
		Element* docElement = document->DocumentElement();
		// first check directly
		Element* titleElement = docElement->GetElementByTagName("title");
		// next check in a "head" element
		if (titleElement == NULL) {
			Element* headElement = docElement->GetElementByTagName("head");
			if (headElement)
				titleElement = headElement->GetElementByTagName("title");
			}
		// if no title, kick out
		String title;
		if (titleElement)
			title = DOMUtils::NodeContents(titleElement);
		if (title.empty()) {
			Error::ShowError("You'll need to give your document a title before you can save it.");
			return;
			}

		// create the filePath
		filePath = System::DocumentsDirectory();
		filePath += title;

		// see if there's already a file with that name
		struct stat statBuf;
		if (stat(filePath.c_str(), &statBuf) == 0) {
			Error::ShowError("There's already a document with that title.  You'll have to change the title before saving.");
			filePath = "";
			return;
			}
		}

	// open the file
	FILE* file = fopen(filePath.c_str(), "w+");
	if (file == NULL)
		throw OSException("Couldn't open file.", errno);
	// write the file
	XMLFileWriter writer(document, file);
	writer.Write();
	// close it
	fclose(file);

	// if it was a new file, set its type
	if (isNewFile)
		System::SetFileMimeType(filePath, "text/xml");
}


void XMLFileSource::InitStatics()
{
}


