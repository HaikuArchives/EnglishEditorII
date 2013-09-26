/* Action.cpp */

#include "Action.h"

#ifndef NULL
#define NULL	(0)
#endif



Action::Action()
	: next(NULL), prev(NULL), done(false)
{
}


Action::~Action()
{
}


void Action::Do(DisplayDirector* director)
{
	/* pure virtual, inherit to set "done" */
	done = true;
}


void Action::Undo(DisplayDirector* director)
{
	/* pure virtual, inherit to set "done" */
	done = false;
}


bool Action::CanIncorporateNext(Action* nextAction)
{
	// default: no
	return false;
}


void Action::IncorporateNext(Action* nextAction)
{
	// default: do nothing, since CanIncorporateNext() defaults to "false"
}


bool Action::IsNulled()
{
	// IsNulled() is only meaningful immediately after IncorporateNext();
	// it means the consolidated action has no effect and should be
	// deleted from action list.
	return false;
}


void Action::InsertAfter(Action* prevAction)
{
	Unlink();

	next = prevAction->next;
	prev = prevAction;
	if (prevAction->next)
		prevAction->next->prev = this;
	prevAction->next = this;
}


void Action::Unlink()
{
	if (prev)
		prev->next = next;
	if (next)
		next->prev = prev;
	next = prev = NULL;
}


Action* Action::NextAction()
{
	return next;
}


Action* Action::PrevAction()
{
	return prev;
}


void Action::DeleteRest()
{
	// delete the actions
	Action* nextAction = next;
	for (Action* action = nextAction; action; action = nextAction) {
		nextAction = action->next;
		delete action;
		}

	// mark this as the end
	next = NULL;
}


void Action::DeleteAll()
{
	DeleteRest();

	// delete the actions
	Action* prevAction = prev;
	for (Action* action = prevAction; action; action = prevAction) {
		prevAction = action->prev;
		delete action;
		}

	// delete this too
	delete this;
}


