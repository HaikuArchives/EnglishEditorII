/* MenuControl.cpp */

#include "MenuControl.h"
#include "WindowDirector.h"
#include "MenuDirector.h"
#include "Font.h"
#include "View.h"

const int MenuControl::xPos = 10;
const int MenuControl::yPos = 30;
Color MenuControl::color = { 128, 192, 0, 255 };
const int MenuControl::diameter = 12;
const int MenuControl::lineWidth = 3;
const char* MenuControl::asterisk = "*";
const char* MenuControl::fontFamily = "Dutch801 Rm BT";
const int MenuControl::fontSize = 28;
const int MenuControl::fontStyle = Font::Normal;


MenuControl::MenuControl(WindowDirector* directorIn)
	: director(directorIn)
{
	font = new Font(fontFamily, fontSize, fontStyle);
}


MenuControl::~MenuControl()
{
	delete font;
}


Rectangle MenuControl::GetRect()
{
	Rectangle rect;
	Rectangle viewBounds = director->ViewBounds();
	int lineSpill = (lineWidth + 1) / 2;
	rect.right = viewBounds.right - xPos + lineSpill;
	rect.left = rect.right - diameter - 2 * lineSpill;
	rect.top = viewBounds.top + yPos - lineSpill;
	rect.bottom = rect.top + diameter + 2 * lineSpill;
	return rect;

/***
	Rectangle viewBounds = director->ViewBounds();
	Rectangle rect;
	rect.right = viewBounds.right - xPos;
	rect.left = rect.right - font->WidthOf(string_slice(asterisk));
	rect.top = viewBounds.top + yPos;
	rect.bottom = rect.top + font->Ascent();
	return rect;
***/
}


void MenuControl::Draw(DisplayDirector* directorIn)
{
	if (director->MenuIsVisible())
		return;
	int alpha = CurAlpha(director);
	if (alpha == 0)
		return;

	// set up
	View* view = director->DrawingView();
	view->PushState();
	view->SetDrawingMode(AlphaDrawingMode);
	view->SetPenSize(lineWidth);
	Color white = { 255, 255, 255, 255 };
	view->SetLowColor(white);	// didn't help
//***	Color alphaColor = color;
	Color alphaColor = MenuDirector::borderColor;
	alphaColor.alpha = alpha;

	// calc the ellipse
	Rectangle rect;
	Rectangle viewBounds = director->ViewBounds();
	rect.right = viewBounds.right - xPos;
	rect.left = rect.right - diameter;
	rect.top = viewBounds.top + yPos;
	rect.bottom = rect.top + diameter;

	// clear it
	view->SetHighColor(white);
	view->StrokeEllipse(rect);

	// draw it
	view->SetHighColor(alphaColor);
	view->StrokeEllipse(rect);
/***
	Rectangle rect = GetRect();
	view->SetFont(font);
	view->DrawString(string_slice(asterisk), rect.left, rect.top + font->Ascent());
***/

	view->PopState();
}


void MenuControl::MouseDown(int x, int y, DisplayDirector* directorIn)
{
	if (director->MenuIsVisible())
		director->HideMenu();
	else
		director->ShowMenu();
}


