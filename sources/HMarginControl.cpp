/* HMarginControl.cpp */

#include "HMarginControl.h"
#include "WindowDirector.h"
#include "View.h"


HMarginControl::HMarginControl(WindowDirector* directorIn)
	: MarginControl(directorIn)
{
}


BRect HMarginControl::GetRect()
{
	int right = director->GetHMargin() - 1;
	int bottom = (int) (director->ViewBounds().bottom - director->GetVMargin());
	BRect rect(right - arrowLength, bottom - barLength, right, bottom);
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
	view->SetDrawingMode(B_OP_ALPHA);
	rgb_color alphaColor = controlColor;
	alphaColor.alpha = curAlpha;
	view->SetHighColor(alphaColor);

	// draw
	BRect curRect = GetRect();
	// bar
	view->StrokeLine(curRect.RightTop(), curRect.RightBottom());
	// arrow
	int arrowY = (int) ceil((curRect.top + curRect.bottom) / 2);
	view->StrokeLine(BPoint(curRect.left, arrowY), BPoint(curRect.right, arrowY));
	view->StrokeLine(BPoint(curRect.right - arrowXOffset, arrowY - arrowYOffset),
	                 BPoint(curRect.right, arrowY));
	view->StrokeLine(BPoint(curRect.right - arrowXOffset, arrowY + arrowYOffset),
	                 BPoint(curRect.right, arrowY));

	// clean up
	view->PopState();
}


void HMarginControl::MouseDown(int x, int y, DisplayDirector* directorIn)
{
	BRect rect = GetRect();
	int offset = (int) (rect.right - x);
	int lastHMargin = director->GetHMargin();
	View* view = director->WindowView();
	while (true) {
		// get the mouse
		BPoint mousePoint = view->GetMousePoint();
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


