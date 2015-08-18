/* DOMEditStylesheet.h */

#ifndef _H_DOMEditStylesheet_
#define _H_DOMEditStylesheet_

#include "EditStylesheet.h"

class Document;
class DOMEditStyleDict;

class DOMEditStylesheet : public EditStylesheet {
public:
	DOMEditStylesheet(Document* document);
	~DOMEditStylesheet();
	String	PropertyForElement(string_slice propertyName, string_slice elementName);
	Element* 	ContentsTemplateForElement(string_slice elementName);

protected:
	DOMEditStyleDict*	dict;
};


#endif
