/* XMLParser.h */

#ifndef _H_XMLParser_
#define _H_XMLParser_

#include "string_slice.h"
#include "String.h"

class Document;
class Node;
class Element;
class Attr;
class Text;
class ProcessingInstruction;
class Comment;
class CDATASection;
class XMLNodeFactory;

class XMLParser {
public:
	XMLParser(string_slice textIn);
	Document*             	Parse();
	void                  	ParseInto(Document* documentIn);
	void                  	ParsePCDataInto(Node* node, string_slice elementName);
	Element*              	ParseElement();
	void                  	ParseTextInto(Node* node, char terminator = '<');
	ProcessingInstruction*	ParseProcessingInstruction();
	Comment*              	ParseComment();
	CDATASection*         	ParseCDATASection();
	string_slice          	ParseEndTag();
	Attr*                 	ParseAttribute();
	void                  	SkipWhitespace();
	bool                  	AtWhitespace();
	long                  	ParseHex(string_slice hexStr);
	String             	CharToUTF8(int c);

protected:
	string_slice	text;
	const char* 	p;
	Document*   	document;
};


#endif
