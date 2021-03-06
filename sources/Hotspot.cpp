/* Hotspot.cpp */

#include "Hotspot.h"
#include "StyleParser.h"
#include "View.h"

const rgb_color Hotspot::hotspotColor = StyleParser::ParseColor("rgba(255, 130, 0, 128)");


Hotspot::~Hotspot()
{
}


void Hotspot::FadeDocument(View* view, BPoint origin)
{
	// I've decided I don't like this
	return;

	static const rgb_color fadeColor = { 255, 255, 255, 128 };
//***	static const rgb_color fadeColor = { 255, 255, 255, 96 };
		// nicer, but only works at 32-bit

	view->PushState();

	// set up
	BRect bounds = Bounds();
	bounds.OffsetBy(origin);
	BRect viewBounds = view->Bounds();
	view->SetDrawingMode(B_OP_ALPHA);
	view->SetHighColor(fadeColor);

	// draw the four rectangles
	BRect rect = viewBounds;
	rect.bottom = bounds.top;
	view->FillRect(rect);
	rect.top = bounds.top;
	rect.bottom = bounds.bottom;
	rect.right = bounds.left;
	view->FillRect(rect);
	rect.left = bounds.right;
	rect.right = viewBounds.right;
	view->FillRect(rect);
	rect.top = bounds.bottom;
	rect.bottom = viewBounds.bottom;
	rect.left = viewBounds.left;
	view->FillRect(rect);

	view->PopState();
}


