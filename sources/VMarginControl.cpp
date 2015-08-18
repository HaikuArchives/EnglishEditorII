/* VMarginControl.cpp */

#include "VMarginControl.h"
#include "WindowDirector.h"
#include "View.h"


VMarginControl::VMarginControl(WindowDirector* directorIn)
	: MarginControl(directorIn)
{
}


Rectangle VMarginControl::GetRect()
{
	int top = (int) (director->ViewBounds().bottom - director->GetVMargin() + 1);
	int left = director->GetHMargin();
	Rectangle rect(left, top, left + barLength, top + arrowLength);
	return rect;
}


void VMarginControl::Draw(DisplayDirector* directorIn)
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
	view->StrokeLine(curRect.LeftTop(), curRect.RightTop());
	// arrow
	int arrowX = (int) ceil((curRect.left + curRect.right) / 2);
	view->StrokeLine(CoordPoint(arrowX, curRect.top), CoordPoint(arrowX, curRect.bottom));
	view->StrokeLine(CoordPoint(arrowX - arrowXOffset, curRect.top + arrowYOffset),
	                 CoordPoint(arrowX, curRect.top));
	view->StrokeLine(CoordPoint(arrowX + arrowXOffset, curRect.top + arrowYOffset),
	                 CoordPoint(arrowX, curRect.top));

	// clean up
	view->PopState();
}


void VMarginControl::MouseDown(int x, int y, DisplayDirector* directorIn)
{
	Rectangle rect = GetRect();
	int offset = (int) (y - rect.top);
	int lastVMargin = director->GetVMargin();
	int viewBottom = (int) director->ViewBounds().bottom;
	View* view = director->WindowView();
	while (true) {
		// get the mouse
		CoordPoint mousePoint = view->GetMousePoint();
		int buttons = view->GetMouseButtons();
		if (buttons == 0)
			break;

		// see if it moved
		float newVMargin = viewBottom - (mousePoint.y - offset);
		if (newVMargin < minVMargin)
			newVMargin = minVMargin;
		if (newVMargin == lastVMargin) {
			view->MouseTrackingPause();
			continue;
			}

		// set the new VMargin
		director->SetVMargin(newVMargin);
		lastVMargin = (int) newVMargin;
		}
}


