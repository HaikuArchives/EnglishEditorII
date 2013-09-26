/* CopyCharsAction.cpp */

#include "CopyCharsAction.h"
#include "Text.h"



CopyCharsAction::CopyCharsAction(Text* srcTextIn, unsigned long srcOffsetIn, unsigned long lengthIn, Text* destTextIn, unsigned long destOffsetIn)
	: srcText(srcTextIn), srcOffset(srcOffsetIn), length(lengthIn),
	  destText(destTextIn), destOffset(destOffsetIn)
{
}


void CopyCharsAction::Do(DisplayDirector* director)
{
	destText->InsertData(destOffset, srcText->SubstringData(srcOffset, length));

	Action::Do(director);
}


void CopyCharsAction::Undo(DisplayDirector* director)
{
	destText->DeleteData(destOffset, length);

	Action::Undo(director);
}


