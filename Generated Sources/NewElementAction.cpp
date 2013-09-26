/* NewElementAction.cpp */

#include "NewElementAction.h"
#include "Element.h"
#include "Document.h"
#include "Text.h"
#include "TextDisplayNode.h"
#include "ElementDisplayNode.h"
#include "DisplayDirector.h"
#include "BetweenGlyphsSelection.h"
#include "AddCharsAction.h"
#include "InternalException.h"



NewElementAction::NewElementAction(DOMString tagName, Node* parentNodeIn, Node* beforeNodeIn)
	: parentNode(parentNodeIn), beforeNode(beforeNodeIn)
{
	Document* document = parentNode->OwnerDocument();
	newElement = document->CreateElement(tagName);
//***	newElement->AppendChild(document->CreateTextNode(""));
}


NewElementAction::~NewElementAction()
{
	if (!done)
		delete newElement;
}


Element* NewElementAction::GetNewElement()
{
	return newElement;
}


void NewElementAction::Do(DisplayDirector* director)
{
	// add the new element
	parentNode->InsertBefore(newElement, beforeNode);

	// select
/***
	Text* newText = dynamic_cast<Text*>(newElement->FirstChild());
	if (useAssertions && newText == NULL)
		throw InternalException("NewElementAction::Do: new Element lost its Text child.");
	TextDisplayNode* displayNode =
		dynamic_cast<TextDisplayNode*>(director->DisplayNodeFor(newText));
	if (useAssertions && displayNode == NULL)
		throw InternalException("NewElementAction::Do: no TextDisplayNode for the new text.");
	director->SetSelection(displayNode->EndSelection());
***/
	DisplayNode* displayNode = director->DisplayNodeFor(newElement);
	if (displayNode)
		director->SetSelection(displayNode->GetSelection());
/***
	ElementDisplayNode* elementNode =
		dynamic_cast<ElementDisplayNode*>(director->DisplayNodeFor(newElement));
	if (useAssertions && elementNode == NULL)
		throw InternalException("NewElementAction::Do: Can't get the new element's display node.");
	director->SetSelection(elementNode->EndSelection());
***/
/*** NOT TRIED (EndSelection() was quicker)
	PlaceholderTextDisplayNode* textNode =
		dynamic_cast<PlaceholderTextDisplayNode*>(elementNode->FirstChild());
	if (useAssertions && textNode == NULL)
		throw InternalException("NewElementAction::Do: Placeholder is missing!");
***/

	Action::Do(director);
}


void NewElementAction::Undo(DisplayDirector* director)
{
	director->SetSelection(NULL);

	// remove the node
	parentNode->RemoveChild(newElement);

	Action::Undo(director);
}


bool NewElementAction::CanIncorporateNext(Action* nextActionIn)
{
/***
	// merge with another AddCharsAction if it's at the end of this one
	AddCharsAction* nextAction = dynamic_cast<AddCharsAction*>(nextActionIn);
	if (nextAction) {
		Text* newText = dynamic_cast<Text*>(newElement->FirstChild());
		if (useAssertions && newText == NULL)
			throw InternalException("NewElementAction::CanIncorporateNext: new Element lost its Text child.");
		return (nextAction->GetText() == newText &&
		        nextAction->AtChar() == newText->Length() - nextAction->Length());
		}
***/

	return false;
}


void NewElementAction::IncorporateNext(Action* nextAction)
{
	// nothing to do--"nextAction" is an AddCharsAction and the characters
	// are already in the new Text
}


