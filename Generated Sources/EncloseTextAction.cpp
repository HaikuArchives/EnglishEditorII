/* EncloseTextAction.cpp */

#include "EncloseTextAction.h"
#include "Text.h"
#include "Element.h"
#include "Document.h"
#include "ElementDisplayNode.h"
#include "DisplayDirector.h"
#include "InternalException.h"



EncloseTextAction::EncloseTextAction(Text* textIn, DOMString tagNameIn)
	: text(textIn), tagName(tagNameIn)
{
}


void EncloseTextAction::Do(DisplayDirector* director)
{
	director->SetSelection(NULL);

	// remove the text from its parent
	Node* nextNode = text->NextSibling();
	Node* parent = text->ParentNode();
	parent->RemoveChild(text);

	// make the new element and add it in
	Element* element = parent->OwnerDocument()->CreateElement(tagName);
	element->AppendChild(text);
	parent->InsertBefore(element, nextNode);

	// select the new element
	ElementDisplayNode* displayNode =
		dynamic_cast<ElementDisplayNode*>(director->DisplayNodeFor(element));
	if (displayNode)
		director->SetSelection(displayNode->GetSelection());

	Action::Do(director);
}


void EncloseTextAction::Undo(DisplayDirector* director)
{
	director->SetSelection(NULL);

	// set up
	Element* element = dynamic_cast<Element*>(text->ParentNode());
	if (useAssertions && element == NULL)
		throw InternalException("EncloseTextAction::Undo: the text lost its parent element!");
	Node* parent = element->ParentNode();
	Node* nextNode = element->NextSibling();

	// remove the element
	parent->RemoveChild(element);

	// move the text back into place
	element->RemoveChild(text);
	parent->InsertBefore(text, nextNode);

	Action::Undo(director);
}


