/* XMLFileWriter.h */

#ifndef _H_XMLFileWriter_
#define _H_XMLFileWriter_

#include "XMLWriter.h"
#include <stdio.h>

class Document;

class XMLFileWriter : public XMLWriter {
public:
	XMLFileWriter(Document* documentIn, FILE* fileIn);
	void	Write(const string_slice str);
	void	Write();

protected:
	FILE*	file;
};


#endif
