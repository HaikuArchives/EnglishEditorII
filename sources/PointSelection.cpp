/* PointSelection.cpp */

#include "PointSelection.h"
#include "TextDisplayNode.h"
#include "CompositeDisplayNode.h"
#include "ElementDisplayNode.h"
#include "Space.h"
#include "Text.h"
#include "Element.h"
#include "TagEditor.h"
#include "WindowDirector.h"
#include "EditStylesheet.h"
#include "BetweenWordsSelection.h"
#include "BetweenGlyphsSelection.h"
#include "BetweenBlocksSelection.h"
#include "CompositeAction.h"
#include "NewLeafBlockAction.h"
#include "SplitTextAction.h"
#include "SplitElementAction.h"
#include "AddCharsAction.h"
#include "RestoreSelectionAction.h"
#include "Font.h"
#include "View.h"
#include "InternalException.h"

const int PointSelection::caretWidth = 4;
const int PointSelection::caretHeight = 6;
const int PointSelection::tagXOffset = -7;
const int PointSelection::tagYOffset = 8;


PointSelection::PointSelection(TextDisplayNode* textNodeIn)
	: textNode(textNodeIn)
{
	const Font* font = textNode->CurFont();
	ascent = font->Ascent();
}


PointSelection::~PointSelection()
{
}


bool PointSelection::CaretOutLeft()
{
	return true;
}


bool PointSelection::CaretOutRight()
{
	return true;
}


void PointSelection::Draw(View* view, BPoint origin)
{
	view->PushState();
	view->SetPenSize(2);
	view->SetHighColor(selectionColor);

	BPoint point = Point() + origin;
	int xPoint = (int) point.x;
	if (CaretOutLeft())
		xPoint -= caretWidth;
	view->StrokeLine(point, BPoint(xPoint, point.y + caretHeight + 1));
	xPoint = (int) point.x;
	if (CaretOutRight())
		xPoint += caretWidth;
	view->StrokeLine(point, BPoint(xPoint, point.y + caretHeight));

	if (tagEditor) {
		BRect tagBounds = tagEditor->Bounds();
		view->SetPenSize(1);
		view->StrokeLine(point, BPoint(point.x, tagBounds.bottom + origin.y));
		tagEditor->Draw(view->Bounds());
		}

	view->PopState();
}


void PointSelection::AcceptKey(string_slice key, DisplayDirector* director)
{
	TagSelection::AcceptKey(key, director);
}


BRect PointSelection::Bounds()
{
	BPoint point = Point();
	BRect bounds(point.x - caretWidth - 1, point.y - ascent,
	                 point.x + caretWidth + 1, point.y + caretHeight + 1);
	if (tagEditor)
		bounds = bounds | tagEditor->Bounds();
	return bounds;
}


bool PointSelection::IsVisible(BRect pageRect)
{
	BPoint point = Point();
	pageRect.top += ascent;		// the line won't show if it can't show the top
	                       		// of the characters, so don't show this either
	return pageRect.Contains(point);
}


bool PointSelection::NeedsClip()
{
	return false;
}


String PointSelection::TagName()
{
	return "";
}


BRect PointSelection::TagSpecRect()
{
	BPoint point = Point();
	int tagX = (int) point.x + tagXOffset;
	return BRect(tagX, 0, tagX, point.y - ascent + tagYOffset);
}


void PointSelection::TagNameChanged(String newTagName, DisplayDirector* director)
{
	InsertTag(newTagName, director);
}


void PointSelection::TagEditDone(DisplayDirector* director)
{
	HideTag(director);
}


void PointSelection::SelectPastText(DisplayDirector* director)
{
	DisplayNode* node = textNode;
	while (true) {
		// find the next node to check
		node = node->NextNodeInDoc();
		if (node == NULL)
			break;

		// if it's a block, use a BetweenBlocksSelection
		if (node->GetDisplayType() == Block) {
			BlockableDisplayNode* nodeBlock = dynamic_cast<BlockableDisplayNode*>(node);
			BlockableDisplayNode* parentBlock =
				dynamic_cast<BlockableDisplayNode*>(node->Parent());
			director->SetSelection(new BetweenBlocksSelection(parentBlock, nodeBlock));
			break;
			}

		// see if it has a selection
		Selection* startSelection = node->StartSelection();
		if (startSelection) {
			director->SetSelection(startSelection);
			break;
			}
		}
}


void PointSelection::SelectBeforeText(DisplayDirector* director)
{
	DisplayNode* node = textNode;
	while (true) {
		// find the next node to check
		node = node->PrevNodeInDoc();
		if (node == NULL)
			break;

		// if it's a block, use a BetweenBlocksSelection
		if (node->GetDisplayType() == Block) {
			BlockableDisplayNode* nextBlock =
				dynamic_cast<BlockableDisplayNode*>(node->NextSibling());
			BlockableDisplayNode* parentBlock =
				dynamic_cast<BlockableDisplayNode*>(node->Parent());
			director->SetSelection(new BetweenBlocksSelection(parentBlock, nextBlock));
			break;
			}

		// see if it has a selection
		Selection* endSelection = node->EndSelection();
		if (endSelection) {
			director->SetSelection(endSelection);
			break;
			}
		}
}


void PointSelection::SelectPastWord(Glyph* word, DisplayDirector* director)
{
	Glyph* nextGlyph = textNode->GlyphAfter(word);
	Space* nextSpace = dynamic_cast<Space*>(nextGlyph);
	if (nextSpace)
		director->SetSelection(new BetweenWordsSelection(textNode, nextSpace));
	else {
		director->SetSelection(new BetweenGlyphsSelection(textNode,
		                                                  word, nextGlyph,
		                                                  true));
		}
}


void PointSelection::SelectBeforeWord(Glyph* word, DisplayDirector* director)
{
	Glyph* prevGlyph = textNode->GlyphBefore(word);
	Space* prevSpace = dynamic_cast<Space*>(prevGlyph);
	if (prevSpace)
		director->SetSelection(new BetweenWordsSelection(textNode, prevSpace));
	else {
		director->SetSelection(new BetweenGlyphsSelection(textNode,
		                                                  prevGlyph, word,
		                                                  false));
		}
}


void PointSelection::HandleReturn(unsigned long offset, DisplayDirector* director)
{
	Text* text = textNode->GetText();
	unsigned long textLen = text->Length();

	// handle "pre" sections specially
	if (textNode->IsPreSpace()) {
		director->DoAction(new AddCharsAction("\n", text, offset));
		return;
		}

	// split the paragraph
	CompositeAction* action = new CompositeAction();
	action->AddAction(new RestoreSelectionAction());

	// start setting up the split
	Element* splittingElement = EnclosingLeafBlockElement();
	Node* afterNode = text;

	// split the text
	if (offset < textLen)
		action->AddAction(new SplitTextAction(text, offset));

	// if not splitting the text, adjust afterNode
	else {
		while (afterNode != splittingElement) {
			if (afterNode->NextSibling() != NULL) {
				// this is where we want to split
				break;
				}

			// nothing after the afterNode, so move up a level
			afterNode = afterNode->ParentNode();
			}
		}

	// look for special-split
	EditStylesheet* stylesheet = director->GetStylesheet();
	String specialSplitSpec;
	for (Node* specialNode = splittingElement; specialNode; specialNode = specialNode->ParentNode()) {
		if (specialNode->NodeType() != ELEMENT_NODE)
			continue;
		specialSplitSpec =
			stylesheet->PropertyForElement("special-split", specialNode->NodeName());
		if (!specialSplitSpec.empty()) {
			splittingElement = dynamic_cast<Element*>(specialNode);
			break;
			}
		}

	// split the element
	if (afterNode != splittingElement)
		action->AddAction(new SplitElementAction(splittingElement, afterNode));

	// insert a new paragraph when appropriate
	if (afterNode == splittingElement || !specialSplitSpec.empty())
		action->AddAction(new NewLeafBlockAction(splittingElement, specialSplitSpec));

	director->DoAction(action);
}


Action* PointSelection::DeleteCharBefore()
{
	/*** TO BE DONE, or not ***/
	return NULL;
}


Action* PointSelection::DeleteCharAfter()
{
	/*** TO BE DONE, or not ***/
	return NULL;
}


bool PointSelection::IsInFirstTextOfBlock()
{
	for (DisplayNode* node = textNode; node; node = node->Parent()) {
		// if we've reached the enclosing block, we're done
		if (node->GetDisplayType() == Block)
			return true;

		// see if this node is the first node in its parent
		CompositeDisplayNode* parent = dynamic_cast<CompositeDisplayNode*>(node->Parent());
		if (parent == NULL) {
			// not composite; must have only one child
			continue;
			}
		if (node != parent->FirstChild())
			return false;
		}

	// got to the top and we're still first
	return true;
}


bool PointSelection::IsInLastTextOfBlock()
{
	for (DisplayNode* node = textNode; node; node = node->Parent()) {
		// if we've reached the enclosing block, we're done
		if (node->GetDisplayType() == Block)
			return true;

		// see if this node is the last node in its parent
		CompositeDisplayNode* parent = dynamic_cast<CompositeDisplayNode*>(node->Parent());
		if (parent == NULL) {
			// not composite; must have only one child
			continue;
			}
		if (node != parent->LastChild())
			return false;
		}

	// got to the top and we're still last
	return true;
}


Element* PointSelection::EnclosingLeafBlockElement()
{
	DisplayNode* block = textNode->EnclosingLeafBlock();
	for (; block; block = block->Parent()) {
		ElementDisplayNode* elementDisplayNode =
			dynamic_cast<ElementDisplayNode*>(block);
		if (elementDisplayNode)
			return elementDisplayNode->GetElement();
		}
	throw InternalException("CharacterSelection::EnclosingLeafBlockElement: there wasn't one.");
}


