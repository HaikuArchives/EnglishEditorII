/* ScrollerControl.cpp */

#include "ScrollerControl.h"
#include "WindowDirector.h"
#include "View.h"

const int ScrollerControl::width = 8;
const int ScrollerControl::height = 80;
rgb_color ScrollerControl::thumbColor = { 0xD5, 0xE5, 0xFF, 255 };


ScrollerControl::ScrollerControl(WindowDirector* directorIn)
	: director(directorIn)
{
}


BRect ScrollerControl::GetRect()
{
	BRect rect = director->ViewBounds();
	rect.bottom -= director->GetVMargin();
	rect.top = rect.bottom - height;
	rect.right -= (director->GetHMargin() - width) / 2;
	rect.left = rect.right - width;
	return rect;
}


void ScrollerControl::Draw(DisplayDirector* directorIn)
{
	int alpha = CurAlpha(director);
	if (alpha == 0)
		return;

	// set up
	View* view = director->DrawingView();
	view->PushState();
	view->SetDrawingMode(B_OP_ALPHA);
	BRect rect = GetRect();
	rgb_color frameColor = controlColor;
	frameColor.alpha = alpha;

	// draw the thumb
	int scrollPos = director->GetScrollPos();
	int docHeight = director->GetDocumentHeight();
	float proportion = director->GetVisibleProportion();
	BRect thumbRect = rect;
	thumbRect.top += (float) scrollPos / docHeight * height;
	thumbRect.bottom = thumbRect.top + proportion * height;
	view->SetHighColor(frameColor);
	view->StrokeRect(thumbRect);
	thumbRect.InsetBy(1, 1);
	rgb_color curThumbColor = thumbColor;
	curThumbColor.alpha = alpha;
	view->SetHighColor(curThumbColor);
	view->FillRect(thumbRect);

	// draw the frame
	view->SetHighColor(frameColor);
	view->StrokeLine(rect.LeftTop(), rect.RightTop());
	view->StrokeLine(rect.LeftBottom(), rect.RightBottom());
	int xCenter = (int) ((rect.left + rect.right) / 2);
	view->StrokeLine(BPoint(xCenter, rect.top), BPoint(xCenter, thumbRect.top - 1));
	view->StrokeLine(BPoint(xCenter, thumbRect.bottom + 1), BPoint(xCenter, rect.bottom));

	view->PopState();
}


void ScrollerControl::MouseDown(int x, int y, DisplayDirector* directorIn)
{
	BPoint mousePoint(x, y);

	// figure out how we're scrolling
	BRect rect = GetRect();
	int lastScrollPos = director->GetScrollPos();
	int docHeight = director->GetDocumentHeight();
	float proportion = director->GetVisibleProportion();
	float thumbTop = rect.top + lastScrollPos / docHeight * height;
	float thumbBottom = thumbTop + proportion * height;
	float offset = 0;	// default is "above thumb"
	bool inThumb = false;
	if (mousePoint.y > thumbTop && mousePoint.y < thumbBottom) {
		// in thumb
		offset = mousePoint.y - thumbTop;
		inThumb = true;
		}
	else if (mousePoint.y > thumbBottom) {
		// below thumb
		offset = proportion * height;
		}
	float lastMouseY = -60000;	// always wanna go thru the loop once

	View* view = director->WindowView();
	while (true) {
		// get the mouse
		mousePoint = view->GetMousePoint();
		int buttons = view->GetMouseButtons();
		if (buttons == 0)
			break;

		// scroll the document
		int targetScrollPos =
			(int) ceil((mousePoint.y - offset - rect.top) / height * docHeight);
		bool scrolling;
		if (lastMouseY != mousePoint.y)
			director->SetScrollTarget(targetScrollPos);
		scrolling = director->DoScrollStep();
		lastMouseY = mousePoint.y;

		// be nice while scroll isn't taking place
		if (!scrolling)
			view->MouseTrackingPause();
		}

	// user let up on mouse; stop scrolling
	director->SetScrollTarget(director->GetScrollPos());
}


