/* SelectBetweenBlocksAction.cpp */

#include "SelectBetweenBlocksAction.h"
#include "BetweenBlocksSelection.h"
#include "BlockableDisplayNode.h"
#include "DisplayDirector.h"
#include "InternalException.h"



SelectBetweenBlocksAction::SelectBetweenBlocksAction(Node* parentNodeIn, Node* beforeNodeIn)
	: parentNode(parentNodeIn), beforeNode(beforeNodeIn)
{
}


void SelectBetweenBlocksAction::Do(DisplayDirector* director)
{
	BlockableDisplayNode* parentDisplayNode =
		dynamic_cast<BlockableDisplayNode*>(director->DisplayNodeFor(parentNode));
	if (useAssertions && parentDisplayNode == NULL)
		throw InternalException("SelectBetweenBlocksAction: Couldn't get parent display node.");
	BlockableDisplayNode* beforeDisplayNode = NULL;
	if (beforeNode) {
		beforeDisplayNode = dynamic_cast<BlockableDisplayNode*>(director->DisplayNodeFor(beforeNode));
		if (useAssertions && beforeDisplayNode == NULL)
			throw InternalException("SelectBetweenBlocksAction: Couldn't get 'before' display node.");
		}
	director->SetSelection(new BetweenBlocksSelection(parentDisplayNode, beforeDisplayNode));

	Action::Do(director);
}


void SelectBetweenBlocksAction::Undo(DisplayDirector* director)
{
	director->SetSelection(NULL);
	Action::Undo(director);
}


