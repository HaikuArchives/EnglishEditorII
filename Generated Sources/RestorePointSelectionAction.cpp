/* RestorePointSelectionAction.cpp */

#include "RestorePointSelectionAction.h"
#include "Text.h"
#include "TextDisplayNode.h"
#include "DisplayDirector.h"
#include "InternalException.h"



RestorePointSelectionAction::RestorePointSelectionAction(Text* textIn, unsigned long offsetIn)
	: text(textIn), offset(offsetIn)
{
}


void RestorePointSelectionAction::Do(DisplayDirector* director)
{
	director->SetSelection(NULL);
	Action::Do(director);
}


void RestorePointSelectionAction::Undo(DisplayDirector* director)
{
	// reselect
	TextDisplayNode* displayNode =
		dynamic_cast<TextDisplayNode*>(director->DisplayNodeFor(text));
	if (displayNode == NULL) {
		if (useAssertions)
			throw InternalException("RestorePointSelectionAction::Undo: couldn't get display node.");
		return;
		}
	director->SetSelection(displayNode->GetSelectionAtOffset(offset));

	Action::Undo(director);
}


