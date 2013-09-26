/* RestoreBetweenBlocksSelectionAction.cpp */

#include "RestoreBetweenBlocksSelectionAction.h"
#include "Node.h"
#include "BlockableDisplayNode.h"
#include "BetweenBlocksSelection.h"
#include "DisplayDirector.h"
#include "InternalException.h"



RestoreBetweenBlocksSelectionAction::RestoreBetweenBlocksSelectionAction(Node* parentNodeIn, Node* beforeNodeIn)
	: parentNode(parentNodeIn), beforeNode(beforeNodeIn)
{
}


void RestoreBetweenBlocksSelectionAction::Do(DisplayDirector* director)
{
	director->SetSelection(NULL);

	Action::Do(director);
}


void RestoreBetweenBlocksSelectionAction::Undo(DisplayDirector* director)
{
	BlockableDisplayNode* parentDisplayNode =
		dynamic_cast<BlockableDisplayNode*>(director->DisplayNodeFor(parentNode));
	if (useAssertions && parentDisplayNode == NULL)
		throw InternalException("RestoreBetweenBlocksSelectionAction: parent isn't blockable.");
	BlockableDisplayNode* beforeDisplayNode = NULL;
	if (beforeNode) {
		beforeDisplayNode =
			dynamic_cast<BlockableDisplayNode*>(director->DisplayNodeFor(beforeNode));
		if (useAssertions && beforeDisplayNode == NULL)
			throw InternalException("RestoreBetweenBlocksSelectionAction: beforeNode isn't blockable.");
		}
	director->SetSelection(new BetweenBlocksSelection(parentDisplayNode, beforeDisplayNode));

	Action::Undo(director);
}


