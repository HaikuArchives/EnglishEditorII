/* StatusControl.cpp */

#include "StatusControl.h"
#include "WindowDirector.h"
#include "MessageFileSource.h"
#include "Font.h"
#include "View.h"

const int StatusControl::xPos = 10;
const int StatusControl::yPos = 10;
const char* StatusControl::changedStr = "Changed";
const char* StatusControl::unsentStr = "Unsent";
const char* StatusControl::sentStr = "Sent";


StatusControl::StatusControl(WindowDirector* directorIn)
	: director(directorIn)
{
	font = new Font("Swis721 BT", 10, Font::Bold);
}


StatusControl::~StatusControl()
{
	delete font;
}


Rectangle StatusControl::GetRect()
{
	Rectangle viewBounds = director->ViewBounds();
	Rectangle rect;
	rect.right = viewBounds.right - xPos;
	rect.left = rect.right - font->WidthOf(string_slice(changedStr));
	rect.top = viewBounds.top + yPos;
	rect.bottom = rect.top + font->LineHeight();
	return rect;
}


void StatusControl::Draw(DisplayDirector* director)
{
	View* view = director->DrawingView();
	view->PushState();
	view->SetHighColor(controlColor);
	Rectangle rect = GetRect();
	view->SetFont(font);
	int y = (int) (rect.top + font->Ascent());
	int lineHeight = font->LineHeight();

	// draw changed indicator
	if (director->IsDirty())
		view->DrawString(string_slice(changedStr), rect.left, y);
	y += lineHeight;

	// draw mail indicator
	MessageFileSource* messageSource =
		dynamic_cast<MessageFileSource*>(director->GetDocSource());
	if (messageSource) {
		if (messageSource->IsNewMessage()) {
			const char* msg = (messageSource->IsSent() ? sentStr : unsentStr);
			view->DrawString(string_slice(msg), rect.left, y);
			}
		}

	view->PopState();
}


void StatusControl::MouseDown(int x, int y, DisplayDirector* director)
{
	// does nothing
}


