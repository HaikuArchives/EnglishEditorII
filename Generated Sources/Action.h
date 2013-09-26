/* Action.h */

#ifndef _H_Action_
#define _H_Action_


class DisplayDirector;

class Action {
public:
	Action();
	virtual~Action();
	virtual void	Do(DisplayDirector* director);
	virtual void	Undo(DisplayDirector* director);
	virtual bool	CanIncorporateNext(Action* nextAction);
	virtual void	IncorporateNext(Action* nextAction);
	virtual bool	IsNulled();
	void        	InsertAfter(Action* prevAction);
	void        	Unlink();
	Action*     	NextAction();
	Action*     	PrevAction();
	void        	DeleteRest();
	void        	DeleteAll();

protected:
	Action*	next;
	Action*	prev;
	bool   	done;
};


#endif
