/* Tweex.cpp */

#include "Tweex.h"
#include "DocumentSource.h"
#include "XMLFileSource.h"
#include "Document.h"
#include "Element.h"
#include "Attr.h"
#include "Text.h"
#include "StyleParser.h"
#include "System.h"
#include "Error.h"

DocumentSource* Tweex::docSource = NULL;
Document* Tweex::tweexDoc = NULL;


string_slice Tweex::GetString(string_slice tweakName)
{
	const string_slice tweexDocName = "Tweex.xml";

	// get the document
	if (docSource == NULL) {
		try {
			DOMString filePath = System::AppDirectory();
			filePath += tweexDocName;
			docSource = new XMLFileSource(filePath);
			tweexDoc = docSource->GetDocument();
			}
		catch (...) {
			DOMString msg = "Couldn't open \"";
			msg += tweexDocName;
			msg += "\".  Make sure the program was installed correctly.";
/***
			Error::ShowError(msg);		// can't do this at init-statics time because
			                      		// the connection to the app-server isn't
			                      		// up yet (presumably)
***/
			return "";
			}
		}

	Element* element = tweexDoc->DocumentElement();
	Attr* attr = element->GetAttributeNode(tweakName);
	if (attr == NULL)
		return string_slice();
	Text* attrText = dynamic_cast<Text*>(attr->FirstChild());
	if (attrText == NULL)
		return string_slice();
	return attrText->Data();
}


int Tweex::GetInt(string_slice tweakName)
{
	return GetString(tweakName).asInt();
}


Color Tweex::GetColor(string_slice tweakName)
{
	string_slice colorStr = GetString(tweakName);
	return StyleParser::ParseColor(colorStr);
}


