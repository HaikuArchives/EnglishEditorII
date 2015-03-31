/* GeneratedLeafBlock.cpp */

#include "GeneratedLeafBlock.h"
#include "ElementDisplayNode.h"
#include "TextDisplayNode.h"
#include "Element.h"
#include "Text.h"


GeneratedLeafBlock::GeneratedLeafBlock(DisplayNode* parentIn)
	: CompositeDisplayNode(parentIn)
{
	// set these now, since we know what they are and CheckIfBlock() (which usually
	// sets them up) is not gonna get called.
	isBlock = true;
	isLeafBlock = true;
}


DisplayType GeneratedLeafBlock::DeclaredDisplayType()
{
	return Block;
}


bool GeneratedLeafBlock::IsLeafBlock()
{
	return true;
}


bool GeneratedLeafBlock::IsDisposable()
{
	return true;
}


int GeneratedLeafBlock::Width()
{
	return dynamic_cast<CompositeDisplayNode*>(parent)->Width();
}


Font* GeneratedLeafBlock::CurFont()
{
	return parent->CurFont();
}


Node* GeneratedLeafBlock::FirstNode()
{
	ElementDisplayNode* elementNode = dynamic_cast<ElementDisplayNode*>(firstChild);
	if (elementNode)
		return elementNode->GetElement();
	TextDisplayNode* textNode = dynamic_cast<TextDisplayNode*>(firstChild);
	if (textNode)
		return textNode->GetText();
	return NULL;
}


Node* GeneratedLeafBlock::LastNode()
{
	ElementDisplayNode* elementNode = dynamic_cast<ElementDisplayNode*>(lastChild);
	if (elementNode)
		return elementNode->GetElement();
	TextDisplayNode* textNode = dynamic_cast<TextDisplayNode*>(lastChild);
	if (textNode)
		return textNode->GetText();
	return NULL;
}


bool GeneratedLeafBlock::CanSelectAcross()
{
	// not really meaningful, since this is never an inline node
	return false;
}


