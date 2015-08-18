/* SpaceDestination.cpp */

#include "SpaceDestination.h"
#include "TextDisplayNode.h"
#include "Space.h"
#include "Text.h"
#include "InlineArrow.h"
#include "BlockableDisplayNode.h"
#include "Selection.h"
#include "GlyphsSelection.h"
#include "InlineElementSelection.h"
#include "GlyphsMover.h"
#include "MoveCharsAction.h"
#include "Font.h"
#include "InternalException.h"



SpaceDestination::SpaceDestination(Selection* selectionIn, TextDisplayNode* textNodeIn, Space* spaceIn)
	: Destination(selectionIn), textNode(textNodeIn), space(spaceIn)
{
	BlockableDisplayNode* leafBlock = textNode->EnclosingLeafBlock();
	if (leafBlock == NULL)
		throw InternalException("SpaceDestination ctor: textNode has no enclosing leaf block.");
	Font* font = textNode->CurFont();

	BPoint destPoint(leafBlock->LeftSpace(), leafBlock->Top());
	destPoint.x += space->GetX() + space->Width(font) / 2;
	destPoint.y += space->GetY() + font->Ascent();
	BRect sourceBounds = selection->Bounds();
	bool above = (destPoint.y < (sourceBounds.top + sourceBounds.bottom) / 2);
	arrow = new InlineArrow(selection->DragStartPoint(destPoint), destPoint,
	                        above, font->Ascent());
}


SpaceDestination::~SpaceDestination()
{
	delete arrow;
}


BRect SpaceDestination::ArrowBounds()
{
	return arrow->Bounds();
}


void SpaceDestination::DrawArrow(View* view, BPoint origin)
{
	arrow->Draw(view, origin);
}


Action* SpaceDestination::GetMoveAction()
{
	return MakeMoveAction(false);
}


Action* SpaceDestination::GetCopyAction()
{
	return MakeMoveAction(true);
}


Action* SpaceDestination::MakeMoveAction(bool copying)
{
	InlineElementSelection* elementSource;
	GlyphsSelection* glyphsSource = dynamic_cast<GlyphsSelection*>(selection);
	if (glyphsSource) {
		GlyphsMover mover(glyphsSource->FirstGlyph(), glyphsSource->LastGlyph(),
		                  GlyphRef(textNode, textNode->GlyphBefore(space)),
		                  GlyphRef(textNode, space),
		                  GlyphRef(textNode, textNode->GlyphAfter(space)),
		                  copying);
		return mover.MakeAction();
		}
	else if ((elementSource = dynamic_cast<InlineElementSelection*>(selection)) != NULL) {
		GlyphsMover mover(elementSource->GetDisplayNode(),
		                  GlyphRef(textNode, textNode->GlyphBefore(space)),
		                  GlyphRef(textNode, space),
		                  GlyphRef(textNode, textNode->GlyphAfter(space)),
		                  copying);
		return mover.MakeAction();
		}
	else
		throw InternalException("SpaceDestination::MakeMoveAction: Invalid selection type.");

	return NULL;	// will never happen, but keep the compiler happy
}


