/* RestoreGlyphsSelectionAction.cpp */

#include "RestoreGlyphsSelectionAction.h"
#include "GlyphsSelection.h"
#include "DisplayDirector.h"
#include "TextDisplayNode.h"
#include "Text.h"
#include "InternalException.h"



RestoreGlyphsSelectionAction::RestoreGlyphsSelectionAction()
	: firstText(NULL), firstOffset(0), lastText(NULL), lastOffset(0)
{
}


void RestoreGlyphsSelectionAction::Do(DisplayDirector* director)
{
	GlyphsSelection* selection = dynamic_cast<GlyphsSelection*>(director->GetSelection());
	if (selection == NULL)
		throw InternalException("RestoreGlyphsSelectionAction::Do: no GlyphsSelection.");

	// get the info
	GlyphRef firstGlyph = selection->FirstGlyph();
	firstText = firstGlyph.GetTextNode()->GetText();
	firstOffset = firstGlyph.GetTextNode()->OffsetOfGlyph(firstGlyph.GetGlyph());
	GlyphRef lastGlyph = selection->LastGlyph();
	lastText = lastGlyph.GetTextNode()->GetText();
	lastOffset = lastGlyph.GetTextNode()->OffsetOfGlyph(lastGlyph.GetGlyph());

	// for safety of further actions, clear the selection
	director->SetSelection(NULL);

	Action::Do(director);
}


void RestoreGlyphsSelectionAction::Undo(DisplayDirector* director)
{
	// sanity clause
	if (firstText == NULL || lastText == NULL)
		return;

	// restore the selection
	TextDisplayNode* firstNode =
		dynamic_cast<TextDisplayNode*>(director->DisplayNodeFor(firstText));
	if (firstNode == NULL)
		throw InternalException("RestoreGlyphsSelectionAction::Undo: first TextDisplayNode doesn't exist.");
	Glyph* firstGlyph = firstNode->GlyphAtOffset(firstOffset);
	if (firstGlyph == NULL)
		throw InternalException("RestoreGlyphsSelectionAction::Undo: first glyph doesn't exist.");
	TextDisplayNode* lastNode =
		dynamic_cast<TextDisplayNode*>(director->DisplayNodeFor(lastText));
	if (lastNode == NULL)
		throw InternalException("RestoreGlyphsSelectionAction::Undo: last TextDisplayNode doesn't exist.");
	Glyph* lastGlyph = lastNode->GlyphAtOffset(lastOffset);
	if (lastGlyph == NULL)
		throw InternalException("RestoreGlyphsSelectionAction::Undo: last glyph doesn't exist.");
	director->SetSelection(new GlyphsSelection(GlyphRef(firstNode, firstGlyph), GlyphRef(lastNode, lastGlyph)));

	Action::Undo(director);
}


