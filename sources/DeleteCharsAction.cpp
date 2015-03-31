/* DeleteCharsAction.cpp */

#include "DeleteCharsAction.h"
#include "Text.h"



DeleteCharsAction::DeleteCharsAction(Text* textIn, unsigned long offsetIn, unsigned long lengthIn)
	: text(textIn), offset(offsetIn), length(lengthIn)
{
}


void DeleteCharsAction::Do(DisplayDirector* director)
{
	// save the text for undo
	data = text->SubstringData(offset, length);
	data.detach();

	// delete it
	text->DeleteData(offset, length);

	Action::Do(director);
}


void DeleteCharsAction::Undo(DisplayDirector* director)
{
	text->InsertData(offset, data);

	Action::Undo(director);
}


