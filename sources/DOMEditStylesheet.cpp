/* DOMEditStylesheet.cpp */

#include "DOMEditStylesheet.h"
#include "Element.h"
#include "Attr.h"
#include "Text.h"
#include "Document.h"
#include "NodeList.h"
#include <map>

class DOMEditStyleDict : public map<DOMString, Element*> {};



DOMEditStylesheet::DOMEditStylesheet(Document* document)
{
	dict = new DOMEditStyleDict();

	// read the elements into the dict
	Element* docElement = document->DocumentElement();
	NodeList* elements = docElement->GetElementsByTagName("element");
	for (int i = elements->Length() - 1; i >= 0; --i) {
		Element* element = dynamic_cast<Element*>(elements->Item(i));
		if (element == NULL)
			continue;

		// get the value of the "name" attribute
		//** NOTE: we assume that this is a single Text--if it's not, only the first
		//** Text will be recognized
		Attr* nameAttr = element->GetAttributeNode("name");
		if (nameAttr == NULL)
			continue;
		Text* nameText = dynamic_cast<Text*>(nameAttr->FirstChild());
		if (nameText == NULL)
			continue;

		// add to the dict
		(*dict)[nameText->Data()] = element;
		}
}


DOMEditStylesheet::~DOMEditStylesheet()
{
	delete dict;
}


DOMString DOMEditStylesheet::PropertyForElement(string_slice propertyName, string_slice elementName)
{
	DOMEditStyleDict::const_iterator e = dict->find(elementName);
	if (e == dict->end())
		return DOMString();
	Element* element = e->second;
	return element->GetAttribute(propertyName);
}


Element* DOMEditStylesheet::ContentsTemplateForElement(string_slice elementName)
{
	DOMEditStyleDict::const_iterator e = dict->find(elementName);
	if (e == dict->end())
		return NULL;
	Element* element = e->second;
	return element->GetElementByTagName("display-contents");
}


