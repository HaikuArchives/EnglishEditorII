/* SelectBlockElementsAction.cpp */

#include "SelectBlockElementsAction.h"
#include "BlocksSelection.h"
#include "BlockElementSelection.h"
#include "Element.h"
#include "DisplayDirector.h"
#include "ElementDisplayNode.h"



SelectBlockElementsAction::SelectBlockElementsAction(Element* firstElementIn, Element* lastElementIn)
	: firstElement(firstElementIn), lastElement(lastElementIn)
{
}


void SelectBlockElementsAction::Do(DisplayDirector* director)
{
	ElementDisplayNode* firstBlock =
		dynamic_cast<ElementDisplayNode*>(director->DisplayNodeFor(firstElement));
	ElementDisplayNode* lastBlock =
		dynamic_cast<ElementDisplayNode*>(director->DisplayNodeFor(lastElement));
	if (firstBlock && lastBlock) {
		if (firstBlock == lastBlock)
			director->SetSelection(new BlockElementSelection(firstBlock));
		else
			director->SetSelection(new BlocksSelection(firstBlock, lastBlock));
		}

	Action::Do(director);
}


void SelectBlockElementsAction::Undo(DisplayDirector* director)
{
	director->SetSelection(NULL);

	Action::Undo(director);
}


