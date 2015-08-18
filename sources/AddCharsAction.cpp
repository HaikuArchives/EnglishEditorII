/* AddCharsAction.cpp */

#include "AddCharsAction.h"
#include "Text.h"
#include "DisplayDirector.h"
#include "TextDisplayNode.h"
#include "BackCharsAction.h"



AddCharsAction::AddCharsAction(string_slice charsIn, Text* textIn, int atCharIn)
	: chars(charsIn), text(textIn), atChar(atCharIn)
{
}


void AddCharsAction::Do(DisplayDirector* director)
{
	director->SetSelection(NULL);

	text->InsertData(atChar, String(chars));

	// move the selection
	TextDisplayNode* displayNode =
		dynamic_cast<TextDisplayNode*>(director->DisplayNodeFor(text));
	if (displayNode)
		director->SetSelection(displayNode->GetSelectionAtOffset(atChar + chars.length()));

	Action::Do(director);
}


void AddCharsAction::Undo(DisplayDirector* director)
{
	director->SetSelection(NULL);

	text->DeleteData(atChar, chars.length());

	// reset the selection
	TextDisplayNode* displayNode =
		dynamic_cast<TextDisplayNode*>(director->DisplayNodeFor(text));
	if (displayNode)
		director->SetSelection(displayNode->GetSelectionAtOffset(atChar));

	Action::Undo(director);
}


bool AddCharsAction::CanIncorporateNext(Action* nextActionIn)
{
	// merge with another AddCharsAction if it's at the end of this one
	AddCharsAction* nextAction = dynamic_cast<AddCharsAction*>(nextActionIn);
	if (nextAction) {
		return (nextAction->text == text &&
		        nextAction->atChar == atChar + chars.length());
		}

	// merge with a BackCharAction if it's at the end of what we've added
	BackCharsAction* backAction = dynamic_cast<BackCharsAction*>(nextActionIn);
	if (backAction) {
		if (backAction->GetText() != text)
			return false;
		int numAddedChars = chars.length();
		int numDeletedChars = backAction->NumChars();
		int whichChar = backAction->WhichChar();
		return (whichChar + numDeletedChars == atChar + numAddedChars &&
		        numDeletedChars <= numAddedChars);
		}

	return false;
}


void AddCharsAction::IncorporateNext(Action* nextActionIn)
{
	// incorporating AddCharsAction
	AddCharsAction* nextAction = dynamic_cast<AddCharsAction*>(nextActionIn);
	if (nextAction) {
		chars += nextAction->chars;
		}

	// incorporating BackCharAction
	BackCharsAction* backAction = dynamic_cast<BackCharsAction*>(nextActionIn);
	if (backAction) {
		// delete the last characters
		qstring newChars =
			string_slice(chars).substr(0, chars.length() - backAction->NumChars());
		chars = newChars;
		}
}


bool AddCharsAction::IsNulled()
{
	return (chars.length() == 0);
}


Text* AddCharsAction::GetText()
{
	return text;
}


int AddCharsAction::AtChar()
{
	return atChar;
}


int AddCharsAction::Length()
{
	return chars.length();
}


