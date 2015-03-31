/* NewTextAction.cpp */

#include "NewTextAction.h"
#include "Document.h"
#include "Text.h"



NewTextAction::NewTextAction(Document* document)
{
	text = document->CreateTextNode("");
}


NewTextAction::~NewTextAction()
{
	if (!done)
		delete text;
}


void NewTextAction::Do(DisplayDirector* director)
{
	// nothing to actually do; if the Text is to be inserted, another Action will do it

	Action::Do(director);
}


void NewTextAction::Undo(DisplayDirector* director)
{
	// nothing to actually undo

	Action::Undo(director);
}


Text* NewTextAction::GetText()
{
	return text;
}


