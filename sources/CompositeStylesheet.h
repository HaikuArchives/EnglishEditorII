/* CompositeStylesheet.h */

#ifndef _H_CompositeStylesheet_
#define _H_CompositeStylesheet_

#include "EditStylesheet.h"
#include "string_slice.h"

class DocumentSource;
class DOMEditStylesheet;

class CompositeStylesheet : public EditStylesheet {
public:
	CompositeStylesheet(string_slice documentType);
	~CompositeStylesheet();
	String  	PropertyForElement(string_slice propertyName, string_slice elementName);
	Element*   	ContentsTemplateForElement(string_slice elementName);
	static void	InitStatics();

protected:
	DocumentSource*       	docStylesheetSource;
	EditStylesheet*       	docStylesheet;
	static DocumentSource*	defaultStylesheetSource;
	static EditStylesheet*	defaultStylesheet;
};


#endif
