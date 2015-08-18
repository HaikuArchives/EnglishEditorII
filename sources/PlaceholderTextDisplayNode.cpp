/* PlaceholderTextDisplayNode.cpp */

#include "PlaceholderTextDisplayNode.h"
#include "Text.h"
#include "DisplayDirector.h"
#include "EditStylesheet.h"
#include "ElementDisplayNode.h"
#include "EmptyPlaceholderSelection.h"
#include "FindSelectionContext.h"
#include "GlyphLoc.h"
#include "InternalException.h"


PlaceholderTextDisplayNode::PlaceholderTextDisplayNode(String placeholderText)
	: TextDisplayNode(new Text(placeholderText))
{
}


PlaceholderTextDisplayNode::~PlaceholderTextDisplayNode()
{
	delete text;
}


bool PlaceholderTextDisplayNode::IsEmpty()
{
	return (text->Length() == 0);
}


String PlaceholderTextDisplayNode::GetProperty(string_slice propertyName)
{
	DisplayDirector* director = GetDisplayDirector();
	if (director == NULL)
		throw InternalException("An ElementDisplayNode couldn't find its Director");
	EditStylesheet* stylesheet = director->GetStylesheet();
	if (stylesheet == NULL)
		throw InternalException("DisplayDirector has no stylesheet.");
	return stylesheet->PropertyForElement(propertyName, "#placeholder-text");
}


Selection* PlaceholderTextDisplayNode::StartSelection()
{
	ElementDisplayNode* elementNode = ParentElementNode();
	if (text->Length() == 0)
		return new EmptyPlaceholderSelection(this);
	else
		return elementNode->GetSelection();
}


Selection* PlaceholderTextDisplayNode::EndSelection()
{
	ElementDisplayNode* elementNode = ParentElementNode();
	if (text->Length() == 0)
		return new EmptyPlaceholderSelection(this);
	else
		return elementNode->GetSelection();
}


void PlaceholderTextDisplayNode::InlineLayout(InlineLayoutContext* layoutContext)
{
	// sometimes UpdateProperties() doesn't get called (eg. after deleting an elements nodes)
	if (font == NULL)
		UpdateProperties();

	TextDisplayNode::InlineLayout(layoutContext);
}


Selection* PlaceholderTextDisplayNode::InlineFindSelection(FindSelectionContext* context)
{
	// see if we should select the element
	GlyphRef firstGlyph = FirstGlyph();
	if (!firstGlyph.IsValid())
		return NULL;
	GlyphRef lastGlyph = LastGlyph();
	BPoint mousePoint(context->mouseX - context->xIndent,
	                      context->mouseY - context->curY);
	if (GlyphLoc(firstGlyph) > mousePoint || GlyphLoc(lastGlyph) < mousePoint)
		return NULL;

	// select the element
	return ParentElementNode()->GetSelection();
}


Destination* PlaceholderTextDisplayNode::InlineFindDestination(FindDestinationContext* context)
{
	return NULL;
}


ElementDisplayNode* PlaceholderTextDisplayNode::ParentElementNode()
{
	for (DisplayNode* node = Parent(); node; node = node->Parent()) {
		ElementDisplayNode* elementNode = dynamic_cast<ElementDisplayNode*>(node);
		if (elementNode)
			return elementNode;
		}
	throw InternalException("PlaceholderTextDisplayNode::ParentElementNode: no parent Element.");
}


