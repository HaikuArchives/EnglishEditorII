/* MessageFileWriter.h */

#ifndef _H_MessageFileWriter_
#define _H_MessageFileWriter_

#include "string_slice.h"

class Document;
class TextFileWriter;
class Node;

class MessageFileWriter {
public:
	MessageFileWriter(string_slice filePath, Document* documentIn);
	~MessageFileWriter();
	void	Write();
	void	WriteContents(Node* node);

protected:
	TextFileWriter*	out;
	Document*      	document;
};


#endif
