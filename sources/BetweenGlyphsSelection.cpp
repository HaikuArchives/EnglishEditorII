/* BetweenGlyphsSelection.cpp */

#include "BetweenGlyphsSelection.h"
#include "TextDisplayNode.h"
#include "BlockableDisplayNode.h"
#include "Text.h"
#include "Element.h"
#include "Glyph.h"
#include "Space.h"
#include "DisplayDirector.h"
#include "CompositeAction.h"
#include "AddCharsAction.h"
#include "SplitTextAction.h"
#include "NewElementAction.h"
#include "MergeElementsAction.h"
#include "SelectPointAction.h"
#include "RestoreSelectionAction.h"
#include "RestorePointSelectionAction.h"
#include "BetweenWordsSelection.h"
#include "BetweenBlocksSelection.h"
#include "GlyphsSelection.h"
#include "Font.h"
#include "View.h"
#include "Keys.h"
#include "UTF8Utils.h"
#include "InternalException.h"



BetweenGlyphsSelection::BetweenGlyphsSelection(TextDisplayNode* textNodeIn, Glyph* firstGlyphIn, Glyph* secondGlyphIn, bool promoteBeforeIn)
	: PointSelection(textNodeIn),
	  firstGlyph(firstGlyphIn), secondGlyph(secondGlyphIn),
	  promoteBefore(promoteBeforeIn)
{
	// double-check validity of "promoteBefore"
	if (promoteBefore) {
		if (firstGlyph == NULL)
			promoteBefore = false;
		}
	else {
		if (secondGlyph == NULL)
			promoteBefore = true;
		}
}


void BetweenGlyphsSelection::AcceptKey(string_slice key, DisplayDirector* director)
{
	if (tagEditor || (key == "\t" && !textNode->IsPreSpace()))
		PointSelection::AcceptKey(key, director);
	else if (key == "\b") {
		if (firstGlyph)
			director->DoAction(firstGlyph->DeleteBackAction(textNode));
		else if (IsInFirstTextOfBlock()) {
			// merge with the previous node
			MergeElementsAction* mergeAction =
				new MergeElementsAction(EnclosingLeafBlockElement());
			if (mergeAction->IsValid()) {
				CompositeAction* action = new CompositeAction();
				action->AddAction(new RestoreSelectionAction());
				action->AddAction(mergeAction);
				action->AddAction(new SelectPointAction(textNode->GetText(), 0));
				director->DoAction(action);
				}
			else
				delete mergeAction;
			}
		}
	else if (key == Keys::DelKey) {
		if (secondGlyph)
			director->DoAction(secondGlyph->DeleteFrontAction(textNode));
		}
	else if (key == Keys::RightArrowKey)
		SelectForward(director);
	else if (key == Keys::LeftArrowKey)
		SelectBackward(director);
	else if (key == "\n") {
		unsigned long offset =
			(secondGlyph ? textNode->OffsetOfGlyph(secondGlyph) :
			               textNode->GetText()->Length());
		HandleReturn(offset, director);
/***
		if (secondGlyph == NULL && IsInLastTextOfBlock()) {
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
			if (secondGlyph) {
				unsigned long offset = textNode->OffsetOfGlyph(secondGlyph);
				action->AddAction(new SplitTextAction(text, offset));
				}
			action->AddAction(new SplitElementAction(EnclosingLeafBlockElement(), text));
			director->DoAction(action);
			}
***/
		}
	else {
		int offset;
		if (secondGlyph)
			offset = textNode->OffsetOfGlyph(secondGlyph);
		else {
			// we're at the end of the text, so the offset is its length
			offset = textNode->GetText()->Length();
			}
		if (offset < 0)
			throw InternalException("BetweenGlyphsSelection::AcceptKey(): couldn't get glyph offset.");
		director->DoAction(new AddCharsAction(key, textNode->GetText(), offset));
		}
}


Selection* BetweenGlyphsSelection::ExtendTo(BPoint point)
{
	/***/
	return this;
}


Action* BetweenGlyphsSelection::GetRestoreAction()
{
	unsigned long offset = 0;
	if (secondGlyph)
		offset = textNode->OffsetOfGlyph(secondGlyph);
	else if (firstGlyph)
		offset = textNode->OffsetOfGlyph(firstGlyph) + firstGlyph->NumCharsInText();
	return new RestorePointSelectionAction(textNode->GetText(), offset);
}


void BetweenGlyphsSelection::Promote(DisplayDirector* director)
{
	Glyph* selectedGlyph;
	if (promoteBefore && firstGlyph)
		selectedGlyph = firstGlyph;
	else if (secondGlyph)
		selectedGlyph = secondGlyph;
	else
		return;

	GlyphRef glyphRef(textNode, selectedGlyph);
	director->SetSelection(GlyphsSelection::SelectionFromTo(glyphRef, glyphRef));
}


bool BetweenGlyphsSelection::CanPaste()
{
	return true;
}


void BetweenGlyphsSelection::Paste(String pasteText, DisplayDirector* director)
{
	int offset;
	if (secondGlyph)
		offset = textNode->OffsetOfGlyph(secondGlyph);
	else {
		// we're at the end of the text, so the offset is its length
		offset = textNode->GetText()->Length();
		}
	if (offset < 0)
		throw InternalException("BetweenGlyphsSelection::Paste(): couldn't get glyph offset.");

	director->DoAction(new AddCharsAction(pasteText, textNode->GetText(), offset));
}


BPoint BetweenGlyphsSelection::Point()
{
	BPoint point = BlockOrigin(textNode);
	Font* font = textNode->CurFont();
	if (firstGlyph && !(firstGlyph->IsLineBreak() && secondGlyph)) {
		// prefer the end of the firstGlyph, in case it's a space at the
		// end of a line
		point.x += firstGlyph->GetX() + firstGlyph->Width(font);
		point.y += firstGlyph->GetY() + font->Ascent();
		}
	else if (secondGlyph) {
		point.x += secondGlyph->GetX();
		point.y += secondGlyph->GetY() + font->Ascent();
		}
	else {
		point.y += font->Ascent();
		// find out where the previous glyph ended (if there is one)
		DisplayNode* curNode = textNode;
		while (true) {
			// find the next (previous) node to check
			bool wentUp = false;
			DisplayNode* prevDisplayNode = curNode->PreviousSibling();
			if (prevDisplayNode == NULL) {
				prevDisplayNode = curNode->Parent();
				wentUp = true;
				}
			InlineableDisplayNode* prevNode =
				dynamic_cast<InlineableDisplayNode*>(prevDisplayNode);
			if (prevNode == NULL || prevNode->GetDisplayType() == Block)
				break;	// we went too far up
			if (wentUp) {
				// we don't actually want to check this node, go to its prev sibling
				curNode = prevDisplayNode;
				continue;
				}

			// see if it has any glyphs
			GlyphRef prevGlyph = prevNode->LastGlyph();
			if (prevGlyph.IsValid()) {
				point.x += prevGlyph.glyph->GetX() +
				           prevGlyph.glyph->Width(prevGlyph.textNode->CurFont());
				point.y += prevGlyph.glyph->GetY();
				break;
				}

			// go around again
			curNode = prevDisplayNode;
			}
		}
	return point;
}


void BetweenGlyphsSelection::InsertTag(String tagName, DisplayDirector* director)
{
	CompositeAction* action = new CompositeAction();
	action->AddAction(new RestoreSelectionAction());
	Text* text = textNode->GetText();
	Text* beforeText;
	if (firstGlyph) {
		int offset = textNode->OffsetOfGlyph(firstGlyph) + firstGlyph->NumCharsInText();
		SplitTextAction* splitAction = new SplitTextAction(text, offset);
		action->AddAction(splitAction);
		beforeText = splitAction->GetNewText();
		}
	else
		beforeText = text;
	action->AddAction(new NewElementAction(tagName, text->ParentNode(), beforeText));
	director->DoAction(action);
}


bool BetweenGlyphsSelection::CaretOutLeft()
{
	return (firstGlyph != NULL || secondGlyph == NULL);
}


bool BetweenGlyphsSelection::CaretOutRight()
{
	return (secondGlyph != NULL || firstGlyph == NULL);
}


void BetweenGlyphsSelection::SelectForward(DisplayDirector* director)
{
	// if the option key is down, select after the secondGlyph
	if ((director->WindowView()->CurModifiers() & B_OPTION_KEY) != 0) {
		if (secondGlyph == NULL)
			SelectPastText(director);
		else
			SelectPastWord(secondGlyph, director);
		}

	// select one character forward
	else if (secondGlyph) {
		director->SetSelection(secondGlyph->SelectionIntoFront(textNode));
		}
	else {
		// at end of the textNode; find the next point selection in the document
		SelectPastText(director);
		}

	director->ScrollToSelection();
}


void BetweenGlyphsSelection::SelectBackward(DisplayDirector* director)
{
	// if the option key is down, select before the firstGlyph
	if ((director->WindowView()->CurModifiers() & B_OPTION_KEY) != 0) {
		if (firstGlyph == NULL)
			SelectBeforeText(director);
		else
			SelectBeforeWord(firstGlyph, director);
		}

	// select one character backward
	else if (firstGlyph) {
		director->SetSelection(firstGlyph->SelectionIntoBack(textNode));
		}
	else {
		// at beginning of the textNode; find the previous point selection in the document
		SelectBeforeText(director);
		}

	director->ScrollToSelection();
}


