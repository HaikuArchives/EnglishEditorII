/* XMLStringWriter.h */

#ifndef _H_XMLStringWriter_
#define _H_XMLStringWriter_

#include "XMLWriter.h"
#include "DOMString.h"

class XMLStringWriter : public XMLWriter {
public:
	XMLStringWriter(Node* rootNodeIn);
	DOMString	GetString();
	void     	AppendNode(Node* extraNode);
	void     	AppendString(string_slice str);
	void     	Write(const string_slice str);
	void     	Write();

protected:
	DOMString	string;
};


#endif
