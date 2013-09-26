/* BetweenWordsSelection.cpp */

#include "BetweenWordsSelection.h"
#include "TextDisplayNode.h"
#include "Space.h"
#include "DisplayDirector.h"
#include "Text.h"
#include "CompositeAction.h"
#include "AddCharsAction.h"
#include "NewLeafBlockAction.h"
#include "NewElementAction.h"
#include "SplitTextAction.h"
#include "SplitElementAction.h"
#include "RestoreSelectionAction.h"
#include "RestoreBetweenWordsSelectionAction.h"
#include "BetweenGlyphsSelection.h"
#include "Font.h"
#include "View.h"
#include "Keys.h"
#include "InternalException.h"



BetweenWordsSelection::BetweenWordsSelection(TextDisplayNode* textNodeIn, Space* spaceIn)
	: PointSelection(textNodeIn), space(spaceIn)
{
}


void BetweenWordsSelection::AcceptKey(string_slice key, DisplayDirector* director)
{
	if (tagEditor || (key == "\t" && !textNode->IsPreSpace()))
		PointSelection::AcceptKey(key, director);

	else if (key == "\b" || key == Keys::DelKey)
		director->DoAction(space->DeleteBackAction(textNode));

	else if (key == Keys::RightArrowKey)
		SelectForward(director);

	else if (key == Keys::LeftArrowKey)
		SelectBackward(director);

	else if (key == "\n") {
		unsigned long offset =
			textNode->OffsetOfGlyph(space) + space->NumCharsInText();
		HandleReturn(offset, director);
/***
		if (textNode->GlyphAfter(space) == NULL && IsInLastTextOfBlock()) {
			// new paragraph
			Action* newLeafBlockAction =
				new NewLeafBlockAction(EnclosingLeafBlockElement());
			director->DoAction(new RestoreSelectionAction(newLeafBlockAction));
			}
		else {
			// split the paragraph
			CompositeAction* action = new CompositeAction();
			action->AddAction(new RestoreSelectionAction());
			Text* text = textNode->GetText();
			if (textNode->GlyphAfter(space) != NULL) {
				unsigned long offset =
					textNode->OffsetOfGlyph(space) + space->NumCharsInText();
				action->AddAction(new SplitTextAction(text, offset));
				}
			action->AddAction(new SplitElementAction(EnclosingLeafBlockElement(), text));
			director->DoAction(action);
			}
***/
		}

	else {
		// inserting a word--do it at the beginning of the whitespace, so
		// additional characters will go at the end of the word
		int offset = textNode->OffsetOfGlyph(space);
		if (offset < 0)
			throw InternalException("BetweenGlyphsSelection::AcceptKey(): couldn't get glyph offset.");
		qstring insertion = " ";
		insertion += key;

		director->DoAction(new AddCharsAction(insertion, textNode->GetText(), offset));
		}
}


Selection* BetweenWordsSelection::ExtendTo(CoordPoint point)
{
	/***/
	return this;
}


Action* BetweenWordsSelection::GetRestoreAction()
{
	return new RestoreBetweenWordsSelectionAction(textNode->GetText(),
	                                              textNode->OffsetOfGlyph(space));
}


bool BetweenWordsSelection::CanPaste()
{
	return true;
}


void BetweenWordsSelection::Paste(String pasteText, DisplayDirector* director)
{
	// inserting a word--do it at the beginning of the whitespace, so
	// additional characters will go at the end of the word
	//*** someday check for existing whitespace in the pasteText
	int offset = textNode->OffsetOfGlyph(space);
	if (offset < 0)
		throw InternalException("BetweenWordsSelection::Paste(): couldn't get glyph offset.");
	qstring insertion = " ";
	insertion += pasteText;

	director->DoAction(new AddCharsAction(insertion, textNode->GetText(), offset));
}


CoordPoint BetweenWordsSelection::Point()
{
	CoordPoint point = BlockOrigin(textNode);
	Font* font = textNode->CurFont();
	point.x += space->GetX() + space->Width(font) / 2;
	point.y += space->GetY() + font->Ascent();
	return point;
}


void BetweenWordsSelection::InsertTag(DOMString tagName, DisplayDirector* director)
{
	CompositeAction* action = new CompositeAction();
	action->AddAction(new RestoreSelectionAction());
	Text* text = textNode->GetText();
	SplitTextAction* splitAction =
		new SplitTextAction(text, textNode->OffsetOfGlyph(space) + space->NumCharsInText());
	action->AddAction(splitAction);
	Text* splitText = splitAction->GetNewText();
	action->AddAction(new AddCharsAction(" ", splitText, 0));
	action->AddAction(new NewElementAction(tagName, text->ParentNode(), splitText));
	director->DoAction(action);
}


void BetweenWordsSelection::SelectForward(DisplayDirector* director)
{
	if ((director->WindowView()->CurModifiers() & OptionModifier) != 0) {
		// skip past the next word
		Glyph* nextWord = textNode->GlyphAfter(space);
		if (nextWord == NULL)
			SelectPastText(director);
		else {
			SelectPastWord(nextWord, director);
/***
			Glyph* nextGlyph = textNode->GlyphAfter(nextWord);
			Space* nextSpace = dynamic_cast<Space*>(nextGlyph);
			if (nextSpace)
				director->SetSelection(new BetweenWordsSelection(textNode, nextSpace));
			else {
				director->SetSelection(new BetweenGlyphsSelection(textNode,
				                                                  nextWord, nextGlyph,
				                                                  true));
				}
***/
			}
		}

	else {
		director->SetSelection(new BetweenGlyphsSelection(textNode, space,
		                                                  textNode->GlyphAfter(space),
		                                                  false));
		}
	director->ScrollToSelection();
}


void BetweenWordsSelection::SelectBackward(DisplayDirector* director)
{
	if ((director->WindowView()->CurModifiers() & OptionModifier) != 0) {
		// skip to before the previous word
		Glyph* prevWord = textNode->GlyphBefore(space);
		if (prevWord == NULL)
			SelectBeforeText(director);
		else {
			SelectBeforeWord(prevWord, director);
/***
			Glyph* prevGlyph = textNode->GlyphBefore(prevWord);
			Space* prevSpace = dynamic_cast<Space*>(prevGlyph);
			if (prevSpace)
				director->SetSelection(new BetweenWordsSelection(textNode, prevSpace));
			else {
				director->SetSelection(new BetweenGlyphsSelection(textNode,
				                                                  prevGlyph, prevWord,
				                                                  false));
				}
***/
			}
		}

	else {
		director->SetSelection(new BetweenGlyphsSelection(textNode,
		                                                  textNode->GlyphBefore(space),
		                                                  space, true));
		}

	director->ScrollToSelection();
}


