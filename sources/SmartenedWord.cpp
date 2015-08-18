/* SmartenedWord.cpp */

#include "SmartenedWord.h"
#include "GlyphDrawContext.h"
#include "Font.h"
#include "View.h"
#include "UTF8Utils.h"



SmartenedWord::SmartenedWord(String wordIn)
	: Word(wordIn)
{
	// smarten the displayWord
	const char* p = word.begin();
	const char* charsStart = p;
	for (; p != word.end(); ++p) {
		if (*p == '\'') {
			if (p > charsStart)
				displayWord += string_slice(charsStart, p);
			displayWord += "’";
			charsStart = p + 1;
			}
		}
	if (p > charsStart)
		displayWord += string_slice(charsStart, p);
}


int SmartenedWord::Width(Font* font)
{
	if (width == 0)
		width = font->WidthOf(displayWord);
	return width;
}


void SmartenedWord::Draw(GlyphDrawContext* context)
{
	// see if we really should draw
	int top = y + context->originY;
	Font* font = context->font;
	if (top < context->startY || top + font->LineHeight() > context->endY)
		return;

	View* view = context->view;
	view->MovePenTo(x + context->originX, top + font->Ascent());
	view->DrawString(displayWord);
}


string_slice SmartenedWord::GetDisplayWord()
{
	return displayWord;
}


int SmartenedWord::DocToDisplayOffset(int docOffset)
{
	int numChars = UTF8Utils::CharsInString(word.substr(0, docOffset));
	return UTF8Utils::BytesForNChars(displayWord, numChars);
}


int SmartenedWord::DisplayToDocOffset(int displayOffset)
{
	int numChars = UTF8Utils::CharsInString(displayWord.substr(0, displayOffset));
	return UTF8Utils::BytesForNChars(word, numChars);
}


