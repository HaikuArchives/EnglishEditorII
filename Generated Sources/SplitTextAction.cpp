/* SplitTextAction.cpp */

#include "SplitTextAction.h"
#include "Text.h"
#include "Document.h"
#include "DisplayDirector.h"



SplitTextAction::SplitTextAction(Text* textIn, unsigned long splitOffsetIn)
	: text(textIn), splitOffset(splitOffsetIn)
{
	newText = text->OwnerDocument()->CreateTextNode("");
}


SplitTextAction::~SplitTextAction()
{
	if (!done)
		delete newText;
}


void SplitTextAction::Do(DisplayDirector* director)
{
	// copy characters to the new text
	unsigned long splitLen = text->Length() - splitOffset;
	if (splitLen > 0) {
		newText->AppendData(text->SubstringData(splitOffset, splitLen));
		text->DeleteData(splitOffset, splitLen);
		}

	// insert the new text node
	text->ParentNode()->InsertBefore(newText, text->NextSibling());

	Action::Do(director);
}


void SplitTextAction::Undo(DisplayDirector* director)
{
	director->SetSelection(NULL);

	// remove the new node
	text->ParentNode()->RemoveChild(newText);

	// copy the text back
	text->AppendData(newText->Data());
	newText->SetData("");

	Action::Undo(director);
}


Text* SplitTextAction::GetNewText()
{
	return newText;
}


