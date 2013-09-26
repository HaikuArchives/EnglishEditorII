/* SmartMDash.cpp */

#include "SmartMDash.h"
#include "GlyphDrawContext.h"
#include "TextDisplayNode.h"
#include "Text.h"
#include "BackCharsAction.h"
#include "View.h"
#include "Font.h"

const string_slice SmartMDash::mDash = "—";


SmartMDash::SmartMDash()
	: width(0)
{
}


int SmartMDash::Width(Font* font)
{
	if (width == 0)
		width = font->WidthOf(mDash);
	return width;
}


void SmartMDash::UncacheWidth()
{
	width = 0;
}


void SmartMDash::Draw(GlyphDrawContext* context)
{
	// see if we really should draw
	int top = y + context->originY;
	Font* font = context->font;
	if (top < context->startY || top + font->LineHeight() > context->endY)
		return;

	View* view = context->view;
	view->MovePenTo(x + context->originX, top + font->Ascent());
	view->DrawString(mDash);
}


int SmartMDash::NumCharsInText()
{
	// always came from "--"
	return 2;
}


Action* SmartMDash::DeleteFrontAction(TextDisplayNode* textNode)
{
	return new BackCharsAction(textNode->GetText(), textNode->OffsetOfGlyph(this), 2);
}


Action* SmartMDash::DeleteBackAction(TextDisplayNode* textNode)
{
	return new BackCharsAction(textNode->GetText(), textNode->OffsetOfGlyph(this), 2);
}


Selection* SmartMDash::SelectionIntoFront(TextDisplayNode* textNode)
{
	return textNode->GetSelectionAtOffset(textNode->OffsetOfGlyph(this) + NumCharsInText());
}


Selection* SmartMDash::SelectionIntoBack(TextDisplayNode* textNode)
{
	return textNode->GetSelectionAtOffset(textNode->OffsetOfGlyph(this));
}


bool SmartMDash::IsPunctuation()
{
	return true;
}


bool SmartMDash::ShouldBindBefore()
{
	return true;
}


bool SmartMDash::ShouldBindAfter()
{
	return true;
}


