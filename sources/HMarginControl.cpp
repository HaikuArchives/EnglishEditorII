/* HMarginControl.cpp */

#include "HMarginControl.h"
#include "WindowDirector.h"
#include "View.h"


HMarginControl::HMarginControl(WindowDirector* directorIn)
	: MarginControl(directorIn)
{
}


Rectangle HMarginControl::GetRect()
{
	int right = director->GetHMargin() - 1;
	int bottom = (int) (director->ViewBounds().bottom - director->GetVMargin());
	Rectangle rect(right - arrowLength, bottom - barLength, right, bottom);
	return rect;
}


void HMarginControl::Draw(DisplayDirector* directorIn)
{
	int curAlpha = CurAlpha(director);
	if (curAlpha == 0)
		return;

	// set up view
	View* view = director->DrawingView();
	view->PushState();
	view->SetDrawingMode(AlphaDrawingMode);
	rgb_color alphaColor = controlColor;
	alphaColor.alpha = curAlpha;
	view->SetHighColor(alphaColor);

	// draw
	Rectangle curRect = GetRect();
	// bar
	view->StrokeLine(curRect.RightTop(), curRect.RightBottom());
	// arrow
	int arrowY = (int) ceil((curRect.top + curRect.bottom) / 2);
	view->StrokeLine(CoordPoint(curRect.left, arrowY), CoordPoint(curRect.right, arrowY));
	view->StrokeLine(CoordPoint(curRect.right - arrowXOffset, arrowY - arrowYOffset),
	                 CoordPoint(curRect.right, arrowY));
	view->StrokeLine(CoordPoint(curRect.right - arrowXOffset, arrowY + arrowYOffset),
	                 CoordPoint(curRect.right, arrowY));

	// clean up
	view->PopState();
}


void HMarginControl::MouseDown(int x, int y, DisplayDirector* directorIn)
{
	Rectangle rect = GetRect();
	int offset = (int) (rect.right - x);
	int lastHMargin = director->GetHMargin();
	View* view = director->WindowView();
	while (true) {
		// get the mouse
		CoordPoint mousePoint = view->GetMousePoint();
		int buttons = view->GetMouseButtons();
		if (buttons == 0)
			break;

		// see if it moved
		int newHMargin = (int) (mousePoint.x + offset);
		if (newHMargin < minHMargin)
			newHMargin = (int) minHMargin;
		if (newHMargin == lastHMargin) {
			view->MouseTrackingPause();
			continue;
			}

		// set the new HMargin
		director->SetHMargin(newHMargin);
		lastHMargin = newHMargin;
		}
}


