/* SelectGlyphsAction.cpp */

#include "SelectGlyphsAction.h"
#include "Text.h"
#include "TextDisplayNode.h"
#include "Glyph.h"
#include "GlyphsSelection.h"
#include "DisplayDirector.h"



SelectGlyphsAction::SelectGlyphsAction(Text* firstTextIn, unsigned long firstOffsetIn, Text* lastTextIn, unsigned long lastOffsetIn)
	: firstText(firstTextIn), firstOffset(firstOffsetIn),
	  lastText(lastTextIn), lastOffset(lastOffsetIn)
{
}


void SelectGlyphsAction::Do(DisplayDirector* director)
{
	TextDisplayNode* firstNode =
		dynamic_cast<TextDisplayNode*>(director->DisplayNodeFor(firstText));
	if (firstNode == NULL)
		throw InternalException("SelectGlyphsAction::Do: first TextDisplayNode doesn't exist.");
	Glyph* firstGlyph = firstNode->GlyphAtOffset(firstOffset);
	if (firstGlyph == NULL)
		throw InternalException("SelectGlyphsAction::Do: first glyph doesn't exist.");
	TextDisplayNode* lastNode =
		dynamic_cast<TextDisplayNode*>(director->DisplayNodeFor(lastText));
	if (lastNode == NULL)
		throw InternalException("SelectGlyphsAction::Do: last TextDisplayNode doesn't exist.");
	Glyph* lastGlyph = lastNode->GlyphAtOffset(lastOffset);
	if (lastGlyph == NULL)
		throw InternalException("SelectGlyphsAction::Do: last glyph doesn't exist.");
	director->SetSelection(new GlyphsSelection(GlyphRef(firstNode, firstGlyph), GlyphRef(lastNode, lastGlyph)));
}


void SelectGlyphsAction::Undo(DisplayDirector* director)
{
	// clear the selection for safety
	director->SetSelection(NULL);

	Action::Undo(director);
}


