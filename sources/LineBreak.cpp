/* LineBreak.cpp */

#include "LineBreak.h"
#include "TextDisplayNode.h"
#include "BackCharsAction.h"
#include "BetweenGlyphsSelection.h"



LineBreak::LineBreak(String text)
	: numCharsInText(text.length())
{
}


int LineBreak::Width(Font* font)
{
	return 0;
}


void LineBreak::Draw(GlyphDrawContext* context)
{
	// nothing to do
}


bool LineBreak::IsWhitespace()
{
	return true;
}


bool LineBreak::IsLineBreak()
{
	return true;
}


int LineBreak::NumCharsInText()
{
	return numCharsInText;
}


Action* LineBreak::DeleteFrontAction(TextDisplayNode* textNode)
{
	return new BackCharsAction(textNode->GetText(), textNode->OffsetOfGlyph(this), numCharsInText);
}


Action* LineBreak::DeleteBackAction(TextDisplayNode* textNode)
{
	return new BackCharsAction(textNode->GetText(), textNode->OffsetOfGlyph(this), numCharsInText);
}


Selection* LineBreak::SelectionIntoFront(TextDisplayNode* textNode)
{
	return new BetweenGlyphsSelection(textNode, this, textNode->GlyphAfter(this), false);
}


Selection* LineBreak::SelectionIntoBack(TextDisplayNode* textNode)
{
	return new BetweenGlyphsSelection(textNode, textNode->GlyphBefore(this), this, true);
}


bool LineBreak::ShouldBindBefore()
{
	// not really meaningful
	return false;
}


bool LineBreak::ShouldBindAfter()
{
	// not really meaningful
	return false;
}


