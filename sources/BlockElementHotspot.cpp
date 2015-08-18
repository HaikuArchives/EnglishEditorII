/* BlockElementHotspot.cpp */

#include "BlockElementHotspot.h"
#include "ElementDisplayNode.h"
#include "View.h"
#include "Shape.h"

const int BlockElementHotspot::lineWidth = 3;
const int BlockElementHotspot::xOutset = 4;
const int BlockElementHotspot::yOutset = 0;
const int BlockElementHotspot::cornerRadius = 8;
const int BlockElementHotspot::lineSpill = lineWidth - 1;


BlockElementHotspot::BlockElementHotspot(ElementDisplayNode* displayNodeIn)
	: ElementHotspot(displayNodeIn)
{
}


BRect BlockElementHotspot::Bounds()
{
	BRect bounds;
	bounds.top = displayNode->Top();
	bounds.bottom = bounds.top + displayNode->Height();
	bounds.left = displayNode->LeftSpace();
	bounds.right = bounds.left + displayNode->Width();
	bounds.InsetBy(-(xOutset + lineSpill), -(yOutset + lineSpill));
	return bounds;
}


bool BlockElementHotspot::ContainsPoint(BPoint point)
{
	return Bounds().Contains(point);
}


void BlockElementHotspot::Draw(View* view, BPoint origin)
{
	// fade the rest of the document
	FadeDocument(view, origin);

	// set up drawing
	view->PushState();
	view->SetPenSize(lineWidth);
	view->SetHighColor(Hotspot::hotspotColor);
	view->SetDrawingMode(B_OP_ALPHA);

	// draw
	BRect rect = Bounds();
	rect.InsetBy(lineSpill, lineSpill);
	Shape shape;
	shape.MoveTo(rect.left, rect.top + cornerRadius);
	shape.VerticalCornerTo(rect.left + cornerRadius, rect.top);
	shape.LineTo(rect.right - cornerRadius, rect.top);
	shape.HorizontalCornerTo(rect.right, rect.top + cornerRadius);
	shape.LineTo(rect.right, rect.bottom - cornerRadius);
	shape.VerticalCornerTo(rect.right - cornerRadius, rect.bottom);
	shape.LineTo(rect.left + cornerRadius, rect.bottom);
	shape.HorizontalCornerTo(rect.left, rect.bottom - cornerRadius);
	shape.Close();
	view->MovePenTo(origin);
	view->StrokeShape(&shape);

	// clean up drawing
	view->PopState();
}


