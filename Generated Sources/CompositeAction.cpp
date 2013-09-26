/* CompositeAction.cpp */

#include "CompositeAction.h"

#ifndef NULL
#define NULL	(0)
#endif



CompositeAction::CompositeAction()
	: firstAction(NULL), lastAction(NULL)
{
}


CompositeAction::~CompositeAction()
{
	Action* nextAction = NULL;
	for (Action* action = firstAction; action; action = nextAction) {
		nextAction = action->NextAction();
		delete action;
		}
}


void CompositeAction::AddAction(Action* action)
{
	if (lastAction)
		action->InsertAfter(lastAction);
	lastAction = action;
	if (firstAction == NULL)
		firstAction = action;
}


void CompositeAction::Do(DisplayDirector* director)
{
	for (Action* action = firstAction; action; action = action->NextAction())
		action->Do(director);
}


void CompositeAction::Undo(DisplayDirector* director)
{
	for (Action* action = lastAction; action; action = action->PrevAction())
		action->Undo(director);
}


bool CompositeAction::CanIncorporateNext(Action* nextAction)
{
	return (lastAction && lastAction->CanIncorporateNext(nextAction));
}


void CompositeAction::IncorporateNext(Action* nextAction)
{
	lastAction->IncorporateNext(nextAction);

	// delete a nulled action
	if (lastAction->IsNulled()) {
		Action* penultimateAction = lastAction->PrevAction();
		delete lastAction;
		lastAction = penultimateAction;
		if (lastAction == NULL)
			firstAction = NULL;
		}
}


bool CompositeAction::IsNulled()
{
	return (firstAction == NULL);
}


