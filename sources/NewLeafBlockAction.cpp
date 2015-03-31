/* NewLeafBlockAction.cpp */

#include "NewLeafBlockAction.h"
#include "Element.h"
#include "Text.h"
#include "Document.h"
#include "DisplayDirector.h"
#include "BetweenGlyphsSelection.h"
#include "TextDisplayNode.h"
#include "RestoreSelectionAction.h"
#include "InternalException.h"



NewLeafBlockAction::NewLeafBlockAction(Element* afterElementIn, DOMString tagName)
	: afterElement(afterElementIn)
{
	Document* document = afterElement->OwnerDocument();
	if (tagName.empty())
		tagName = afterElement->TagName();
	newElement = document->CreateElement(tagName);
	Text* newText = document->CreateTextNode("");
	newElement->AppendChild(newText);
}


NewLeafBlockAction::~NewLeafBlockAction()
{
	if (!done) {
		delete newElement->RemoveChild(newElement->FirstChild());
		delete newElement;		
		}
}


void NewLeafBlockAction::Do(DisplayDirector* director)
{
	// add the new leaf block
	afterElement->ParentNode()->InsertBefore(newElement, afterElement->NextSibling());

	// select
	Text* text = dynamic_cast<Text*>(newElement->FirstChild());
	if (useAssertions && text == NULL)
		throw InternalException("NewLeafBlockAction::Do: New element's text disappeared!");
	TextDisplayNode* textDisplayNode =
		dynamic_cast<TextDisplayNode*>(director->DisplayNodeFor(text));
	if (useAssertions && textDisplayNode == NULL)
		throw InternalException("NewLeafBlockAction::Do: Didn't get DisplayNode for the new text.");
	director->SetSelection(new BetweenGlyphsSelection(textDisplayNode, NULL, NULL, false));

	Action::Do(director);
}


void NewLeafBlockAction::Undo(DisplayDirector* director)
{
	director->SetSelection(NULL);

	afterElement->ParentNode()->RemoveChild(newElement);

	Action::Undo(director);
}


