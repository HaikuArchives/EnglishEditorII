/* BetweenGlyphsDestination.cpp */

#include "BetweenGlyphsDestination.h"
#include "TextDisplayNode.h"
#include "Glyph.h"
#include "Text.h"
#include "Element.h"
#include "InlineArrow.h"
#include "BlockableDisplayNode.h"
#include "ElementDisplayNode.h"
#include "Selection.h"
#include "GlyphsSelection.h"
#include "InlineElementSelection.h"
#include "GlyphsMover.h"
#include "Font.h"
#include "InternalException.h"



BetweenGlyphsDestination::BetweenGlyphsDestination(Selection* selectionIn, TextDisplayNode* textNodeIn, Glyph* firstGlyphIn, Glyph* secondGlyphIn)
	: Destination(selectionIn), textNode(textNodeIn),
	  firstGlyph(firstGlyphIn), secondGlyph(secondGlyphIn)
{
	BlockableDisplayNode* leafBlock = textNode->EnclosingLeafBlock();
	if (leafBlock == NULL)
		throw InternalException("SpaceDestination ctor: textNode has no enclosing leaf block.");
	Font* font = textNode->CurFont();

	CoordPoint destPoint(leafBlock->LeftSpace(), leafBlock->Top());
	if (firstGlyph) {
		destPoint.x += firstGlyph->GetX() + firstGlyph->Width(font);
		destPoint.y += firstGlyph->GetY() + font->Ascent();
		}
	else {
		destPoint.x += secondGlyph->GetX();
		destPoint.y += secondGlyph->GetY() + font->Ascent();
		}
	Rectangle sourceBounds = selection->Bounds();
	bool above = (destPoint.y < (sourceBounds.top + sourceBounds.bottom) / 2);
	arrow = new InlineArrow(selection->DragStartPoint(destPoint), destPoint,
	                        above, font->Ascent());
}


BetweenGlyphsDestination::~BetweenGlyphsDestination()
{
	delete arrow;
}


Rectangle BetweenGlyphsDestination::ArrowBounds()
{
	return arrow->Bounds();
}


void BetweenGlyphsDestination::DrawArrow(View* view, CoordPoint origin)
{
	arrow->Draw(view, origin);
}


Action* BetweenGlyphsDestination::GetMoveAction()
{
	return MakeMoveAction(false);
/***
	GlyphsSelection* source = dynamic_cast<GlyphsSelection*>(selection);
	if (source == NULL)
		throw InternalException("BetweenGlyphsDestination::GetMoveAction: Source is not a GlyphsSelection.");
	GlyphsMover mover(source->FirstGlyph(), source->LastGlyph(),
	                  GlyphRef(textNode, firstGlyph), GlyphRef(),
	                  GlyphRef(textNode, secondGlyph),
	                  false);
	return mover.MakeAction();
***/
}


Action* BetweenGlyphsDestination::GetCopyAction()
{
	return MakeMoveAction(true);
/***
	GlyphsSelection* source = dynamic_cast<GlyphsSelection*>(selection);
	if (source == NULL)
		throw InternalException("BetweenGlyphsDestination::GetCopyAction: Source is not a GlyphsSelection.");
	GlyphsMover mover(source->FirstGlyph(), source->LastGlyph(),
	                  GlyphRef(textNode, firstGlyph), GlyphRef(),
	                  GlyphRef(textNode, secondGlyph),
	                  true);
	return mover.MakeAction();
***/
}


Action* BetweenGlyphsDestination::MakeMoveAction(bool copying)
{
	InlineElementSelection* elementSource;
	GlyphsSelection* glyphsSource = dynamic_cast<GlyphsSelection*>(selection);
	if (glyphsSource) {
		GlyphsMover mover(glyphsSource->FirstGlyph(), glyphsSource->LastGlyph(),
		                  GlyphRef(textNode, firstGlyph), GlyphRef(),
	                      GlyphRef(textNode, secondGlyph),
		                  copying);
		return mover.MakeAction();
		}
	else if ((elementSource = dynamic_cast<InlineElementSelection*>(selection)) != NULL) {
		GlyphsMover mover(elementSource->GetDisplayNode(),
		                  GlyphRef(textNode, firstGlyph), GlyphRef(),
	                      GlyphRef(textNode, secondGlyph),
		                  copying);
		return mover.MakeAction();
		}
	else
		throw InternalException("BetweenGlyphsDestination::MakeMoveAction: Invalid selection type.");

	return NULL;	// will never happen, but keep the compiler happy
}


