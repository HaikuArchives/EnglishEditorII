/* RestoreSelectionAction.h */

#ifndef _H_RestoreSelectionAction_
#define _H_RestoreSelectionAction_

#include "Action.h"

#ifndef NULL
#define NULL	(0)
#endif

class RestoreSelectionAction : public Action {
public:
	RestoreSelectionAction(Action* wrappedActionIn = NULL);
	~RestoreSelectionAction();
	void	Do(DisplayDirector* director);
	void	Undo(DisplayDirector* director);

protected:
	Action*	restoreAction;
	Action*	wrappedAction;
};


#endif
