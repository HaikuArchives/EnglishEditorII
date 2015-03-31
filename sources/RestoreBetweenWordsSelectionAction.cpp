/* RestoreBetweenWordsSelectionAction.cpp */

#include "RestoreBetweenWordsSelectionAction.h"
#include "TextDisplayNode.h"
#include "Space.h"
#include "Text.h"
#include "BetweenWordsSelection.h"
#include "DisplayDirector.h"
#include "InternalException.h"



RestoreBetweenWordsSelectionAction::RestoreBetweenWordsSelectionAction(Text* textIn, unsigned long offsetIn)
	: text(textIn), offset(offsetIn)
{
}


void RestoreBetweenWordsSelectionAction::Do(DisplayDirector* director)
{
	director->SetSelection(NULL);
	Action::Do(director);
}


void RestoreBetweenWordsSelectionAction::Undo(DisplayDirector* director)
{
	TextDisplayNode* displayNode =
		dynamic_cast<TextDisplayNode*>(director->DisplayNodeFor(text));
	if (displayNode == NULL)
		throw InternalException("RestoreBetweenWordsSelectionAction::Do: couldn't get TextDisplayNode.");
	Space* space = dynamic_cast<Space*>(displayNode->GlyphAtOffset(offset));
	if (space == NULL)
		throw InternalException("RestoreBetweenWordsSelectionAction::Do: couldn't get Space.");
	director->SetSelection(new BetweenWordsSelection(displayNode, space));

	Action::Undo(director);
}


