/* EmptyPlaceholderSelection.cpp */

#include "EmptyPlaceholderSelection.h"
#include "Element.h"
#include "Text.h"
#include "PlaceholderTextDisplayNode.h"
#include "ElementDisplayNode.h"
#include "DisplayDirector.h"
#include "CompositeAction.h"
#include "NewTextAction.h"
#include "MoveNodesAction.h"
#include "AddCharsAction.h"
#include "ChangeElementTagNameAction.h"
#include "RestoreSelectionAction.h"
#include "RestoreEmptyPlaceholderSelectionAction.h"
#include "Keys.h"
#include "InternalException.h"


EmptyPlaceholderSelection::EmptyPlaceholderSelection(PlaceholderTextDisplayNode* textNodeIn)
	: BetweenGlyphsSelection(textNodeIn, NULL, NULL, false)
{
}


void EmptyPlaceholderSelection::Draw(View* view, CoordPoint origin)
{
	// create the tagEditor if there isn't one yet
	// we'd do this in the ctor, except stupid C++ can't call derived virtual
	//		functions from the ctor
	if (tagEditor == NULL)
		ShowTag(textNode->GetDisplayDirector());

	BetweenGlyphsSelection::Draw(view, origin);
}


void EmptyPlaceholderSelection::AcceptKey(string_slice key, DisplayDirector* director)
{
	if (TagIsSelected() || key == "\t")
		PointSelection::AcceptKey(key, director);
	else if (key == "\b" || key == Keys::DelKey) {
		/***/
		}
	else if (key == "\n") {
		/***/
		}
	else if (key == Keys::RightArrowKey)
		SelectForward(director);
	else if (key == Keys::LeftArrowKey)
		SelectBackward(director);
	else {
		// add contents to the element
		Element* element = GetElement();
		CompositeAction* action = new CompositeAction();
		action->AddAction(new RestoreSelectionAction());
		NewTextAction* newTextAction = new NewTextAction(element->OwnerDocument());
		action->AddAction(newTextAction);
		Text* newText = newTextAction->GetText();
		action->AddAction(new MoveNodesAction(newText, newText, element, NULL));
		action->AddAction(new AddCharsAction(key, newText, 0));
		director->DoAction(action);
		}
}


Action* EmptyPlaceholderSelection::GetRestoreAction()
{
	return new RestoreEmptyPlaceholderSelectionAction(GetElement());
}


void EmptyPlaceholderSelection::Paste(String pasteText, DisplayDirector* director)
{
	// add pasteText to the element
	Element* element = GetElement();
	CompositeAction* action = new CompositeAction();
	action->AddAction(new RestoreSelectionAction());
	NewTextAction* newTextAction = new NewTextAction(element->OwnerDocument());
	action->AddAction(newTextAction);
	Text* newText = newTextAction->GetText();
	action->AddAction(new MoveNodesAction(newText, newText, element, NULL));
	action->AddAction(new AddCharsAction(pasteText, newText, 0));
	director->DoAction(action);
}


void EmptyPlaceholderSelection::InsertTag(DOMString tagName, DisplayDirector* director)
{
	HideTag(director);	// for safety
	ChangeElementTagNameAction* changeTagAction =
		new ChangeElementTagNameAction(GetElement(), tagName);
	director->DoAction(new RestoreSelectionAction(changeTagAction));
}


DOMString EmptyPlaceholderSelection::TagName()
{
	return GetElement()->TagName();
}


Element* EmptyPlaceholderSelection::GetElement()
{
	for (DisplayNode* node = textNode->Parent(); node; node = node->Parent()) {
		ElementDisplayNode* elementNode = dynamic_cast<ElementDisplayNode*>(node);
		if (elementNode)
			return elementNode->GetElement();
		}
	if (useAssertions)
		throw InternalException("EmptyPlaceholderSelection: parent isn't an ElementDisplayNode.");
	return NULL;
}


