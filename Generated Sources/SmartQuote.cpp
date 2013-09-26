/* SmartQuote.cpp */

#include "SmartQuote.h"
#include "GlyphDrawContext.h"
#include "TextDisplayNode.h"
#include "BackCharsAction.h"
#include "Text.h"
#include "Font.h"
#include "View.h"



SmartQuote::SmartQuote(bool openingIn)
	: opening(openingIn), width(0)
{
}


int SmartQuote::Width(Font* font)
{
	if (width == 0)
		width = font->WidthOf(DisplayString());
	return width;
}


void SmartQuote::UncacheWidth()
{
	width = 0;
}


void SmartQuote::Draw(GlyphDrawContext* context)
{
	if (!ShouldDraw(context))
		return;

	View* view = context->view;
	view->MovePenTo(x + context->originX, y + context->originY + context->font->Ascent());
	view->DrawString(DisplayString());
}


bool SmartQuote::IsPunctuation()
{
	return true;
}


int SmartQuote::NumCharsInText()
{
	return 1;
}


Action* SmartQuote::DeleteFrontAction(TextDisplayNode* textNode)
{
	return new BackCharsAction(textNode->GetText(), textNode->OffsetOfGlyph(this));
}


Action* SmartQuote::DeleteBackAction(TextDisplayNode* textNode)
{
	return new BackCharsAction(textNode->GetText(), textNode->OffsetOfGlyph(this));
}


Selection* SmartQuote::SelectionIntoFront(TextDisplayNode* textNode)
{
	return textNode->GetSelectionAtOffset(textNode->OffsetOfGlyph(this) + NumCharsInText());
}


Selection* SmartQuote::SelectionIntoBack(TextDisplayNode* textNode)
{
	return textNode->GetSelectionAtOffset(textNode->OffsetOfGlyph(this));
}


bool SmartQuote::ShouldBindBefore()
{
	return !opening;
}


bool SmartQuote::ShouldBindAfter()
{
	return opening;
}


