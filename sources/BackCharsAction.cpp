/* BackCharsAction.cpp */

#include "BackCharsAction.h"
#include "Text.h"
#include "DisplayDirector.h"
#include "TextDisplayNode.h"
#include "UTF8Utils.h"



BackCharsAction::BackCharsAction(Text* textIn, int whichCharIn, int numBytes)
	: text(textIn), whichChar(whichCharIn)
{
	// store it into "chars", accounting for multi-byte UTF8 characters
	DOMString textData = text->Data();
	if (numBytes == 1) {
		// asking for deletion of one character; make sure to get all the bytes
		// of that character
		string_slice theChar = textData.substr(whichChar, 1);
		numBytes = UTF8Utils::BytesForChar(*theChar.begin());
		}
	chars = textData.substr(whichChar, numBytes);
}


BackCharsAction::~BackCharsAction()
{
}


void BackCharsAction::Do(DisplayDirector* director)
{
	director->SetSelection(NULL);

	// delete character(s)
	text->DeleteData(whichChar, chars.length());

	// move selection
	TextDisplayNode* displayNode =
		dynamic_cast<TextDisplayNode*>(director->DisplayNodeFor(text));
	if (displayNode)
		director->SetSelection(displayNode->GetSelectionAtOffset(whichChar));

	Action::Do(director);
}


void BackCharsAction::Undo(DisplayDirector* director)
{
	// re-insert the character
	text->InsertData(whichChar, DOMString(chars));

	// move selection
	TextDisplayNode* displayNode =
		dynamic_cast<TextDisplayNode*>(director->DisplayNodeFor(text));
	if (displayNode)
		director->SetSelection(displayNode->GetSelectionAtOffset(whichChar + chars.length()));

	Action::Undo(director);
}


bool BackCharsAction::CanIncorporateNext(Action* nextAction)
{
	// merge with a BackCharAction if it's right before or after what we delete
	BackCharsAction* backAction = dynamic_cast<BackCharsAction*>(nextAction);
	if (backAction) {
		if (backAction->text != text)
			return false;
		if (backAction->whichChar + backAction->chars.length() == whichChar)
			return true;
		if (whichChar == backAction->whichChar) {
			// this test comes *after* we've done our deletion
			return true;
			}
		}

	return false;
}


void BackCharsAction::IncorporateNext(Action* nextAction)
{
	// incorporating BackCharAction
	BackCharsAction* backAction = dynamic_cast<BackCharsAction*>(nextAction);
	if (backAction) {
		// add to ours
		if (backAction->whichChar + backAction->chars.length() == whichChar) {
			// backAction is a backwards delete from us
			whichChar -= backAction->chars.length();
			qstring newChars = backAction->chars;
			newChars += chars;
			chars = newChars;
			}
		else {
			// backAction is a forwards delete from us
			chars += backAction->chars;
			}
		}
}


Text* BackCharsAction::GetText()
{
	return text;
}


int BackCharsAction::WhichChar()
{
	return whichChar;
}


int BackCharsAction::NumChars()
{
	return chars.length();
}


