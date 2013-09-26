/* MoveCharsAction.cpp */

#include "MoveCharsAction.h"
#include "Text.h"
#include "DisplayDirector.h"
#include "TextDisplayNode.h"
#include "GlyphsSelection.h"
#include "Glyph.h"
#include "InternalException.h"



MoveCharsAction::MoveCharsAction(Text* srcTextIn, unsigned long srcOffsetIn, unsigned long srcLenIn, Text* destTextIn, unsigned long destOffsetIn, unsigned long destLen, bool addPreSpaceIn, bool addPostSpaceIn, bool selectIn)
	: srcText(srcTextIn), srcOffset(srcOffsetIn), srcLen(srcLenIn),
	  destText(destTextIn), destOffset(destOffsetIn),
	  addPreSpace(addPreSpaceIn), addPostSpace(addPostSpaceIn), select(selectIn)
{
	if (destLen > 0) {
		destDeleted = destText->SubstringData(destOffset, destLen);
		destDeleted.detach();
		}
}


void MoveCharsAction::Do(DisplayDirector* director)
{
	if (select)
		director->SetSelection(NULL);

	unsigned long newSrcOffset = srcOffset;
	unsigned long newDestOffset = destOffset;

	// first delete anything necessary from the dest
	if (!destDeleted.empty()) {
		destText->DeleteData(destOffset, destDeleted.length());
		if (srcText == destText && newSrcOffset > destOffset)
			newSrcOffset -= destOffset;
		}

	// next insert the data
	if (addPreSpace) {
		destText->InsertData(newDestOffset, " ");
		newDestOffset += 1;
		if (srcText == destText && newSrcOffset > newDestOffset)
			newSrcOffset += 1;
		}
	destText->InsertData(newDestOffset, srcText->SubstringData(newSrcOffset, srcLen));
	if (srcText == destText && newSrcOffset >= destOffset)
		newSrcOffset += srcLen;
	if (addPostSpace) {
		destText->InsertData(newDestOffset + srcLen, " ");
		if (srcText == destText && newSrcOffset > newDestOffset)
			newSrcOffset += 1;
		}

	// finally, remove it from the source
	srcText->DeleteData(newSrcOffset, srcLen);
	if (srcText == destText && newDestOffset > newSrcOffset)
		newDestOffset -= srcLen;

	// select
	if (select) {
		TextDisplayNode* displayNode =
			dynamic_cast<TextDisplayNode*>(director->DisplayNodeFor(destText));
		if (displayNode == NULL)
			throw InternalException("MoveCharsAction::Do: No display node for destText.");
		Glyph* firstGlyph =
			displayNode->GlyphAtOffset(newDestOffset - displayNode->TextOffset());
		while (firstGlyph->IsWhitespace()) {
			Glyph* nextGlyph = displayNode->GlyphAfter(firstGlyph);
			if (nextGlyph == NULL)
				break;
			firstGlyph = nextGlyph;
			}
		Glyph* lastGlyph =
			displayNode->GlyphAtOffset(newDestOffset + srcLen - 1 - displayNode->TextOffset());
		while (lastGlyph->IsWhitespace()) {
			Glyph* prevGlyph = displayNode->GlyphBefore(lastGlyph);
			if (prevGlyph == NULL)
				break;
			lastGlyph = prevGlyph;
			}
		director->SetSelection(
			new GlyphsSelection(GlyphRef(displayNode, firstGlyph),
			                    GlyphRef(displayNode, lastGlyph)));
		}

	Action::Do(director);
}


void MoveCharsAction::Undo(DisplayDirector* director)
{
	if (select)
		director->SetSelection(NULL);

	// figure out where things ended up
	unsigned long newSrcOffset = srcOffset;
	unsigned long newDestOffset = destOffset;
	bool sameText = (srcText == destText);
	if (sameText) {
		if (srcOffset > destOffset) {
			if (!destDeleted.empty())
				newSrcOffset -= destOffset;
			if (addPreSpace)
				newSrcOffset += 1;
			if (addPostSpace)
				newSrcOffset += 1;
			newSrcOffset += srcLen;
			}
		else {
			newDestOffset -= srcLen;
			}
		}
	if (addPreSpace)
		newDestOffset += 1;

	// copy back to the source
	srcText->InsertData(newSrcOffset, destText->SubstringData(newDestOffset, srcLen));
	if (sameText && newDestOffset > newSrcOffset)
		newDestOffset += srcLen;

	// remove from the dest
	unsigned long delLen = srcLen;
	if (addPreSpace) {
		newDestOffset -= 1;
		delLen += 1;
		}
	if (addPostSpace)
		delLen += 1;
	destText->DeleteData(newDestOffset, delLen);

	// restore destDeleted
	if (!destDeleted.empty())
		destText->InsertData(destOffset, destDeleted);

	// select
	if (select) {
		TextDisplayNode* displayNode =
			dynamic_cast<TextDisplayNode*>(director->DisplayNodeFor(srcText));
		if (displayNode == NULL)
			throw InternalException("MoveCharsAction::Undo: No display node for srcText.");
		Glyph* firstGlyph =
			displayNode->GlyphAtOffset(srcOffset - displayNode->TextOffset());
		while (firstGlyph->IsWhitespace()) {
			Glyph* nextGlyph = displayNode->GlyphAfter(firstGlyph);
			if (nextGlyph == NULL)
				break;
			firstGlyph = nextGlyph;
			}
		Glyph* lastGlyph =
			displayNode->GlyphAtOffset(srcOffset + srcLen - 1 - displayNode->TextOffset());
		while (lastGlyph->IsWhitespace()) {
			Glyph* prevGlyph = displayNode->GlyphBefore(lastGlyph);
			if (prevGlyph == NULL)
				break;
			lastGlyph = prevGlyph;
			}
		director->SetSelection(
			new GlyphsSelection(GlyphRef(displayNode, firstGlyph),
			                    GlyphRef(displayNode, lastGlyph)));
		}

	Action::Undo(director);
}


