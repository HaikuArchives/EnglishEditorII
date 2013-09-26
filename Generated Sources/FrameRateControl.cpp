/* FrameRateControl.cpp */

#include "FrameRateControl.h"
#include "WindowDirector.h"
#include "View.h"
#include "Font.h"
#include <stdio.h>

// values for "displaying"
enum {
	DisplayOff,
	DisplayFrameRate,
	DisplayDrawTime,

	NumDisplayChoices
};

const int FrameRateControl::xPos = 10;
const int FrameRateControl::yPos = 3;


FrameRateControl::FrameRateControl(WindowDirector* directorIn)
	: director(directorIn), displaying(DisplayOff)
{
	font = new Font("Dutch801 Rm BT", 12, Font::Normal);
}


FrameRateControl::~FrameRateControl()
{
	delete font;
}


Rectangle FrameRateControl::GetRect()
{
	static const char* maxString = "00.0d/0.00s/0.00b";

	Rectangle viewBounds = director->ViewBounds();
	Rectangle rect;
	rect.left = viewBounds.left + xPos;
	rect.right = rect.left + font->WidthOf(string_slice(maxString));
	rect.bottom = viewBounds.bottom - yPos;
	rect.top = rect.bottom - font->LineHeight();
	return rect;
}


void FrameRateControl::Draw(DisplayDirector* directorIn)
{
	if (displaying == DisplayOff)
		return;

	View* view = director->DrawingView();
	view->PushState();
	view->SetHighColor(controlColor);

	// create the string
	char str[64];
	if (displaying == DisplayFrameRate) {
		Microseconds frameTime = director->LastScrollFrameTime();
		if (frameTime == 0)
			strcpy(str, "----");
		else
			sprintf(str, "%2.1f fps", 1000000.0 / frameTime);
		}
	else /* displaying == DisplayDrawTime */ {
		sprintf(str, "%2.1fd/%2.1fb",
		        director->LastDrawTime() / 1000.0,
		        director->LastBlitTime() / 1000.0);
		}

	// draw it
	Rectangle rect = GetRect();
	view->SetFont(font);
	view->DrawString(str, rect.left, rect.top + font->Ascent());

	view->PopState();
}


void FrameRateControl::MouseDown(int x, int y, DisplayDirector* director)
{
	displaying++;
	if (displaying >= NumDisplayChoices)
		displaying = DisplayOff;
	director->RefreshViewRect(GetRect());
}


