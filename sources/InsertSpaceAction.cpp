/* InsertSpaceAction.cpp */

#include "InsertSpaceAction.h"
#include "Text.h"



InsertSpaceAction::InsertSpaceAction(Text* textIn, unsigned long offsetIn)
	: text(textIn), offset(offsetIn)
{
}


void InsertSpaceAction::Do(DisplayDirector* director)
{
	// insert
	text->InsertData(offset, " ");

	Action::Do(director);
}


void InsertSpaceAction::Undo(DisplayDirector* director)
{
	text->DeleteData(offset, 1);

	Action::Undo(director);
}


unsigned long InsertSpaceAction::SpaceLength()
{
	return 1;
}


