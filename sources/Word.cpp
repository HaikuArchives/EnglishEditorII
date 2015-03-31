/* Word.cpp */

#include "Word.h"
#include "GlyphDrawContext.h"
#include "TextDisplayNode.h"
#include "Text.h"
#include "BackCharsAction.h"
#include "View.h"
#include "Font.h"
#include "UTF8Utils.h"
#include "qstring.h"



Word::Word(DOMString wordIn, bool isPunctuationIn)
	: word(wordIn), width(0), isPunctuation(isPunctuationIn)
{
}


int Word::Width(Font* font)
{
	if (width == 0)
		width = font->WidthOf(word);
	return width;
}


void Word::UncacheWidth()
{
	width = 0;
}


void Word::Draw(GlyphDrawContext* context)
{
	// see if we really should draw
	int top = y + context->originY;
	Font* font = context->font;
	if (top < context->startY || top + font->LineHeight() > context->endY)
		return;

	View* view = context->view;
	view->MovePenTo(x + context->originX, top + font->Ascent());
	view->DrawString(word);
}


int Word::NumCharsInText()
{
	return word.length();
}


Action* Word::DeleteFrontAction(TextDisplayNode* textNode)
{
	return new BackCharsAction(textNode->GetText(), textNode->OffsetOfGlyph(this));
}


Action* Word::DeleteBackAction(TextDisplayNode* textNode)
{
	// find the offset, making sure we've got the first byte of the character
	int offset = word.length() - 1;
	const char* wordStr = word.begin();
	while (offset > 0 && !UTF8Utils::IsFirstByte(wordStr[offset]))
		--offset;

	return new BackCharsAction(textNode->GetText(), textNode->OffsetOfGlyph(this) + offset);
}


Selection* Word::SelectionIntoFront(TextDisplayNode* textNode)
{
	// find the start of the second character, accounting for UTF8
	int offset = 1;
	const char* str = word.begin();
	int len = word.length();
	while (offset < len && !UTF8Utils::IsFirstByte(str[offset]))
		++offset;
	return textNode->GetSelectionAtOffset(textNode->OffsetOfGlyph(this) + offset);
}


Selection* Word::SelectionIntoBack(TextDisplayNode* textNode)
{
	// find the start of the last character, accounting for UTF8
	int offset = word.length() - 1;
	const char* str = word.begin();
	while (offset > 0 && !UTF8Utils::IsFirstByte(str[offset]))
		--offset;
	return textNode->GetSelectionAtOffset(textNode->OffsetOfGlyph(this) + offset);
}


bool Word::IsPunctuation()
{
	return isPunctuation;
}


bool Word::ShouldBindBefore()
{
	return (word == "." ||
	        word == "..." ||
	        word == "\'" || word == "\"" ||		//*** these will go away when we do smart quotes
	        word == "," ||
	        word == "?" ||
	        word == "!" ||
	        word == ")" ||
	        word == "]" ||
	        word == ";" ||
	        word == ":");
}


bool Word::ShouldBindAfter()
{
	return (word == "(" || word == "[" ||
	        word == "\'" || word == "\""		//*** these will go away when we do smart quotes
	        );
}


string_slice Word::GetDisplayWord()
{
	return word;
}


int Word::DocToDisplayOffset(int docOffset)
{
	return docOffset;
}


int Word::DisplayToDocOffset(int displayOffset)
{
	return displayOffset;
}


