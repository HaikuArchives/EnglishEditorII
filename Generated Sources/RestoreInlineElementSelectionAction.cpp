/* RestoreInlineElementSelectionAction.cpp */

#include "RestoreInlineElementSelectionAction.h"
#include "Element.h"
#include "ElementDisplayNode.h"
#include "InlineElementSelection.h"
#include "DisplayDirector.h"
#include "InternalException.h"



RestoreInlineElementSelectionAction::RestoreInlineElementSelectionAction()
{
}


void RestoreInlineElementSelectionAction::Do(DisplayDirector* director)
{
	InlineElementSelection* selection =
		dynamic_cast<InlineElementSelection*>(director->GetSelection());
	if (selection == NULL)
		throw InternalException("RestoreInlineElementSelectionAction::Do: no InlineElementSelection.");

	element = selection->GetElement();
	director->SetSelection(NULL);

	Action::Do(director);
}


void RestoreInlineElementSelectionAction::Undo(DisplayDirector* director)
{
	// sanity clause
	if (element == NULL)
		return;

	// restore the selection
	ElementDisplayNode* displayNode =
		dynamic_cast<ElementDisplayNode*>(director->DisplayNodeFor(element));
	if (displayNode == NULL)
		throw InternalException("RestoreInlineElementAction::Undo: no DislayNode.");
	director->SetSelection(new InlineElementSelection(displayNode));

	Action::Undo(director);
}


