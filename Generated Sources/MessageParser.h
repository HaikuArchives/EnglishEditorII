/* MessageParser.h */

#ifndef _H_MessageParser_
#define _H_MessageParser_

#include "string_slice.h"

class Document;
class Element;
class LinesList;

class MessageParser {
public:
	MessageParser(string_slice textIn, Document* documentIn);
	void        	Parse();
	inline bool 	AtEOF();
	string_slice	GetLine();
	bool        	IsBlank(string_slice line);
	void        	ParseHeader(string_slice line, string_slice* headerNameOut,
                 string_slice* headerTextOut);
	void        	ParseQuote(string_slice line, string_slice* quoter, string_slice* lineBody);
	bool        	IsAttribution(string_slice line);
	Element*    	StripAttribution(LinesList* lines);
	void        	FillBlock(Element* element, LinesList* lines);
	void        	HandleParas(LinesList* lines, Element* receptacle);
	Element*    	BuildParagraph(LinesList* lines);
	Element*    	BuildParagraph(string_slice text);
	void        	FinishQuote(Element* quote, Element* receptacle);
	bool        	IsEmailAddress(string_slice word);

protected:
	string_slice	text;
	Document*   	document;
	const char* 	p;
};

inline bool MessageParser::AtEOF()
{
	return (p >= text.end());
}




#endif
