/* RestoreBlocksSelectionAction.cpp */

#include "RestoreBlocksSelectionAction.h"
#include "BlocksSelection.h"
#include "ElementDisplayNode.h"
#include "Element.h"
#include "DisplayDirector.h"
#include "InternalException.h"



RestoreBlocksSelectionAction::RestoreBlocksSelectionAction(Element* firstElementIn, Element* lastElementIn)
	: firstElement(firstElementIn), lastElement(lastElementIn)
{
}


void RestoreBlocksSelectionAction::Do(DisplayDirector* director)
{
	director->SetSelection(NULL);
	Action::Do(director);
}


void RestoreBlocksSelectionAction::Undo(DisplayDirector* director)
{
	ElementDisplayNode* firstDisplayNode =
		dynamic_cast<ElementDisplayNode*>(director->DisplayNodeFor(firstElement));
	if (firstDisplayNode == NULL)
		throw InternalException("RestoreBlocksSelectAction::Do: couldn't get ElementDisplayNode.");
	ElementDisplayNode* lastDisplayNode =
		dynamic_cast<ElementDisplayNode*>(director->DisplayNodeFor(lastElement));
	if (lastDisplayNode == NULL)
		throw InternalException("RestoreBlocksSelectAction::Do: couldn't get ElementDisplayNode.");

	director->SetSelection(new BlocksSelection(firstDisplayNode, lastDisplayNode));

	Action::Undo(director);
}


