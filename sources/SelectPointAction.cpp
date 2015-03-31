/* SelectPointAction.cpp */

#include "SelectPointAction.h"
#include "DisplayDirector.h"
#include "TextDisplayNode.h"
#include "Text.h"
#include "InternalException.h"



SelectPointAction::SelectPointAction(Text* textIn, unsigned long offsetIn)
	: text(textIn), offset(offsetIn)
{
}


void SelectPointAction::Do(DisplayDirector* director)
{
	TextDisplayNode* displayNode =
		dynamic_cast<TextDisplayNode*>(director->DisplayNodeFor(text));
	if (displayNode == NULL)
		throw InternalException("SelectPointAction::Do: couldn't get display node.");
	director->SetSelection(displayNode->GetSelectionAtOffset(offset));

	Action::Do(director);
}


void SelectPointAction::Undo(DisplayDirector* director)
{
	director->SetSelection(NULL);
	Action::Undo(director);
}


