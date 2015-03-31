/* RestoreBlockElementSelectionAction.cpp */

#include "RestoreBlockElementSelectionAction.h"
#include "Element.h"
#include "BlockElementSelection.h"
#include "DisplayDirector.h"
#include "ElementDisplayNode.h"
#include "InternalException.h"



RestoreBlockElementSelectionAction::RestoreBlockElementSelectionAction(Element* elementIn)
	: element(elementIn)
{
}


void RestoreBlockElementSelectionAction::Do(DisplayDirector* director)
{
	director->SetSelection(NULL);
	Action::Do(director);
}


void RestoreBlockElementSelectionAction::Undo(DisplayDirector* director)
{
	ElementDisplayNode* displayNode =
		dynamic_cast<ElementDisplayNode*>(director->DisplayNodeFor(element));
	if (displayNode == NULL)
		throw InternalException("RestoreBlockElementSelectAction::Do: couldn't get ElementDisplayNode.");

	director->SetSelection(new BlockElementSelection(displayNode));

	Action::Undo(director);
}


