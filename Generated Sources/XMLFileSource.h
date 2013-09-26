/* XMLFileSource.h */

#ifndef _H_XMLFileSource_
#define _H_XMLFileSource_

#include "DocumentSource.h"
#include "String.h"

class Document;

class XMLFileSource : public DocumentSource {
public:
	XMLFileSource(const string_slice filePathIn, bool isNewDoc = false);
	~XMLFileSource();
	Document*   	GetDocument();
	string_slice	GetWindowTitle();
	void        	Save();
	static void 	InitStatics();

protected:
	String   	filePath;
	char*    	fileText;
	Document*	document;
};


#endif
