/* RestoreSelectionAction.cpp */

#include "RestoreSelectionAction.h"
#include "Selection.h"
#include "DisplayDirector.h"



RestoreSelectionAction::RestoreSelectionAction(Action* wrappedActionIn)
	: restoreAction(NULL), wrappedAction(wrappedActionIn)
{
}


RestoreSelectionAction::~RestoreSelectionAction()
{
	delete restoreAction;
	delete wrappedAction;
}


void RestoreSelectionAction::Do(DisplayDirector* director)
{
	// set up to restore the selection
	delete restoreAction;
	Selection* selection = director->GetSelection();
	restoreAction = (selection ? selection->GetRestoreAction() : NULL);
	if (restoreAction)
		restoreAction->Do(director);

	// do the wrappedAction
	if (wrappedAction)
		wrappedAction->Do(director);

	Action::Do(director);
}


void RestoreSelectionAction::Undo(DisplayDirector* director)
{
	// undo the wrappedAction
	if (wrappedAction)
		wrappedAction->Undo(director);

	// restore the selection
	if (restoreAction)
		restoreAction->Undo(director);
	else
		director->SetSelection(NULL);

	Action::Undo(director);
}


