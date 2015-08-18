/* XMLStringWriter.h */

#ifndef _H_XMLStringWriter_
#define _H_XMLStringWriter_

#include "XMLWriter.h"
#include "String.h"

class XMLStringWriter : public XMLWriter {
public:
	XMLStringWriter(Node* rootNodeIn);
	String	GetString();
	void     	AppendNode(Node* extraNode);
	void     	AppendString(string_slice str);
	void     	Write(const string_slice str);
	void     	Write();

protected:
	String	string;
};


#endif
