/* RestoreEmptyPlaceholderSelectionAction.cpp */

#include "RestoreEmptyPlaceholderSelectionAction.h"
#include "Element.h"
#include "ElementDisplayNode.h"
#include "PlaceholderTextDisplayNode.h"
#include "EmptyPlaceholderSelection.h"
#include "DisplayDirector.h"
#include "InternalException.h"



RestoreEmptyPlaceholderSelectionAction::RestoreEmptyPlaceholderSelectionAction(Element* elementIn)
	: element(elementIn)
{
}


void RestoreEmptyPlaceholderSelectionAction::Do(DisplayDirector* director)
{
	director->SetSelection(NULL);
	Action::Do(director);
}


void RestoreEmptyPlaceholderSelectionAction::Undo(DisplayDirector* director)
{
	ElementDisplayNode* elementNode =
		dynamic_cast<ElementDisplayNode*>(director->DisplayNodeFor(element));
	if (useAssertions && elementNode == NULL)
		throw InternalException("RestoreEmptyPlaceholderSelectionAction::Undo: Couldn't get ElementDisplayNode.");
	PlaceholderTextDisplayNode* placeholder = FindPlaceholderIn(elementNode);
/***
	PlaceholderTextDisplayNode* placeholder =
		dynamic_cast<PlaceholderTextDisplayNode*>(elementNode->FirstChild());
***/
	if (useAssertions && placeholder == NULL)
		throw InternalException("RestoreEmptyPlaceholderSelectionAction::Undo: Couldn't get the PlaceholderTextDisplayNode.");
	director->SetSelection(new EmptyPlaceholderSelection(placeholder));

	Action::Undo(director);
}


PlaceholderTextDisplayNode* RestoreEmptyPlaceholderSelectionAction::FindPlaceholderIn(DisplayNode* node)
{
	for (DisplayNode* child = node->FirstChild(); child; child = child->NextSibling()) {
		// see if the child is the placeholder
		PlaceholderTextDisplayNode* placeholder =
			dynamic_cast<PlaceholderTextDisplayNode*>(child);
		if (placeholder)
			return placeholder;

		// see if the placeholder is inside the child
		placeholder = FindPlaceholderIn(child);
		if (placeholder)
			return placeholder;
		}

	return NULL;
}


