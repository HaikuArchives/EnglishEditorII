/* BlockableDisplayNode.cpp */

#include "BlockableDisplayNode.h"
#include "DisplayDirector.h"
#include "InternalException.h"



BlockableDisplayNode::BlockableDisplayNode(DisplayNode* parentIn)
	: DisplayNode(parentIn), height(0)
{
}


int BlockableDisplayNode::Width()
{
	BlockableDisplayNode* parentNode = dynamic_cast<BlockableDisplayNode*>(parent);
	if (parentNode == NULL)
		throw InternalException("BlockableDisplayNode has non-block parent.");
	return
		parentNode->Width()
			- GetPropertyInPixels("margin-left", false)
			- GetPropertyInPixels("margin-right", false);
}


int BlockableDisplayNode::LeftSpace()
{
	BlockableDisplayNode* parentNode = dynamic_cast<BlockableDisplayNode*>(parent);
	if (parentNode == NULL)
		throw InternalException("BlockableDisplayNode has non-block parent.");
	return
		parentNode->LeftSpace()
			+ GetPropertyInPixels("margin-left", false);
}


BlockableDisplayNode* BlockableDisplayNode::AsBlockableDisplayNode()
{
	return this;
}


int BlockableDisplayNode::Top()
{
	return (parent ? dynamic_cast<BlockableDisplayNode*>(parent)->GetChildTop(this) : 0);
}


int BlockableDisplayNode::Bottom()
{
	return Top() + Height();
}


int BlockableDisplayNode::DisplayWidth()
{
	int width = Width();
	int textIndent = GetPropertyInPixels("text-indent", true);
	if (textIndent < 0)
		width += -textIndent;
	return width;
}


int BlockableDisplayNode::DisplayLeft()
{
	int left = LeftSpace();
	int textIndent = GetPropertyInPixels("text-indent", true);
	if (textIndent < 0)
		left -= -textIndent;
	return left;
}


void BlockableDisplayNode::HeightChangedBy(int heightDelta)
{
	height += heightDelta;

	// pass it on up the tree
	BlockableDisplayNode* parentBlock = dynamic_cast<BlockableDisplayNode*>(parent);
	if (parentBlock)
		parentBlock->HeightChangedBy(heightDelta);
}


void BlockableDisplayNode::HeightChangedTo(int newHeight)
{
	HeightChangedBy(newHeight - height);
}


void BlockableDisplayNode::VerticalMarginsChanged()
{
	CalcVerticalMargins();

	if (parent && (this == parent->FirstChild() || this == parent->LastChild())) {
		BlockableDisplayNode* blockParent = dynamic_cast<BlockableDisplayNode*>(parent);
		if (blockParent)
			blockParent->VerticalMarginsChanged();
		}
}


void BlockableDisplayNode::RefreshAfter()
{
	DisplayDirector* director = GetDisplayDirector();
	if (director == NULL)
		throw InternalException("BlockableDisplayNode::RefreshDocAfter(): not attached to director.");
	director->RefreshDocAfter(Top());
}


