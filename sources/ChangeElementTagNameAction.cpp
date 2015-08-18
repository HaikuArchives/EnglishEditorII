/* ChangeElementTagNameAction.cpp */

#include "ChangeElementTagNameAction.h"
#include "Element.h"
#include "Document.h"
#include "DisplayDirector.h"
#include "DisplayNode.h"



ChangeElementTagNameAction::ChangeElementTagNameAction(Element* element, String newTagName)
	: oldElement(element)
{
	newElement = oldElement->OwnerDocument()->CreateElement(newTagName);
}


ChangeElementTagNameAction::~ChangeElementTagNameAction()
{
	if (!done)
		delete newElement;
}


void ChangeElementTagNameAction::Do(DisplayDirector* director)
{
	director->SetSelection(NULL);

	// replace the element
	oldElement->ParentNode()->ReplaceChild(newElement, oldElement);

	// move the child nodes
	for (Node* child = oldElement->FirstChild(); child; child = oldElement->FirstChild())
		newElement->AppendChild(oldElement->RemoveChild(child));

	// select
	DisplayNode* displayNode = director->DisplayNodeFor(newElement);
	if (displayNode)
		director->SetSelection(displayNode->GetSelection());

	Action::Do(director);
}


void ChangeElementTagNameAction::Undo(DisplayDirector* director)
{
	director->SetSelection(NULL);

	// restore the old element
	newElement->ParentNode()->ReplaceChild(oldElement, newElement);

	// move the child nodes
	for (Node* child = newElement->FirstChild(); child; child = newElement->FirstChild())
		oldElement->AppendChild(newElement->RemoveChild(child));

	Action::Undo(director);
}


