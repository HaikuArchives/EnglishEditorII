/* ElementDisplayNode.cpp */

#include "ElementDisplayNode.h"
#include "Element.h"
#include "DisplayDirector.h"
#include "EditStylesheet.h"
#include "Text.h"
#include "DisplayElementNode.h"
#include "PlaceholderTextDisplayNode.h"
#include "InlineElementSelection.h"
#include "BlockElementSelection.h"
#include "InlineElementHotspot.h"
#include "BlockElementHotspot.h"
#include "EmptyPlaceholderSelection.h"
#include "DOMUtils.h"
#include "Font.h"
#include "InternalException.h"



ElementDisplayNode::ElementDisplayNode(Element* elementIn)
	: CompositeDisplayNode(NULL), element(elementIn), font(NULL)
{
	element->AddChangeClient(this);

	//*** REMOVE FOR FINAL
	// what's this all about?  The MW debugger craps out if you try to look deep enough
	// into an ElementDisplayNode to see the element name (I think it's that you
	// can't see into an STL list).  Since it's often helpful to see this when
	// debugging, we pull the tagName up here where it's visible and easy to get to
	debugTagName = element->TagName();
	debugTagName.c_str();
}


ElementDisplayNode::~ElementDisplayNode()
{
	element->RemoveChangeClient(this);
	delete font;
}


DisplayType ElementDisplayNode::DeclaredDisplayType()
{
	if (GetProperty("display") == "inline")
		return Inline;
	else
		return Block;
}


bool ElementDisplayNode::IsDisposable()
{
	return false;
}


void ElementDisplayNode::Load(DisplayDirector* director)
{
	// clear current children
	ClearChildren();

	// load immediate children
	Element* contentsTemplate =
		director->GetStylesheet()->ContentsTemplateForElement(element->NodeName());
	if (contentsTemplate)
		LoadFromContentsTemplate(contentsTemplate, element, director);
//***		AppendChild(new DisplayElementNode(contentsTemplate, element));
	else
		LoadFromElement(element, director);

#ifdef MOVED_TO_CompositeDisplayNode
	// if empty, use placeholder text
	if (firstChild == NULL) {
		DOMString placeholderText = GetProperty("placeholder-text");
		if (true /*** !placeholderText.empty() ***/) {
			PlaceholderTextDisplayNode* placeholderNode =
				new PlaceholderTextDisplayNode(placeholderText);
			AppendChildren(placeholderNode, placeholderNode);
			}
		}
#endif

	LoadChildren(director);
	CheckIfBlock();

	CompositeDisplayNode::Load(director);
}


void ElementDisplayNode::UpdateProperties()
{
	// clear the cached font
	delete font;
	font = NULL;

	CompositeDisplayNode::UpdateProperties();
}


DOMString ElementDisplayNode::GetProperty(string_slice propertyName)
{
	DisplayDirector* director = GetDisplayDirector();
	if (director == NULL)
		throw InternalException("An ElementDisplayNode couldn't find its Director");
	EditStylesheet* stylesheet = director->GetStylesheet();
	if (stylesheet == NULL)
		throw InternalException("DisplayDirector has no stylesheet.");
	return Eval(stylesheet->PropertyForElement(propertyName, element->TagName()));
}


Element* ElementDisplayNode::GetElement()
{
	return element;
}


Selection* ElementDisplayNode::GetSelection()
{
	if (GetDisplayType() == Inline) {
		// handle empty placeholder specially
		if (!element->HasChildNodes()) {
			PlaceholderTextDisplayNode* placeholder =
				dynamic_cast<PlaceholderTextDisplayNode*>(firstChild);
			if (useAssertions && placeholder == NULL)
				throw InternalException("ElementDisplayNode::GetSelection: Couldn't get placeholder.");
			if (placeholder->IsEmpty())
				return new EmptyPlaceholderSelection(placeholder);
			}

		return new InlineElementSelection(this);
		}
	else
		return new BlockElementSelection(this);
}


Hotspot* ElementDisplayNode::GetHotspot(DisplayDirector* director)
{
	// first, make sure it's really hot
	EditStylesheet* stylesheet = director->GetStylesheet();
	string_slice tagName = element->TagName();
	DOMString linkProperty = stylesheet->PropertyForElement("link", tagName);
	if (!linkProperty.empty()) {
		if (linkProperty.startsWith("attr(")) {
			DOMString attrName = linkProperty.substr(5, linkProperty.length() - 6);
			if (element->GetAttribute(attrName).empty())
				return NULL;
			}
		}
	else {
		DOMString actionProperty = Eval(stylesheet->PropertyForElement("action", tagName));
		if (actionProperty.empty())
			return NULL;
		if (actionProperty.startsWith("attr(")) {
			DOMString attrName = actionProperty.substr(5, actionProperty.length() - 6);
			if (element->GetAttribute(attrName).empty())
				return NULL;
			}
		}

	// return the hotspot
	if (GetDisplayType() == Inline)
		return new InlineElementHotspot(this);
	else
		return new BlockElementHotspot(this);
}


Font* ElementDisplayNode::CurFont()
{
	if (font == NULL)
		font = GetFont();
	return font;
}


Node* ElementDisplayNode::FirstNode()
{
	return element;
}


Node* ElementDisplayNode::LastNode()
{
	return element;
}


bool ElementDisplayNode::CanSelectAcross()
{
	/*** ??? ***/
	return true;
}


DOMString ElementDisplayNode::GetScriptProperty(DOMString property)
{
	if (property == "tag-name")
		return element->TagName();
	else if (property == "director")
		return GetDisplayDirector()->AsString();
	else if (property == "contents")
		return DOMUtils::NodeContents(element);
	else
		return StyleScriptable::GetScriptProperty(property);
}


DOMString ElementDisplayNode::FunctionCall(DOMString function, DOMString arg, StyleScriptable* target)
{
	if (function == "attr")
		return element->GetAttribute(arg);
	else
		return StyleScriptable::FunctionCall(function, arg, target);
}


