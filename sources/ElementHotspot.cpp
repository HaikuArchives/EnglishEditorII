/* ElementHotspot.cpp */

#include "ElementHotspot.h"
#include "ElementDisplayNode.h"
#include "Element.h"
#include "DisplayDirector.h"
#include "EditStylesheet.h"
#include "System.h"



ElementHotspot::ElementHotspot(ElementDisplayNode* displayNodeIn)
	: displayNode(displayNodeIn)
{
}


void ElementHotspot::Clicked(DisplayDirector* director)
{
	Element* element = displayNode->GetElement();
	EditStylesheet* stylesheet = director->GetStylesheet();
	DOMString action = stylesheet->PropertyForElement("action", element->TagName());
	action = displayNode->Eval(action);
	if (!action.empty()) {
		if (!action.empty())
			director->DoDocAction(action);
		return;
		}
	DOMString link = stylesheet->PropertyForElement("link", element->TagName());
	if (!link.empty()) {
		// chase down "attr()"
		if (link.startsWith("attr(")) {
			DOMString attrName = link.substr(5, link.length() - 6);
			link = element->GetAttribute(attrName);
			}
		if (!link.empty()) {
			// for now, we only support opening it in the browser
			System::OpenURL(link);
			}
		}
}


