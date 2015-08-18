/* EditStylesheet.h */

#ifndef _H_EditStylesheet_
#define _H_EditStylesheet_

#include "String.h"

class Element;

class EditStylesheet {
public:
	virtual~EditStylesheet();
	virtual String	PropertyForElement(string_slice propertyName, string_slice elementName) = 0;
	virtual Element* 	ContentsTemplateForElement(string_slice elementName) = 0;
};


#endif
