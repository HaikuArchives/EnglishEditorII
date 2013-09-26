/* DisplayNode.cpp */

#include "DisplayNode.h"
#include "DocumentDisplayNode.h"
#include "EditStylesheet.h"
#include "DisplayDirector.h"
#include "Font.h"
#include "InternalException.h"
#include "DOMStringIter.h"
#include "qstring.h"
#include <memory>	// for auto_ptr



DisplayNode::DisplayNode(DisplayNode* parentIn)
	: parent(parentIn), prevSibling(NULL), nextSibling(NULL),
	  needsLoad(true), needsLayout(true)
{
}


DisplayNode::~DisplayNode()
{
}


void DisplayNode::Load(DisplayDirector* director)
{
	/* default - just clear "needsLoad" */
	needsLoad = false;
	needsLayout = true;
}


DOMString DisplayNode::GetProperty(string_slice propertyName)
{
	return "";
}


DOMString DisplayNode::GetInheritedProperty(string_slice propertyName)
{
	for (DisplayNode* node = this; node; node = node->Parent()) {
		DOMString property = node->GetProperty(propertyName);
		if (property.length() > 0)
			return property;
		}

	return DOMString();
}


int DisplayNode::GetPropertyInPixels(string_slice propertyName, bool inherited)
{
	DOMString propStr = GetProperty(propertyName);
	if (propStr.length() == 0)
		return (inherited ? parent->GetPropertyInPixels(propertyName, true) : 0);
	DOMString suffix = propStr.substr(propStr.length() - 2, 2);
	double multiplier = 1;
	if (suffix == "px" || suffix == "pt")
		multiplier = 1;
	else if (suffix == "in")
		multiplier = 72;
	else if (suffix == "cm")
		multiplier = (2.54 / 72);
	else if (suffix == "mm")
		multiplier = (25.4 / 72);
	else if (suffix == "pc")
		multiplier = 12;
	else if (suffix == "em") {
		DisplayNode* node = (propertyName == "font-size" ? parent : this);
//***		auto_ptr<Font> font(node->GetFont());
		Font* font = node->CurFont();
		multiplier = font->Size();
		}
	else if (suffix == "ex") {
		/***/
		}
	else if (suffix == "sp") {
		DisplayNode* node = (propertyName == "font-size" ? parent : this);
		Font* font = node->CurFont();
		multiplier = font->SpaceWidth();
		}

	// read the number
	double num = 0;
	bool negative = false;
	DOMString numStr = propStr.substr(0, propStr.length() - 2);
	for (DOMStringIter p = numStr.begin(); p != numStr.end(); ++p) {
		char c = *p;
		int digit;
		if (c == '-')
			negative = true;
		else if (c == '.') {
			// read the post-. portion
			double factor = 0.1;
			++p;	// skip '.'
			for (; p != numStr.end(); ++p) {
				digit = (*p - '0');
				if (digit >= 0 && digit <= 9) {
					num += digit * factor;
					factor /= 10;
					}
				}
			break;
			}
		else {
			digit = c - '0';
			if (digit >= 0 && digit <= 9) {
				num *= 10;
				num += digit;
				}
			}
		}
	if (negative)
		num *= -1;

	return (int) (num * multiplier);
}


Selection* DisplayNode::GetSelection()
{
	// default: none (for non-selectable nodes)
	return NULL;
}


Hotspot* DisplayNode::GetHotspot(DisplayDirector* director)
{
	// default: none
	return NULL;
}


DisplayNode* DisplayNode::Parent()
{
	return parent;
}


DisplayNode* DisplayNode::PreviousSibling()
{
	return prevSibling;
}


DisplayNode* DisplayNode::NextSibling()
{
	return nextSibling;
}


bool DisplayNode::DescendsFrom(DisplayNode* otherNode)
{
	for (DisplayNode* node = parent; node; node = node->Parent()) {
		if (node == otherNode)
			return true;
		}
	return false;
}


DisplayNode* DisplayNode::PrevNodeInDoc()
{
	DisplayNode* node = this;
	DisplayNode* prevNode = node->PreviousSibling();
	while (prevNode == NULL) {
		node = node->Parent();
		if (node == NULL)
			break;
		prevNode = node->PreviousSibling();
		}
	return prevNode;
}


DisplayNode* DisplayNode::NextNodeInDoc()
{
	DisplayNode* node = this;
	DisplayNode* nextNode = node->NextSibling();
	while (nextNode == NULL) {
		node = node->Parent();
		if (node == NULL)
			break;
		nextNode = node->NextSibling();
		}
	return nextNode;
}


DisplayDirector* DisplayNode::GetDisplayDirector()
{
	// find the DocumentDisplayNode
	for (DisplayNode* node = this; node; node = node->parent) {
		DocumentDisplayNode* docNode = dynamic_cast<DocumentDisplayNode*>(node);
		if (docNode)
			return docNode->Director();
		}
	return NULL;
}


/* makes */ Font* DisplayNode::GetFont()
{
	// get the family name
	DOMString familyName = GetInheritedProperty("font-family");
	if (familyName.length() == 0)
		throw InternalException("Couldn't find font-family.");

/***
	// turn it into a string_slice
	qstring familyStr;
	for (DOMStringSliceIter s = familyName.beginSlices(); s != familyName.endSlices(); ++s)
		familyStr += *s;
***/

	// get the size
	int size = GetPropertyInPixels("font-size", true);

	// get the style
	int style = 0;
	if (GetInheritedProperty("font-style") == "italic")
		style |= Font::Italic;
	if (GetInheritedProperty("font-weight") == "bold")
		style |= Font::Bold;

	// return the font
	return new Font(familyName, size, style);
}


bool DisplayNode::NeedsLoad()
{
	return needsLoad;
}


bool DisplayNode::NeedsLayout()
{
	return needsLayout;
}


void DisplayNode::SetPrevSibling(DisplayNode* newPrevSibling)
{
	prevSibling = newPrevSibling;
}


void DisplayNode::SetNextSibling(DisplayNode* newNextSibling)
{
	nextSibling = newNextSibling;
}


void DisplayNode::SetParent(DisplayNode* newParent)
{
	parent = newParent;
}


