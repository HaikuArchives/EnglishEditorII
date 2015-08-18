/* CompositeStylesheet.cpp */

#include "CompositeStylesheet.h"
#include "XMLFileSource.h"
#include "DOMEditStylesheet.h"
#include "System.h"
#include "InternalException.h"

DocumentSource* CompositeStylesheet::defaultStylesheetSource = NULL;
EditStylesheet* CompositeStylesheet::defaultStylesheet = NULL;


CompositeStylesheet::CompositeStylesheet(string_slice documentType)
	: docStylesheetSource(NULL), docStylesheet(NULL)
{
	InitStatics();

	String filePath = System::AppDirectory();
	filePath += "Stylesheets/";
	filePath += documentType;
	try {
		docStylesheetSource = new XMLFileSource(filePath);
		docStylesheet = new DOMEditStylesheet(docStylesheetSource->GetDocument());
		}
	catch (...) {
		// we don't care about errors
		}
}


CompositeStylesheet::~CompositeStylesheet()
{
	delete docStylesheet;
	delete docStylesheetSource;
}


String CompositeStylesheet::PropertyForElement(string_slice propertyName, string_slice elementName)
{
	String result;
	if (docStylesheet)
		result = docStylesheet->PropertyForElement(propertyName, elementName);
	if (result.empty() && defaultStylesheet)
		result = defaultStylesheet->PropertyForElement(propertyName, elementName);
	return result;
}


Element* CompositeStylesheet::ContentsTemplateForElement(string_slice elementName)
{
	Element* result = NULL;
	if (docStylesheet)
		result = docStylesheet->ContentsTemplateForElement(elementName);
	if (result == NULL && defaultStylesheet)
		result = defaultStylesheet->ContentsTemplateForElement(elementName);
	return result;
}


void CompositeStylesheet::InitStatics()
{
	if (defaultStylesheet == NULL) {
		String filePath = System::AppDirectory();
		filePath += "Stylesheets/#default";
		try {
			defaultStylesheetSource = new XMLFileSource(filePath);
			defaultStylesheet = new DOMEditStylesheet(defaultStylesheetSource->GetDocument());
			}
		catch (...) {
			throw InternalException("Couldn't open the default stylesheet.  Make sure the program was installed correctly.");
			}
		}
}


