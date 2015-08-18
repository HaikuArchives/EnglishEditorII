/* DisplayElementNode.cpp */

#include "DisplayElementNode.h"
#include "Element.h"
#include "Text.h"
#include "DisplayTextNode.h"
#include "Font.h"



DisplayElementNode::DisplayElementNode(Element* templateElementIn, Element* sourceElementIn)
	: CompositeDisplayNode(NULL),
	  templateElement(templateElementIn), sourceElement(sourceElementIn),
	  font(NULL)
{
}


DisplayElementNode::~DisplayElementNode()
{
	delete font;
}


DisplayType DisplayElementNode::DeclaredDisplayType()
{
	if (GetProperty("display") == "inline")
		return Inline;
	else
		return Block;
}


bool DisplayElementNode::IsDisposable()
{
	return true;
}


String DisplayElementNode::GetProperty(string_slice propertyName)
{
	return templateElement->GetAttribute(propertyName);
}


void DisplayElementNode::Load(DisplayDirector* director)
{
	// load immediate children
	ClearChildren();
	LoadFromContentsTemplate(templateElement, sourceElement, director);
/***
	for (Node* child = templateElement->FirstChild(); child; child = child->NextSibling()) {
		DisplayNode* newDisplayNode = NULL;
		if (child->NodeType() == TEXT_NODE)
			newDisplayNode = new DisplayTextNode(dynamic_cast<Text*>(child));
		else if (child->NodeType() == ELEMENT_NODE) {
			String tagName = child->NodeName();
			if (tagName == "display-element") {
				newDisplayNode =
					new DisplayElementNode(dynamic_cast<Element*>(child), sourceElement);
				}
			else if (tagName == "all-contents")
				LoadFromElement(sourceElement, director);
			}
		if (newDisplayNode)
			AppendChild(newDisplayNode);
		}
***/

	LoadChildren(director);
	CheckIfBlock();

	CompositeDisplayNode::Load(director);
}


bool DisplayElementNode::CanSelectAcross()
{
	return false;
}


Font* DisplayElementNode::CurFont()
{
	if (font == NULL)
		font = GetFont();
	return font;
}


Node* DisplayElementNode::FirstNode()
{
	//*** ???
	return NULL;
}


Node* DisplayElementNode::LastNode()
{
	//*** ???
	return NULL;
}


