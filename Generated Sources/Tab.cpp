/* Tab.cpp */

#include "Tab.h"
#include "TextDisplayNode.h"
#include "BackCharsAction.h"



Tab::Tab()
	: width(0)
{
}


void Tab::SetWidth(int newWidth)
{
	width = newWidth;
}


int Tab::Width(Font* font)
{
	return width;
}


void Tab::Draw(GlyphDrawContext* context)
{
	// nothing to draw
}


bool Tab::IsWhitespace()
{
	return true;
}


bool Tab::IsTab()
{
	return true;
}


int Tab::NumCharsInText()
{
	return 1;
}


Action* Tab::DeleteFrontAction(TextDisplayNode* textNode)
{
	return new BackCharsAction(textNode->GetText(), textNode->OffsetOfGlyph(this));
}


Action* Tab::DeleteBackAction(TextDisplayNode* textNode)
{
	return new BackCharsAction(textNode->GetText(), textNode->OffsetOfGlyph(this));
}


Selection* Tab::SelectionIntoFront(TextDisplayNode* textNode)
{
	return textNode->GetSelectionAtOffset(textNode->OffsetOfGlyph(this) + NumCharsInText());
}


Selection* Tab::SelectionIntoBack(TextDisplayNode* textNode)
{
	return textNode->GetSelectionAtOffset(textNode->OffsetOfGlyph(this));
}


bool Tab::ShouldBindBefore()
{
	return false;
}


bool Tab::ShouldBindAfter()
{
	return false;
}


