/* CompositeDisplayNode.cpp */

#include "CompositeDisplayNode.h"
#include "Element.h"
#include "Text.h"
#include "GeneratedLeafBlock.h"
#include "TextDisplayNode.h"
#include "DisplayTextNode.h"
#include "DisplayElementNode.h"
#include "PlaceholderTextDisplayNode.h"
#include "DisplayDirector.h"
#include "BlocksSelection.h"
#include "BlockElementSelection.h"
#include "BetweenBlocksSelection.h"
#include "BetweenBlocksDestination.h"
#include "InlineLayoutContext.h"
#include "InlineDrawContext.h"
#include "BlockDrawContext.h"
#include "FindSelectionContext.h"
#include "FindDestinationContext.h"
#include "FindHotspotContext.h"
#include "EditStylesheet.h"
#include "GlyphLoc.h"
#include "InternalException.h"
#include <typeinfo>



CompositeDisplayNode::CompositeDisplayNode(DisplayNode* parentIn)
	: BlockableDisplayNode(parentIn), firstChild(NULL), lastChild(NULL),
	  isBlock(false), isLeafBlock(false),
	  topMargin(0), bottomMargin(0)
{
}


CompositeDisplayNode::~CompositeDisplayNode()
{
	ClearChildren();
}


DisplayType CompositeDisplayNode::GetDisplayType()
{
	return (isBlock ? Block : Inline);
}


bool CompositeDisplayNode::IsLeafBlock()
{
	return isLeafBlock;
}


void CompositeDisplayNode::UpdateProperties()
{
	// have all children update properties--must do this before updating margins
	for (DisplayNode* childNode = firstChild; childNode; childNode = childNode->NextSibling())
		childNode->UpdateProperties();

	CalcVerticalMargins();
}


Selection* CompositeDisplayNode::StartSelection()
{
	for (DisplayNode* childNode = firstChild; childNode; childNode = childNode->NextSibling()) {
		InlineableDisplayNode* child = dynamic_cast<InlineableDisplayNode*>(childNode);
		if (child == NULL)
			throw InternalException("CompositeDisplayNode::StartSelection: Child is not inline.");
		Selection* startSelection = child->StartSelection();
		if (startSelection)
			return startSelection;
		}

	return NULL;
}


Selection* CompositeDisplayNode::EndSelection()
{
	for (DisplayNode* childNode = lastChild; childNode; childNode = childNode->PreviousSibling()) {
		InlineableDisplayNode* child = dynamic_cast<InlineableDisplayNode*>(childNode);
		if (child == NULL)
			throw InternalException("CompositeDisplayNode::EndSelection: Child is not inline.");
		Selection* endSelection = child->EndSelection();
		if (endSelection)
			return endSelection;
		}

	return NULL;
}


DisplayNode* CompositeDisplayNode::FirstChild()
{
	return firstChild;
}


DisplayNode* CompositeDisplayNode::LastChild()
{
	return lastChild;
}


void CompositeDisplayNode::BlockLayout(bool force)
{
	if (isLeafBlock)
		LayoutInlineChildren();
	else
		LayoutChildBlocks(force);
	needsLayout = false;
}


void CompositeDisplayNode::BlockDraw(BlockDrawContext* context)
{
	if (isLeafBlock)
		DrawInlineChildren(context);
	else
		DrawChildBlocks(context);
}


Selection* CompositeDisplayNode::BlockFindSelection(FindSelectionContext* context)
{
	if (isLeafBlock)
		return FindSelectionInInlineChildren(context);
	else
		return FindSelectionInChildBlocks(context);
}


Destination* CompositeDisplayNode::BlockFindDestination(FindDestinationContext* context)
{
	if (isLeafBlock)
		return FindDestinationInInlineChildren(context);
	else
		return FindDestinationInChildBlocks(context);
}


Hotspot* CompositeDisplayNode::BlockFindHotspot(FindHotspotContext* context)
{
	// first, see if this block has a hotspot
	Hotspot* hotspot = GetHotspot(context->director);
	if (hotspot)
		return hotspot;

	// otherwise, check the children
	if (isLeafBlock)
		return FindHotspotInInlineChildren(context);
	else
		return FindHotspotInChildBlocks(context);
}


int CompositeDisplayNode::TopMargin()
{
	return topMargin;

/***
	int topMargin = GetPropertyInPixels("margin-top", false);
	if (isBlock && !isLeafBlock && firstChild) {
		BlockableDisplayNode* topChild =
			dynamic_cast<BlockableDisplayNode*>(firstChild);
		if (topChild == NULL)
			throw InternalException("Non-leaf block has non-block child.");
		int childTopMargin = topChild->TopMargin();
		if (childTopMargin > topMargin)
			topMargin = childTopMargin;
		}
	return topMargin;
***/
}


int CompositeDisplayNode::BottomMargin()
{
	return bottomMargin;

/***
	int bottomMargin = GetPropertyInPixels("margin-bottom", false);
	if (isBlock && !isLeafBlock && lastChild) {
		BlockableDisplayNode* bottomChild =
			dynamic_cast<BlockableDisplayNode*>(lastChild);
		if (bottomChild == NULL)
			throw InternalException("Non-leaf block has non-block child.");
		int childBottomMargin = bottomChild->BottomMargin();
		if (childBottomMargin > bottomMargin)
			bottomMargin = childBottomMargin;
		}
	return bottomMargin;
***/
}


int CompositeDisplayNode::GetChildTop(DisplayNode* child)
{
	int interBlockSpace = 0;
	int childTop = Top();
	for (DisplayNode* childNode = firstChild; childNode; childNode = childNode->NextSibling()) {
		BlockableDisplayNode* childBlock = dynamic_cast<BlockableDisplayNode*>(childNode);
		if (childBlock == NULL)
			throw InternalException("Non-leaf block has inline child.");

		// add inter-block space
		if (childBlock != firstChild) {
			// use the max of the last child's bottom margin and the current child's
			// top margin, but this block's top margin isn't counted as part of
			// its height (which is why we don't do this on the first child block,
			// which contributes to this block's top margin)
			int childTopMargin = childBlock->TopMargin();
			if (childTopMargin > interBlockSpace)
				interBlockSpace = childTopMargin;
			}
		childTop += interBlockSpace;

		// check if we're done
		if (childNode == child)
			return childTop;

		// add to the height
		childTop += childBlock->Height();
		interBlockSpace = childBlock->BottomMargin();
		}

	return childTop;
}


void CompositeDisplayNode::CalcVerticalMargins()
{
	// top margin
	topMargin = GetPropertyInPixels("margin-top", false);
	if (isBlock && !isLeafBlock && firstChild) {
		BlockableDisplayNode* topChild =
			dynamic_cast<BlockableDisplayNode*>(firstChild);
		if (topChild == NULL)
			throw InternalException("Non-leaf block has non-block child.");
		int childTopMargin = topChild->TopMargin();
		if (childTopMargin > topMargin)
			topMargin = childTopMargin;
		}

	// bottom margin
	bottomMargin = GetPropertyInPixels("margin-bottom", false);
	if (isBlock && !isLeafBlock && lastChild) {
		BlockableDisplayNode* bottomChild =
			dynamic_cast<BlockableDisplayNode*>(lastChild);
		if (bottomChild == NULL)
			throw InternalException("Non-leaf block has non-block child.");
		int childBottomMargin = bottomChild->BottomMargin();
		if (childBottomMargin > bottomMargin)
			bottomMargin = childBottomMargin;
		}
}


int CompositeDisplayNode::MaxLineWidth()
{
	// if this is a leaf block, we saved the maxLineWidth after the last layout
	if (isLeafBlock)
		return maxLineWidth;

	// otherwise, get the max of the children's line widths
	maxLineWidth = 0;
	for (DisplayNode* childNode = firstChild; childNode; childNode = childNode->NextSibling()) {
		BlockableDisplayNode* childBlock = dynamic_cast<BlockableDisplayNode*>(childNode);
		if (childBlock == NULL)
			throw InternalException("Non-leaf block has inline child.");

		int childLineWidth = childBlock->MaxLineWidth();
		if (childLineWidth > maxLineWidth)
			maxLineWidth = childLineWidth;
		}
	return maxLineWidth;
}


void CompositeDisplayNode::InlineLayout(InlineLayoutContext* layoutContext)
{
	// layout the children
	for (DisplayNode* childNode = firstChild; childNode; childNode = childNode->NextSibling()) {
		InlineableDisplayNode* child = dynamic_cast<InlineableDisplayNode*>(childNode);
		if (child == NULL)
			throw InternalException("Attempt to layout non-InlineableDisplayNode.");

		child->InlineLayout(layoutContext);
		}
}


void CompositeDisplayNode::InlineDraw(InlineDrawContext* context)
{
	for (DisplayNode* childNode = firstChild; childNode; childNode = childNode->NextSibling()) {
		InlineableDisplayNode* child = dynamic_cast<InlineableDisplayNode*>(childNode);
		if (child == NULL)
			throw InternalException("Attempt to draw non-InlineableDisplayNode.");

		child->InlineDraw(context);
		}
}


Selection* CompositeDisplayNode::InlineFindSelection(FindSelectionContext* context)
{
	for (DisplayNode* childNode = firstChild; childNode; childNode = childNode->NextSibling()) {
		InlineableDisplayNode* child = dynamic_cast<InlineableDisplayNode*>(childNode);
		if (child == NULL)
			throw InternalException("Attempt to find selection in non-InlineableDisplayNode.");

		Selection* selection = child->InlineFindSelection(context);
		if (selection)
			return selection;
		}
	return NULL;
}


Destination* CompositeDisplayNode::InlineFindDestination(FindDestinationContext* context)
{
	for (DisplayNode* childNode = firstChild; childNode; childNode = childNode->NextSibling()) {
		InlineableDisplayNode* child = dynamic_cast<InlineableDisplayNode*>(childNode);
		if (child == NULL)
			throw InternalException("Attempt to find destination in non-InlineableDisplayNode.");

		Destination* destination = child->InlineFindDestination(context);
		if (destination)
			return destination;
		}
	return NULL;
}


Hotspot* CompositeDisplayNode::InlineFindHotspot(FindHotspotContext* context)
{
	// if the mouse is in this node, see if there's a hotspot for it
	CoordPoint mousePoint(context->mouseX - context->xIndent,
	                      context->mouseY - context->curY);
	GlyphRef firstGlyph = FirstSelectableGlyph();
	GlyphRef lastGlyph = LastSelectableGlyph();
	if (!firstGlyph.IsValid() || !lastGlyph.IsValid() ||
			GlyphLoc(firstGlyph) > mousePoint)
		return NULL;
	if (GlyphLoc(lastGlyph) >= mousePoint) {
		Hotspot* hotspot = GetHotspot(context->director);
		if (hotspot)
			return hotspot;
		}

	// otherwise, check children
	for (DisplayNode* childNode = firstChild; childNode; childNode = childNode->NextSibling()) {
		InlineableDisplayNode* child = dynamic_cast<InlineableDisplayNode*>(childNode);
		if (child == NULL)
			throw InternalException("Attempt to find hotspot in non-InlineableDisplayNode.");

		Hotspot* hotspot = child->InlineFindHotspot(context);
		if (hotspot)
			return hotspot;
		}

	return NULL;
}


GlyphRef CompositeDisplayNode::FirstSelectableGlyph()
{
	for (DisplayNode* childNode = firstChild; childNode; childNode = childNode->NextSibling()) {
		InlineableDisplayNode* child = dynamic_cast<InlineableDisplayNode*>(childNode);
		if (child == NULL)
			throw InternalException("CompositeDisplayNode::FirstGlyph: Child is not inline.");
		GlyphRef firstGlyph = child->FirstSelectableGlyph();
		if (firstGlyph.IsValid())	
			return firstGlyph;
		}

	return GlyphRef();
}


GlyphRef CompositeDisplayNode::LastSelectableGlyph()
{
	for (DisplayNode* childNode = lastChild; childNode; childNode = childNode->PreviousSibling()) {
		InlineableDisplayNode* child = dynamic_cast<InlineableDisplayNode*>(childNode);
		if (child == NULL)
			throw InternalException("CompositeDisplayNode::FirstGlyph: Child is not inline.");
		GlyphRef lastGlyph = child->LastSelectableGlyph();
		if (lastGlyph.IsValid())	
			return lastGlyph;
		}

	return GlyphRef();
}


GlyphRef CompositeDisplayNode::LastGlyph()
{
	for (DisplayNode* childNode = lastChild; childNode; childNode = childNode->PreviousSibling()) {
		InlineableDisplayNode* child = dynamic_cast<InlineableDisplayNode*>(childNode);
		if (child == NULL)
			throw InternalException("CompositeDisplayNode::FirstGlyph: Child is not inline.");
		GlyphRef lastGlyph = child->LastGlyph();
		if (lastGlyph.IsValid())	
			return lastGlyph;
		}

	return GlyphRef();
}


void CompositeDisplayNode::ChildInsertedBefore(Node* newChild, Node* refChild)
{
	// don't bother if we're not currently attached
	DisplayDirector* director = GetDisplayDirector();
	if (director == NULL) {
		// mark as needing a load, so we'll get one if added back into a document
		needsLoad = true;
		return;
		}

	// load
	Load(director);

	// get the child set up
	DisplayNode* childDisplayNode = director->DisplayNodeFor(newChild, false);
	if (childDisplayNode) {
		childDisplayNode->UpdateProperties();
		if (childDisplayNode == firstChild || childDisplayNode == lastChild)
			VerticalMarginsChanged();
		if (childDisplayNode->NeedsLoad())
			childDisplayNode->Load(director);
		}

	// layout & refresh
	BlockableDisplayNode* block = (GetDisplayType() == Block ? this : EnclosingLeafBlock());
	if (block == NULL)
		throw InternalException("ElementDisplayNode::ChildInsertedBefore: No block.");
	block->BlockLayout();
	block->RefreshAfter();
}


void CompositeDisplayNode::ChildRemoved(Node* oldChild)
{
	// don't bother if we're not currently attached
	DisplayDirector* director = GetDisplayDirector();
	if (director == NULL) {
		// mark as needing a load, so we'll get one if added back into a document
		needsLoad = true;
		return;
		}

	// load
	Load(director);
	VerticalMarginsChanged();

	// layout & refresh
	BlockableDisplayNode* block = (GetDisplayType() == Block ? this : EnclosingLeafBlock());
	if (block == NULL)
		throw InternalException("ElementDisplayNode::ChildInsertedBefore: No block.");
	block->BlockLayout();
	block->RefreshAfter();
}


void CompositeDisplayNode::ChildReplaced(Node* newChild, Node* oldChild)
{
	// don't bother if we're not currently attached
	DisplayDirector* director = GetDisplayDirector();
	if (director == NULL) {
		// mark as needing a load, so we'll get one if added back into a document
		needsLoad = true;
		return;
		}

	// load
	Load(director);

	// get the child set up
	DisplayNode* childDisplayNode = director->DisplayNodeFor(newChild, false);
	if (childDisplayNode) {
		childDisplayNode->UpdateProperties();
		if (childDisplayNode == firstChild || childDisplayNode == lastChild)
			VerticalMarginsChanged();
		if (childDisplayNode->NeedsLoad())
			childDisplayNode->Load(director);
		}

	// layout & refresh
	BlockableDisplayNode* block = (GetDisplayType() == Block ? this : EnclosingLeafBlock());
	if (block == NULL)
		throw InternalException("ElementDisplayNode::ChildReplaced: No block.");
	block->BlockLayout();
	block->RefreshAfter();
}


void CompositeDisplayNode::ClearChildren()
{
	DisplayNode* nextNode = NULL;
	for (DisplayNode* curNode = firstChild; curNode; curNode = nextNode) {
		nextNode = curNode->NextSibling();
		curNode->SetParent(NULL);
		curNode->SetPrevSibling(NULL);
		curNode->SetNextSibling(NULL);
		if (curNode->IsDisposable())
			delete curNode;
		}
	firstChild = lastChild = NULL;
}


void CompositeDisplayNode::LoadFromElement(Element* element, DisplayDirector* director)
{
	bool hadChildren = false;
	EditStylesheet* stylesheet = director->GetStylesheet();
	for (Node* child = element->FirstChild(); child; child = child->NextSibling()) {
		if (child->NodeType() == ELEMENT_NODE &&
		         stylesheet->PropertyForElement("display", child->NodeName()) == "none")
			continue;
		DisplayNode* displayNode = director->DisplayNodeFor(child);
		if (displayNode) {
			AppendChild(displayNode);
			hadChildren = true;
			}
		}

	// if empty, use placeholder text
	if (!hadChildren) {
		DOMString placeholderText =
			stylesheet->PropertyForElement("placeholder-text", element->NodeName());
		if (true /*** !placeholderText.empty() ***/) {
			PlaceholderTextDisplayNode* placeholderNode =
				new PlaceholderTextDisplayNode(placeholderText);
			AppendChild(placeholderNode);
			}
		}
}


void CompositeDisplayNode::LoadFromContentsTemplate(Element* templateElement, Element* sourceElement, DisplayDirector* director)
{
	for (Node* child = templateElement->FirstChild(); child; child = child->NextSibling()) {
		DisplayNode* newDisplayNode = NULL;
		if (child->NodeType() == TEXT_NODE)
			newDisplayNode = new DisplayTextNode(dynamic_cast<Text*>(child));
		else if (child->NodeType() == ELEMENT_NODE) {
			DOMString tagName = child->NodeName();
			if (tagName == "display-element") {
				newDisplayNode =
					new DisplayElementNode(dynamic_cast<Element*>(child), sourceElement);
				}
			else if (tagName == "all-contents")
				LoadFromElement(sourceElement, director);
			}
		if (newDisplayNode)
			AppendChild(newDisplayNode);
		}
}


void CompositeDisplayNode::LoadChildren(DisplayDirector* director)
{
	for (DisplayNode* child = firstChild; child; child = child->NextSibling()) {
		if (child->NeedsLoad())
			child->Load(director);
		}
}


void CompositeDisplayNode::LayoutChildBlocks(bool force)
{
	int interBlockSpace = 0;
	int totalHeight = 0;
	for (DisplayNode* child = firstChild; child; child = child->NextSibling()) {
		BlockableDisplayNode* childBlock = dynamic_cast<BlockableDisplayNode*>(child);
		if (childBlock == NULL)
			throw InternalException("Non-leaf block has inline child.");

		if (force || childBlock->NeedsLayout())
			childBlock->BlockLayout(force);

		// calc the total height, accounting for space between the blocks
		totalHeight += childBlock->Height();
		if (childBlock != firstChild) {
			// use the max of the last child's bottom margin and the current child's
			// top margin, but this block's top margin isn't counted as part of
			// its height (which is why we don't do this on the first child block,
			// which contributes to this block's top margin
			int childTopMargin = childBlock->TopMargin();
			if (childTopMargin > interBlockSpace)
				interBlockSpace = childTopMargin;
			}
		totalHeight += interBlockSpace;
		interBlockSpace = childBlock->BottomMargin();
		}
	// note that we ignore the last interBlockSpace, since the bottom margin (including
	// the effect of the last child's bottom margin) doesn't count towards this
	// block's total height

	// finish setting up the total height
	HeightChangedTo(totalHeight);
}


void CompositeDisplayNode::LayoutInlineChildren()
{
	InlineLayoutContext layoutContext(Width(),
	                                  GetPropertyInPixels("text-indent", true),
	                                  GetInheritedProperty("text-align"));
	InlineLayout(&layoutContext);
	layoutContext.Finish();
	HeightChangedTo(layoutContext.TotalHeight());
	maxLineWidth = layoutContext.MaxLineWidth() + LeftSpace();
}


void CompositeDisplayNode::DrawChildBlocks(BlockDrawContext* context)
{
	int interBlockSpace = 0;
	for (DisplayNode* child = firstChild; child; child = child->NextSibling()) {
		BlockableDisplayNode* childBlock = dynamic_cast<BlockableDisplayNode*>(child);
		if (childBlock == NULL)
			throw InternalException("Non-leaf block has inline child.");

		// handle inter-block spacing (see LayoutChildBlocks() for a lengthier explanation)
		if (childBlock != firstChild) {
			int childTopMargin = childBlock->TopMargin();
			if (childTopMargin > interBlockSpace)
				interBlockSpace = childTopMargin;
			context->curY += interBlockSpace;
			}

		// check if we're done
		if (context->curY > context->endY) {
			return;
			}

		// draw or skip the child
		int childHeight = childBlock->Height();
		if (context->curY + childHeight < context->startY)
			context->curY += childHeight;
		else {
			childBlock->BlockDraw(context);
			}

		// set up to handle next inter-block space
		interBlockSpace = childBlock->BottomMargin();
		}
}


void CompositeDisplayNode::DrawInlineChildren(BlockDrawContext* context)
{
	InlineDrawContext inlineDrawContext(context, LeftSpace());
	InlineDraw(&inlineDrawContext);
	context->curY += height;
}


Selection* CompositeDisplayNode::FindSelectionInChildBlocks(FindSelectionContext* context)
{
	int interBlockSpace = 0;
	for (DisplayNode* child = firstChild; child; child = child->NextSibling()) {
		BlockableDisplayNode* childBlock = dynamic_cast<BlockableDisplayNode*>(child);
		if (childBlock == NULL)
			throw InternalException("Non-leaf block has inline child.");

		// handle inter-block spacing (see LayoutChildBlocks() for a lengthier explanation)
		if (childBlock != firstChild) {
			int childTopMargin = childBlock->TopMargin();
			if (childTopMargin > interBlockSpace)
				interBlockSpace = childTopMargin;
			context->curY += interBlockSpace;
			}

		// check if we're done
		if (context->curY > context->mouseY) {
			// select between blocks
			return new BetweenBlocksSelection(this, childBlock);
			}

		// check or skip the child
		int childHeight = childBlock->Height();
		if (context->curY + childHeight < context->mouseY)
			context->curY += childHeight;
		else {
			Selection* selection = childBlock->BlockFindSelection(context);
			if (selection)
				return selection;
			}

		// set up to handle next inter-block space
		interBlockSpace = childBlock->BottomMargin();
		}

	return NULL;
}


Selection* CompositeDisplayNode::FindSelectionInInlineChildren(FindSelectionContext* context)
{
	context->xIndent = LeftSpace();
	Selection* selection = InlineFindSelection(context);
	context->curY += height;
	return (selection ? selection : EndSelection());
}


Destination* CompositeDisplayNode::FindDestinationInChildBlocks(FindDestinationContext* context)
{
	int interBlockSpace = 0;
	BlockableDisplayNode* prevBlock = NULL;
	for (DisplayNode* child = firstChild; child; child = child->NextSibling()) {
		BlockableDisplayNode* childBlock = dynamic_cast<BlockableDisplayNode*>(child);
		if (childBlock == NULL)
			throw InternalException("Non-leaf block has inline child.");

		// handle inter-block spacing (see LayoutChildBlocks() for a lengthier explanation)
		if (childBlock != firstChild) {
			int childTopMargin = childBlock->TopMargin();
			if (childTopMargin > interBlockSpace)
				interBlockSpace = childTopMargin;
			context->curY += interBlockSpace;
			}

		// check if we're done
		if (context->mouseY < context->curY) {
			// destination is between blocks
			if (typeid(*context->source) == typeid(BlocksSelection) ||
					typeid(*context->source) == typeid(BlockElementSelection))
				return new BetweenBlocksDestination(context->source, this, prevBlock);
			else
				return NULL;
			}

		// check or skip the child
		int childHeight = childBlock->Height();
		if (context->curY + childHeight < context->mouseY)
			context->curY += childHeight;
		else {
			// check within the child
			Destination* destination = childBlock->BlockFindDestination(context);
			if (destination)
				return destination;
			// no destination within the child; check before and after
			// note that context->curY now include the childHeight
			if (context->mouseY < context->curY - childHeight / 2) {
				// destination is before the block
				if (typeid(*context->source) == typeid(BlocksSelection) ||
						typeid(*context->source) == typeid(BlockElementSelection))
					return new BetweenBlocksDestination(context->source, this, prevBlock);
				else
					return NULL;
				}
			else {
				// destination is after the block
				if (typeid(*context->source) == typeid(BlocksSelection) ||
						typeid(*context->source) == typeid(BlockElementSelection))
					return new BetweenBlocksDestination(context->source, this, childBlock);
				else
					return NULL;
				}
			}

		// set up to handle next inter-block space
		interBlockSpace = childBlock->BottomMargin();
		prevBlock = childBlock;
		}

	return NULL;
}


Destination* CompositeDisplayNode::FindDestinationInInlineChildren(FindDestinationContext* context)
{
	context->xIndent = LeftSpace();
	Destination* destination = InlineFindDestination(context);
	context->curY += height;
	return destination;
}


Hotspot* CompositeDisplayNode::FindHotspotInChildBlocks(FindHotspotContext* context)
{
	int interBlockSpace = 0;
	for (DisplayNode* child = firstChild; child; child = child->NextSibling()) {
		BlockableDisplayNode* childBlock = dynamic_cast<BlockableDisplayNode*>(child);
		if (childBlock == NULL)
			throw InternalException("Non-leaf block has inline child.");

		// handle inter-block spacing (see LayoutChildBlocks() for a lengthier explanation)
		if (childBlock != firstChild) {
			int childTopMargin = childBlock->TopMargin();
			if (childTopMargin > interBlockSpace)
				interBlockSpace = childTopMargin;
			context->curY += interBlockSpace;
			}

		// check if we're done
		if (context->curY > context->mouseY)
			break;

		// check or skip the child
		int childHeight = childBlock->Height();
		if (context->curY + childHeight < context->mouseY)
			context->curY += childHeight;
		else {
			Hotspot* hotspot = childBlock->BlockFindHotspot(context);
			if (hotspot)
				return hotspot;
			}

		// set up to handle next inter-block space
		interBlockSpace = childBlock->BottomMargin();
		}

	return NULL;
}


Hotspot* CompositeDisplayNode::FindHotspotInInlineChildren(FindHotspotContext* context)
{
	context->xIndent = LeftSpace();
	Hotspot* hotspot = InlineFindHotspot(context);
	context->curY += height;
	return hotspot;
}


void CompositeDisplayNode::CheckIfBlock()
{
	DisplayNode* child;

	// check children
	bool hasBlockChildren = false;
	bool hasGeneratedLeafBlocks = false;
	bool hasDirectInlines = false;
	for (child = firstChild; child; child = child->NextSibling()) {
		if (typeid(*child) == typeid(GeneratedLeafBlock))
			hasGeneratedLeafBlocks = true;
		else if (child->GetDisplayType() == Block)
			hasBlockChildren = true;
		else
			hasDirectInlines = true;
		}

	// set block status
	isBlock = (hasBlockChildren || DeclaredDisplayType() == Block);
	isLeafBlock = (isBlock && !hasBlockChildren);

	// if we're a non-leaf block, we need to get rid of any nodes for ignorable whitespace
	if (isBlock && !isLeafBlock) {
		DisplayNode* nextChild;
		for (child = firstChild; child; child = nextChild) {
			nextChild = child->NextSibling();
			TextDisplayNode* textChild = dynamic_cast<TextDisplayNode*>(child);
			if (textChild && textChild->GetText()->IsIgnorableWhitespace()) {
				UnlinkChildren(child, child);
				delete child;
				}
			}
		}

	// if we're a non-leaf block, we need GeneratedLeafBlocks on any inline children
	if (isBlock && !isLeafBlock && hasDirectInlines) {
		GeneratedLeafBlock* curGenLeafBlock = NULL;
		for (child = firstChild; child; child = child->NextSibling()) {
			if (child->GetDisplayType() == Inline) {
				if (curGenLeafBlock == NULL) {
					// make the GeneratedLeafBlock & link it in
					curGenLeafBlock = new GeneratedLeafBlock(this);
					LinkChildrenBefore(curGenLeafBlock, curGenLeafBlock, child->NextSibling());
					}
				// add "child" to "curGenLeafBlock"
				UnlinkChildren(child, child);
				curGenLeafBlock->AppendChildren(child, child);
				child = curGenLeafBlock;	// keep the loop going
				}
			else if (curGenLeafBlock) {
				// close out the current GeneratedLeafBlock
				curGenLeafBlock = NULL;
				}
			}
		}

	// if we're inline, or a leaf block, we need to ablate any GeneratedLeafBlocks
	else if ((!isBlock || isLeafBlock) && hasGeneratedLeafBlocks) {
		for (child = firstChild; child; child = child->NextSibling()) {
			if (child->GetDisplayType() != Inline) {
				// ablate the GeneratedLeafBlock
				GeneratedLeafBlock* genLeafBlock = dynamic_cast<GeneratedLeafBlock*>(child);
				DisplayNode* firstToPromote = genLeafBlock->FirstChild();
				DisplayNode* lastToPromote = genLeafBlock->LastChild();
				genLeafBlock->UnlinkChildren(firstToPromote, lastToPromote);
				LinkChildrenBefore(firstToPromote, lastToPromote, genLeafBlock);
				UnlinkChildren(genLeafBlock, genLeafBlock);
				child = lastToPromote;	// keep the loop going
				}
			}
		}
}


void CompositeDisplayNode::UnlinkChildren(DisplayNode* firstToUnlink, DisplayNode* lastToUnlink)
{
	// unlink
	if (firstToUnlink == firstChild)
		firstChild = firstToUnlink->NextSibling();
	if (lastToUnlink == lastChild)
		lastChild = lastToUnlink->PreviousSibling();
	DisplayNode* prevChild = firstToUnlink->PreviousSibling();
	DisplayNode* nextChild = lastToUnlink->NextSibling();
	if (prevChild)
		prevChild->SetNextSibling(nextChild);
	if (nextChild)
		nextChild->SetPrevSibling(prevChild);
	firstToUnlink->SetPrevSibling(NULL);
	lastToUnlink->SetNextSibling(NULL);

	// set parents
	for (DisplayNode* node = firstToUnlink; node; node = node->NextSibling())
		node->SetParent(NULL);
}


void CompositeDisplayNode::LinkChildrenBefore(DisplayNode* firstToLink, DisplayNode* lastToLink, DisplayNode* beforeChild)
{
	if (beforeChild == NULL) {
		AppendChildren(firstToLink, lastToLink);
		return;
		}

	// link
	if (beforeChild == firstChild)
		firstChild = firstToLink;
	DisplayNode* prevChild = beforeChild->PreviousSibling();
	if (prevChild)
		prevChild->SetNextSibling(firstToLink);
	firstToLink->SetPrevSibling(prevChild);
	lastToLink->SetNextSibling(beforeChild);
	beforeChild->SetPrevSibling(lastToLink);

	// set parents
	for (DisplayNode* child = firstToLink; ; child = child->NextSibling()) {
		child->SetParent(this);
		if (child == lastToLink)
			break;
		}
}


void CompositeDisplayNode::AppendChildren(DisplayNode* firstToLink, DisplayNode* lastToLink)
{
	// link
	if (lastChild)
		lastChild->SetNextSibling(firstToLink);
	firstToLink->SetPrevSibling(lastChild);
	lastChild = lastToLink;
	if (firstChild == NULL)
		firstChild = firstToLink;

	// set parents
	for (DisplayNode* child = firstToLink; ; child = child->NextSibling()) {
		child->SetParent(this);
		if (child == lastToLink)
			break;
		}
}


void CompositeDisplayNode::AppendChild(DisplayNode* newChild)
{
	// link
	if (lastChild)
		lastChild->SetNextSibling(newChild);
	newChild->SetPrevSibling(lastChild);
	lastChild = newChild;
	if (firstChild == NULL)
		firstChild = newChild;

	// set parent
	newChild->SetParent(this);
}


