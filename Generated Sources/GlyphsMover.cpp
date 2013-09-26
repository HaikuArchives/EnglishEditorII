/* GlyphsMover.cpp */

#include "GlyphsMover.h"
#include "TextDisplayNode.h"
#include "ElementDisplayNode.h"
#include "Text.h"
#include "Element.h"
#include "Glyph.h"
#include "CompositeAction.h"
#include "SplitTextAction.h"
#include "MoveCharsAction.h"
#include "CopyCharsAction.h"
#include "DeleteCharsAction.h"
#include "InsertSpaceAction.h"
#include "MoveNodesAction.h"
#include "CopyNodesAction.h"
#include "SelectGlyphsAction.h"
#include "RestoreSelectionAction.h"
#include "InternalException.h"



GlyphsMover::GlyphsMover(GlyphRef srcFirstGlyph, GlyphRef srcLastGlyph, GlyphRef destPreGlyph, GlyphRef destSpaceGlyph, GlyphRef destPostGlyph, bool copy)
	: copying(copy), takePreSpace(false), takePostSpace(false),
	  addPreSpace(false), addPostSpace(false), addSrcSpace(false),
	  firstMoveNode(NULL), lastMoveNode(NULL)
{
	// get info about the source structure
	DisplayNode* ancestor = srcFirstGlyph.CommonAncestorWith(srcLastGlyph);
	bool srcStartsWithText = (srcFirstGlyph.textNode->Parent() == ancestor);
	bool srcEndsWithText = (srcLastGlyph.textNode->Parent() == ancestor);
	singleSrcText = (srcFirstGlyph.textNode == srcLastGlyph.textNode);
	hasNodes =
		(!srcStartsWithText || !srcEndsWithText ||
		 srcFirstGlyph.textNode->NextSibling() != srcLastGlyph.textNode);
	if (singleSrcText) {
		srcStartsWithText = true;
		srcEndsWithText = true;
		hasNodes = false;
		}

	// find the pre and post space in the src
	//*** someday make this peek outside the Texts when appropriate
	//*** but watch out: you'll need to use GlyphRefs instead of Glyphs
	Glyph* srcPreGlyph = srcFirstGlyph.textNode->GlyphBefore(srcFirstGlyph.glyph);
	Glyph* srcPreSpaceGlyph = srcPreGlyph;
	bool srcBindsBefore = true;
	if (srcPreSpaceGlyph) {
		if (srcPreSpaceGlyph->IsWhitespace())
			srcBindsBefore = false;
		else
			srcPreSpaceGlyph = NULL;
		}
	bool srcBindsAfter = true;
	Glyph* srcPostGlyph = srcLastGlyph.textNode->GlyphAfter(srcLastGlyph.glyph);
	Glyph* srcPostSpaceGlyph = srcPostGlyph;
	if (srcPostSpaceGlyph) {
		if (srcPostSpaceGlyph->IsWhitespace())
			srcBindsAfter = false;
		else
			srcPostSpaceGlyph = NULL;
		}

	// begin setting up the source
	// src
	if (srcPreSpaceGlyph)
		srcPreSpace.Set(GlyphRef(srcFirstGlyph.textNode, srcPreSpaceGlyph));
	unsigned long srcStartOffset;
	if (singleSrcText) {
		srcStartOffset =
			srcFirstGlyph.textNode->OffsetOfGlyph(srcFirstGlyph.glyph);
		srcFirstText.Set(srcFirstGlyph.textNode->GetText(),
		                 srcStartOffset,
		                 srcLastGlyph.textNode->OffsetOfGlyph(srcLastGlyph.glyph) +
		                 	srcLastGlyph.glyph->NumCharsInText() - srcStartOffset);
		}
	else {
		if (srcStartsWithText) {
			srcStartOffset =
				srcFirstGlyph.textNode->OffsetOfGlyph(srcFirstGlyph.glyph);
			srcFirstText.Set(srcFirstGlyph.textNode->GetText(),
			                 srcStartOffset,
			                 srcFirstGlyph.textNode->GetText()->Length() - srcStartOffset);
			}
		if (srcEndsWithText) {
			unsigned long srcLastOffset =
				srcLastGlyph.textNode->OffsetOfGlyph(srcLastGlyph.glyph) +
				srcLastGlyph.glyph->NumCharsInText();
			srcLastText.Set(srcLastGlyph.textNode->GetText(), 0, srcLastOffset);
			}
		}
	if (srcPostSpaceGlyph)
		srcPostSpace.Set(GlyphRef(srcLastGlyph.textNode, srcPostSpaceGlyph));

	// figure out source binding
	if (copying)
		keepSrcPreSpace = keepSrcPostSpace = true;
	else {
		bool needSrcSpace = true;
		if (srcPreGlyph && srcPreGlyph->ShouldBindAfter())
			needSrcSpace = false;
		if (srcPostGlyph && srcPostGlyph->ShouldBindBefore())
			needSrcSpace = false;
		if (needSrcSpace) {
			if (srcPreSpaceGlyph && srcPostSpaceGlyph) {
				// have both; take whichever is less work
				if (takePostSpace) {
					keepSrcPostSpace = false;
					keepSrcPreSpace = true;
					}
				else {
					keepSrcPreSpace = false;
					keepSrcPostSpace = true;
					}
				}
			else if (srcPreSpaceGlyph) {
				keepSrcPreSpace = true;
				keepSrcPostSpace = false;
				}
			else if (srcPostSpaceGlyph) {
				keepSrcPostSpace = true;
				keepSrcPreSpace = false;
				}
			else
				addSrcSpace = true;
			}
		else
			keepSrcPreSpace = keepSrcPostSpace = false;
		}

/***
	// see if we need source space
	bool needSrcSpace = (!copying && srcBindsBefore && srcBindsAfter &&
	                     srcPreGlyph && srcPostGlyph &&
	                     !srcPreGlyph->ShouldBindAfter() &&
	                     !srcPostGlyph->ShouldBindAfter());
***/

	// merge first & last source texts if we can
	if (srcFirstText.IsJustBefore(srcLastText)) {
		srcFirstText.Append(srcLastText);
		srcLastText.Clear();
		}

	// set up the destination
	ConfigureDest(GlyphRef(srcFirstGlyph.textNode, srcPreSpaceGlyph), srcFirstGlyph,
	              srcLastGlyph, GlyphRef(srcLastGlyph.textNode, srcPostSpaceGlyph),
	              destPreGlyph, destSpaceGlyph, destPostGlyph);

	// move nodes
	if (hasNodes) {
		ElementDisplayNode* ancestorDisplayNode = dynamic_cast<ElementDisplayNode*>(ancestor);
		if (ancestorDisplayNode == NULL)
			throw InternalException("BetweenGlyphsDestination::MoveActionWithElements: ancestor isn't an element.");
		Element* ancestorElement = ancestorDisplayNode->GetElement();
		firstMoveNode = srcFirstGlyph.textNode->GetText();
		while (firstMoveNode->ParentNode() != ancestorElement)
			firstMoveNode = firstMoveNode->ParentNode();
		if (srcStartsWithText)
			firstMoveNode = firstMoveNode->NextSibling();
		lastMoveNode = srcLastGlyph.textNode->GetText();
		while (lastMoveNode->ParentNode() != ancestorElement)
			lastMoveNode = lastMoveNode->ParentNode();
		if (srcEndsWithText)
			lastMoveNode = lastMoveNode->PreviousSibling();
		}
}


GlyphsMover::GlyphsMover(ElementDisplayNode* displayNode, GlyphRef destPreGlyph, GlyphRef destSpaceGlyph, GlyphRef destPostGlyph, bool copy)
	: copying(copy), takePreSpace(false), takePostSpace(false),
	  keepSrcPreSpace(true), keepSrcPostSpace(false),
	  addPreSpace(false), addPostSpace(false), addSrcSpace(false),
	  firstMoveNode(NULL), lastMoveNode(NULL)
{
	// get info about the source structure
	singleSrcText = false;
	hasNodes = true;

	//*** someday look for pre- and post-space

	// set up the destination
	ConfigureDest(GlyphRef(), displayNode->FirstSelectableGlyph(),
	              displayNode->LastSelectableGlyph(), GlyphRef(),
	              destPreGlyph, destSpaceGlyph, destPostGlyph);

	// move nodes
	Element* element = displayNode->GetElement();
	firstMoveNode = element;
	lastMoveNode = element;
}


Action* GlyphsMover::MakeAction()
{
	// set up
	CompositeAction* action = new CompositeAction();
	action->AddAction(new RestoreSelectionAction());
	TextSpan selectionStart(destFirstText.text, destFirstText.EndOffset(), 0);
	TextSpan selectionEnd(destLastText.text, destLastText.offset, 0);
	Node* moveBeforeNode = destLastText.text->NextSibling();
	Node* moveDestParentNode = destLastText.text->ParentNode();
	Text* srcText;
	Text* destText;
	unsigned long srcOffset, destOffset, length;

	// first, delete dest space if required
	if (!destDeleteText.IsEmpty()) {
		destText = destDeleteText.text;
		destOffset = destDeleteText.offset;
		length = destDeleteText.length;
		action->AddAction(new DeleteCharsAction(destText, destOffset, length));
		// adjust
		destLastText.AdjustForDelete(destText, destOffset, length);
		selectionEnd.AdjustForDelete(destText, destOffset, length);
		srcPreSpace.AdjustForDelete(destText, destOffset, length);
		srcFirstText.AdjustForDelete(destText, destOffset, length);
		srcLastText.AdjustForDelete(destText, destOffset, length);
		srcPostSpace.AdjustForDelete(destText, destOffset, length);
		}

	// split the destination
	if (!singleSrcText && destFirstText.text == destLastText.text) {
		destText = destFirstText.text;
		unsigned long splitOffset = destFirstText.EndOffset();
		SplitTextAction* splitAction =
			new SplitTextAction(destText, splitOffset);
		action->AddAction(splitAction);
		Text* newText = splitAction->GetNewText();
		moveBeforeNode = newText;
		// adjust
		destLastText.Set(newText, 0, 0);
		selectionEnd.Set(newText, 0, 0);
		if (srcPreSpace.text == destText && srcPreSpace.offset >= splitOffset)
			srcPreSpace.Set(newText, srcPreSpace.offset - splitOffset, srcPreSpace.length);
		if (srcFirstText.text == destText && srcFirstText.offset >= splitOffset)
			srcFirstText.Set(newText, srcFirstText.offset - splitOffset, srcFirstText.length);
		if (srcLastText.text == destText && srcLastText.offset >= splitOffset)
			srcLastText.Set(newText, srcLastText.offset - splitOffset, srcLastText.length);
		if (srcPostSpace.text == destText && srcPostSpace.offset >= splitOffset)
			srcPostSpace.Set(newText, srcPostSpace.offset - splitOffset, srcPostSpace.length);
		}

	// pre-space
	if (takePreSpace) {
		srcText = srcPreSpace.text;
		srcOffset = srcPreSpace.offset;
		destText = destFirstText.text;
		destOffset = destFirstText.EndOffset();
		length = srcPreSpace.length;
		if (keepSrcPreSpace) {
			action->AddAction(new CopyCharsAction(srcText, srcOffset, length,
			                                      destText, destOffset));
			}
		else {
			action->AddAction(new MoveCharsAction(srcText, srcOffset, length,
			                                      destText, destOffset));

			// adjust for the deletion
			destFirstText.AdjustForDelete(srcText, srcOffset, length);
			destLastText.AdjustForDelete(srcText, srcOffset, length);
			selectionStart.AdjustForDelete(srcText, srcOffset, length);
			selectionEnd.AdjustForDelete(srcText, srcOffset, length);
			srcFirstText.AdjustForDelete(srcText, srcOffset, length);
			srcLastText.AdjustForDelete(srcText, srcOffset, length);
			srcPostSpace.AdjustForDelete(srcText, srcOffset, length);
			if (srcText == destText && srcOffset < destOffset)
				destOffset -= length;
			}

		// adjust for the insertion
		destFirstText.length += length;
		selectionStart.offset += length;
		destLastText.AdjustForInsert(destText, destOffset, length);
		selectionEnd.AdjustForInsert(destText, destOffset, length);
		srcFirstText.AdjustForInsert(destText, destOffset, length);
		srcLastText.AdjustForInsert(destText, destOffset, length);
		srcPostSpace.AdjustForInsert(destText, destOffset, length);
		}
	else if (addPreSpace) {
		destText = destFirstText.text;
		destOffset = destFirstText.EndOffset();
		InsertSpaceAction* preSpaceAction = new InsertSpaceAction(destText, destOffset);
		action->AddAction(preSpaceAction);
		length = preSpaceAction->SpaceLength();
		// adjust for insertion
		destFirstText.length += length;
		selectionStart.offset += length;
		destLastText.AdjustForInsert(destText, destOffset, length);
		selectionEnd.AdjustForInsert(destText, destOffset, length);
		srcFirstText.AdjustForInsert(destText, destOffset, length);
		srcLastText.AdjustForInsert(destText, destOffset, length);
		srcPostSpace.AdjustForInsert(destText, destOffset, length);
		}
	if (!keepSrcPreSpace && !takePreSpace && !srcPreSpace.IsEmpty()) {
		// delete the src pre-space
		srcText = srcPreSpace.text;
		srcOffset = srcPreSpace.offset;
		length = srcPreSpace.length;
		action->AddAction(new DeleteCharsAction(srcText, srcOffset, length));
		// adjust for deletion
		destFirstText.AdjustForDelete(srcText, srcOffset, length);
		destLastText.AdjustForDelete(srcText, srcOffset, length);
		selectionStart.AdjustForDelete(srcText, srcOffset, length);
		selectionEnd.AdjustForDelete(srcText, srcOffset, length);
		srcFirstText.AdjustForDelete(srcText, srcOffset, length);
		srcLastText.AdjustForDelete(srcText, srcOffset, length);
		srcPostSpace.AdjustForDelete(srcText, srcOffset, length);
		}

	// first text
	if (!srcFirstText.IsEmpty()) {
		srcText = srcFirstText.text;
		srcOffset = srcFirstText.offset;
		length = srcFirstText.length;
		destText = destFirstText.text;
		destOffset = destFirstText.EndOffset();
		if (copying) {
			action->AddAction(new CopyCharsAction(srcText, srcOffset, length,
			                                      destText, destOffset));
			}
		else {
			action->AddAction(new MoveCharsAction(srcText, srcOffset, length,
			                                      destText, destOffset));
			// adjust for the deletion
			srcFirstText.length = 0;
			selectionStart.AdjustForDelete(srcText, srcOffset, length);
			selectionEnd.AdjustForDelete(srcText, srcOffset, length);
			destLastText.AdjustForDelete(srcText, srcOffset, length);
			srcLastText.AdjustForDelete(srcText, srcOffset, length);
			srcPostSpace.AdjustForDelete(srcText, srcOffset, length);
			if (srcText == destText && srcOffset < destOffset)
				destOffset -= length;
			}

		// adjust for the insertion
		selectionEnd.AdjustForInsert(destText, destOffset, length);
		destLastText.AdjustForInsert(destText, destOffset, length);
		srcFirstText.AdjustForInsert(destText, destOffset, length);
		srcLastText.AdjustForInsert(destText, destOffset, length);
		srcPostSpace.AdjustForInsert(destText, destOffset, length);
		}

	// last text
	if (!srcLastText.IsEmpty()) {
		srcText = srcLastText.text;
		srcOffset = srcLastText.offset;
		length = srcLastText.length;
		destText = destLastText.text;
		destOffset = destLastText.offset;
		if (copying) {
			action->AddAction(new CopyCharsAction(srcText, srcOffset, length,
			                                      destText, destOffset));
			}
		else {
			action->AddAction(new MoveCharsAction(srcText, srcOffset, length,
			                                      destText, destOffset));

			// adjust for delete
			selectionStart.AdjustForDelete(srcText, srcOffset, length);
			selectionEnd.AdjustForDelete(srcText, srcOffset, length);
			srcPostSpace.AdjustForDelete(srcText, srcOffset, length);
			if (srcText == destText && srcOffset < destOffset)
				destOffset -= length;
			}

		// adjust for insertion
		selectionEnd.length += length;
		destLastText.offset += length;
		srcPostSpace.AdjustForInsert(destText, destOffset, length);
		}

	// post space
	if (takePostSpace) {
		srcText = srcPostSpace.text;
		srcOffset = srcPostSpace.offset;
		length = srcPostSpace.length;
		destText = destLastText.text;
		destOffset = destLastText.offset;
		if (keepSrcPostSpace) {
			action->AddAction(new CopyCharsAction(srcText, srcOffset, length,
			                                      destText, destOffset));
			}
		else {
			action->AddAction(new MoveCharsAction(srcText, srcOffset, length,
			                                      destText, destOffset));
			selectionStart.AdjustForDelete(srcText, srcOffset, length);
			selectionEnd.AdjustForDelete(srcText, srcOffset, length);
			}
		}
	else if (addPostSpace) {
		action->AddAction(new InsertSpaceAction(destLastText.text, destLastText.offset));
		}
	if (!keepSrcPostSpace && !takePostSpace && !srcPostSpace.IsEmpty()) {
		srcText = srcPostSpace.text;
		srcOffset = srcPostSpace.offset;
		length = srcPostSpace.length;
		action->AddAction(new DeleteCharsAction(srcText, srcOffset, length));
		selectionStart.AdjustForDelete(srcText, srcOffset, length);
		selectionEnd.AdjustForDelete(srcText, srcOffset, length);
		}

	// source space
	if (addSrcSpace) {
		if (srcLastText.IsValid()) {
			srcText = srcLastText.text;
			srcOffset = srcLastText.offset;
			}
		else if (srcFirstText.IsValid()) {
			srcText = srcFirstText.text;
			srcOffset = srcFirstText.EndOffset();
			}
		else
			throw InternalException("GlyphsMover::MakeAction: Sorry, no adding source space where there's no Text yet.");
		InsertSpaceAction* srcSpaceAction = new InsertSpaceAction(srcText, srcOffset);
		action->AddAction(srcSpaceAction);
		length = srcSpaceAction->SpaceLength();
		// adjust for insertion
		selectionStart.AdjustForInsert(srcText, srcOffset, length);
		selectionEnd.AdjustForInsert(srcText, srcOffset, length);
		}

	// move the nodes
	Node* moveFirstDestNode = NULL;
	Node* moveLastDestNode = NULL;
	if (hasNodes) {
		if (copying) {
			CopyNodesAction* copyNodesAction =
				new CopyNodesAction(firstMoveNode, lastMoveNode,
			                        moveDestParentNode,
			                        moveBeforeNode);
			action->AddAction(copyNodesAction);
			moveFirstDestNode = copyNodesAction->FirstNode();
			moveLastDestNode = copyNodesAction->LastNode();
			}
		else {
			action->AddAction(new MoveNodesAction(firstMoveNode, lastMoveNode,
			                                      moveDestParentNode,
			                                      moveBeforeNode));
			moveFirstDestNode = firstMoveNode;
			moveLastDestNode = lastMoveNode;
			}
		}

	// select
	if (!srcFirstText.IsValid()) {
		// no first text; need to select from the beginning of firstMoveNode
		Text* firstText = FirstTextIn(moveFirstDestNode);
		if (useAssertions && firstText == NULL)
			throw InternalException("GlyphsMover::MakeAction: no first text.");
		selectionStart.Set(firstText, 0, firstText->Length());
		}
	if (selectionEnd.EndOffset() == 0) {
		// this is the beginning of the split node; we need to select to the
		// end of lastMoveNode
		Text* lastText = LastTextIn(moveLastDestNode);
		if (useAssertions && lastText == NULL)
			throw InternalException("GlyphsMover::MakeAction: no end text.");
		selectionEnd.Set(lastText, 0, lastText->Length());
		}
	action->AddAction(new SelectGlyphsAction(selectionStart.text, selectionStart.offset,
	                                         selectionEnd.text,
	                                         selectionEnd.EndOffset() - 1));

	return action;
}


void GlyphsMover::ConfigureDest(GlyphRef srcPreSpaceGlyph, GlyphRef srcFirstGlyph, GlyphRef srcLastGlyph, GlyphRef srcPostSpaceGlyph, GlyphRef destPreGlyph, GlyphRef destSpaceGlyph, GlyphRef destPostGlyph)
{
	// figure out how it should bind
	bool srcBindsBefore = !srcPreSpaceGlyph.IsValid();
	bool bindBefore = srcBindsBefore && srcFirstGlyph.glyph->ShouldBindBefore();
	if (destPreGlyph.IsValid() && destPreGlyph.glyph->ShouldBindAfter())
		bindBefore = true;
	bool srcBindsAfter = !srcPostSpaceGlyph.IsValid();
	bool bindAfter = srcBindsAfter && srcLastGlyph.GetGlyph()->ShouldBindAfter();
	if (destPostGlyph.IsValid() && destPostGlyph.glyph->ShouldBindBefore())
		bindAfter = true;

	// begin setting up the dest
	if (destPreGlyph.IsValid())
		destFirstText.Set(destPreGlyph);
	else
		destFirstText.Set(destPostGlyph.textNode->GetText(), destPostGlyph.StartOffset(), 0);
	if (destPostGlyph.IsValid())
		destLastText.Set(destPostGlyph);
	else
		destLastText.Set(destPreGlyph.textNode->GetText(), destPreGlyph.EndOffset(), 0);

	// now figure out exactly how to do the dest binding
	bool needPreSpace = !bindBefore;
	bool needPostSpace = !bindAfter;
	// may need to delete space that's there
	if (!needPreSpace && !needPostSpace && destSpaceGlyph.IsValid())
		destDeleteText.Set(destSpaceGlyph);
	// take what we need from the source, preferring the srcPostSpaceGlyph
	else if (needPreSpace && needPostSpace) {
		// pre-space
		if (destSpaceGlyph.IsValid())
			destFirstText.Set(destSpaceGlyph);
		else if (srcPreSpaceGlyph.IsValid())
			takePreSpace = true;
		else
			addPreSpace = true;
		// post-space
		if (srcPostSpaceGlyph.IsValid())
			takePostSpace = true;
		else
			addPostSpace = true;
		}
	else if (needPreSpace) {
		if (destSpaceGlyph.IsValid())
			destFirstText.Set(destSpaceGlyph);
		else if (srcPreSpaceGlyph.IsValid())
			takePreSpace = true;
		else
			addPreSpace = true;
		}
	else if (needPostSpace) {
		if (destSpaceGlyph.IsValid())
			destLastText.Set(destSpaceGlyph);
		else if (srcPostSpaceGlyph.IsValid())
			takePostSpace = true;
		else
			addPostSpace = true;
		}

}


Text* GlyphsMover::FirstTextIn(Node* node)
{
	for (Node* childNode = node->FirstChild(); childNode; childNode = childNode->NextSibling()) {
		if (childNode->NodeType() == TEXT_NODE)
			return dynamic_cast<Text*>(childNode);
		Text* firstText = FirstTextIn(childNode);
		if (firstText)
			return firstText;
		}
	return NULL;
}


Text* GlyphsMover::LastTextIn(Node* node)
{
	for (Node* childNode = node->LastChild(); childNode; childNode = childNode->PreviousSibling()) {
		if (childNode->NodeType() == TEXT_NODE)
			return dynamic_cast<Text*>(childNode);
		Text* lastText = LastTextIn(childNode);
		if (lastText)
			return lastText;
		}
	return NULL;
}


