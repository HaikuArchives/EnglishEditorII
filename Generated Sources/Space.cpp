/* Space.cpp */

#include "Space.h"
#include "TextDisplayNode.h"
#include "Text.h"
#include "BackCharsAction.h"
#include "BetweenWordsSelection.h"
#include "Font.h"



Space::Space(string_slice text)
	: numCharsInText(text.length())
{
}


int Space::Width(Font* font)
{
	return font->SpaceWidth();
}


void Space::Draw(GlyphDrawContext* context)
{
	// don't have to do anything...
}


bool Space::IsWhitespace()
{
	return true;
}


int Space::NumCharsInText()
{
	return numCharsInText;
}


Action* Space::DeleteFrontAction(TextDisplayNode* textNode)
{
	return new BackCharsAction(textNode->GetText(), textNode->OffsetOfGlyph(this), numCharsInText);
}


Action* Space::DeleteBackAction(TextDisplayNode* textNode)
{
	return new BackCharsAction(textNode->GetText(), textNode->OffsetOfGlyph(this), numCharsInText);
}


Selection* Space::SelectionIntoFront(TextDisplayNode* textNode)
{
	return new BetweenWordsSelection(textNode, this);
}


Selection* Space::SelectionIntoBack(TextDisplayNode* textNode)
{
	return new BetweenWordsSelection(textNode, this);
}


bool Space::ShouldBindBefore()
{
	// not really meaningful
	return false;
}


bool Space::ShouldBindAfter()
{
	// not really meaningful
	return false;
}


