/* CharacterSelection.cpp */

#include "CharacterSelection.h"
#include "TextDisplayNode.h"
#include "Word.h"
#include "Space.h"
#include "AddCharsAction.h"
#include "BackCharsAction.h"
#include "CompositeAction.h"
#include "NewElementAction.h"
#include "SplitTextAction.h"
#include "SplitElementAction.h"
#include "RestoreSelectionAction.h"
#include "RestorePointSelectionAction.h"
#include "DisplayDirector.h"
#include "Text.h"
#include "GlyphsSelection.h"
#include "BetweenWordsSelection.h"
#include "BetweenGlyphsSelection.h"
#include "View.h"
#include "Font.h"
#include "Keys.h"
#include "UTF8Utils.h"



CharacterSelection::CharacterSelection(TextDisplayNode* textNodeIn, Word* wordIn, int charIndexIn)
	: PointSelection(textNodeIn), word(wordIn), charIndex(charIndexIn)
{
}


void CharacterSelection::AcceptKey(string_slice key, DisplayDirector* director)
{
	Text* text = textNode->GetText();
	int offset = textNode->OffsetOfGlyph(word);
	if (offset < 0)
		throw InternalException("CharacterSelection::AcceptKey(): couldn't get glyph offset.");
	offset += charIndex;

	if (tagEditor || (key == "\t" && !textNode->IsPreSpace()))
		PointSelection::AcceptKey(key, director);

	else if (key == "\b") {
		// step back one character, accounting for UTF8
		offset -= 1;
		DOMString textData = text->Data();
		const char* str = textData.begin();
		while (offset > 0 && !UTF8Utils::IsFirstByte(str[offset]))
			--offset;
		director->DoAction(new BackCharsAction(text, offset));
		}

	else if (key == Keys::DelKey)
		director->DoAction(new BackCharsAction(text, offset));

	else if (key == "\n") {
		HandleReturn(textNode->OffsetOfGlyph(word) + charIndex, director);
/***
		CompositeAction* action = new CompositeAction();
		action->AddAction(new RestoreSelectionAction());
		Text* text = textNode->GetText();
		unsigned long offset =
			textNode->OffsetOfGlyph(word) + charIndex;
		action->AddAction(new SplitTextAction(text, offset));
		action->AddAction(new SplitElementAction(EnclosingLeafBlockElement(), text));
		director->DoAction(action);
***/
		}

	else if (key == Keys::RightArrowKey) {
		if ((director->WindowView()->CurModifiers() & B_OPTION_KEY) != 0) {
			// go past the word
			SelectPastWord(word, director);
			}
		else {
			// forward one char, accounting for UTF8
			offset += 1;
			DOMString textData = text->Data();
			const char* str = textData.begin();
			int len = textData.length();
			while (offset < len && !UTF8Utils::IsFirstByte(str[offset]))
				++offset;
			director->SetSelection(textNode->GetSelectionAtOffset(offset));
			}
		director->ScrollToSelection();
		}

	else if (key == Keys::LeftArrowKey) {
		if ((director->WindowView()->CurModifiers() & B_OPTION_KEY) != 0) {
			// go back past the word
			SelectBeforeWord(word, director);
			}
		else {
			// back one char, accounting for UTF8
			offset -= 1;
			const char* str = text->Data().begin();
			while (offset > 0 && !UTF8Utils::IsFirstByte(str[offset]))
				--offset;
			director->SetSelection(textNode->GetSelectionAtOffset(offset));
			}
		director->ScrollToSelection();
		}

	else
		director->DoAction(new AddCharsAction(key, text, offset));
}


Selection* CharacterSelection::ExtendTo(CoordPoint point)
{
	/***/
	return this;
}


void CharacterSelection::Promote(DisplayDirector* director)
{
	GlyphRef glyphRef(textNode, word);
	director->SetSelection(GlyphsSelection::SelectionFromTo(glyphRef, glyphRef));
}


Action* CharacterSelection::GetRestoreAction()
{
	unsigned long offset = textNode->OffsetOfGlyph(word) + charIndex;
	return new RestorePointSelectionAction(textNode->GetText(), offset);
}


bool CharacterSelection::CanPaste()
{
	return true;
}


void CharacterSelection::Paste(String pasteText, DisplayDirector* director)
{
	Text* text = textNode->GetText();
	int offset = textNode->OffsetOfGlyph(word);
	if (offset < 0)
		throw InternalException("CharacterSelection::Paste(): couldn't get glyph offset.");
	offset += charIndex;

	director->DoAction(new AddCharsAction(pasteText, text, offset));
}


CoordPoint CharacterSelection::Point()
{
	CoordPoint point = BlockOrigin(textNode);
	Font* font = textNode->CurFont();
	point.x += word->GetX() +
		font->WidthOf(word->GetDisplayWord().substr(0, word->DocToDisplayOffset(charIndex)));
	point.y += word->GetY() + font->Ascent();
	return point;
}


void CharacterSelection::InsertTag(DOMString tagName, DisplayDirector* director)
{
	CompositeAction* action = new CompositeAction();
	action->AddAction(new RestoreSelectionAction());
	Text* text = textNode->GetText();
	SplitTextAction* splitAction =
		new SplitTextAction(text, textNode->OffsetOfGlyph(word) + charIndex);
	action->AddAction(splitAction);
	Text* splitText = splitAction->GetNewText();
	action->AddAction(new NewElementAction(tagName, text->ParentNode(), splitText));
	director->DoAction(action);
}


