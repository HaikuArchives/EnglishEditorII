/* CompositeAction.h */

#ifndef _H_CompositeAction_
#define _H_CompositeAction_

#include "Action.h"

class CompositeAction : public Action {
public:
	CompositeAction();
	~CompositeAction();
	void	AddAction(Action* action);
	void	Do(DisplayDirector* director);
	void	Undo(DisplayDirector* director);
	bool	CanIncorporateNext(Action* nextAction);
	void	IncorporateNext(Action* nextAction);
	bool	IsNulled();

protected:
	Action*	firstAction;
	Action*	lastAction;
};


#endif
