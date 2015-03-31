/* InlineArrow.cpp */

#include "InlineArrow.h"
#include "View.h"

const Color InlineArrow::arrowColor = { 0, 0xBB, 255, 255 };
const int InlineArrow::arrowheadWidth = 8;
const int InlineArrow::arrowheadHeight = 6;
const int InlineArrow::arrowLineWidth = 2;
const int InlineArrow::loopZone = 24;
const int InlineArrow::loopHeight = 16;
const int InlineArrow::lineSlop = arrowLineWidth / 2;


InlineArrow::InlineArrow(CoordPoint fromPointIn, CoordPoint toPointIn, bool above, int ascent)
	: fromPoint(fromPointIn), toPoint(toPointIn)
{
	if (above) {
		// above selection
		if (toPoint.y >= fromPoint.y - loopZone) {
			// need to loop
			loops = true;
			arrowPointsDown = true;
			toPoint.y -= ceil(ascent / 2);
			}
		else {
			loops = false;
			arrowPointsDown = false;
			}
		}
	else {
		// below selection
		if (toPoint.y <= fromPoint.y + loopZone) {
			loops = true;
			arrowPointsDown = false;
			}
		else {
			loops = false;
			arrowPointsDown = true;
			toPoint.y -= ceil(ascent / 2);
			}
		}
}


Rectangle InlineArrow::Bounds()
{
	int left = (int) (fromPoint.x - lineSlop);
	int arrowLeft = (int) (toPoint.x - arrowheadWidth / 2 - lineSlop);
	if (arrowLeft < left)
		left = arrowLeft;

	int right = (int) (fromPoint.x + lineSlop);
	int arrowRight = (int) (toPoint.x + arrowheadWidth / 2 + lineSlop);
	if (arrowRight > right)
		right = arrowRight;

	int top = (int) (fromPoint.y - lineSlop);
	if (loops && arrowPointsDown)
		top = (int) (toPoint.y - loopHeight - lineSlop);
	else {
		int arrowTop = (int) (toPoint.y - lineSlop);
		if (arrowPointsDown)
			arrowTop -= arrowheadHeight;
		if (arrowTop < top)
			top = arrowTop;
		}

	int bottom = (int) (fromPoint.y + lineSlop);
	if (loops && !arrowPointsDown)
		bottom = (int) (toPoint.y + loopHeight + lineSlop);
	else {
		float arrowBottom = toPoint.y + lineSlop;
		if (!arrowPointsDown)
			arrowBottom += arrowheadHeight;
		if (arrowBottom > bottom)
			bottom = (int) arrowBottom;
		}

	return Rectangle(left, top, right, bottom);
}


void InlineArrow::Draw(View* view, CoordPoint origin)
{
	view->PushState();
	view->SetPenSize(arrowLineWidth);
	view->SetHighColor(arrowColor);

	// line
	BPoint points[4];
	if (loops) {
		CoordPoint center((fromPoint.x + toPoint.x) / 2,
		                  toPoint.y + (arrowPointsDown ? -loopHeight : loopHeight));
		points[0] = fromPoint + origin;
		points[1] = CoordPoint(fromPoint.x, center.y) + origin;
		points[2] = points[1];
		points[3] = center + origin;
		view->StrokeBezier(points);
		points[0] = center + origin;
		points[1] = CoordPoint(toPoint.x, center.y) + origin;
		points[2] = points[1];
		points[3] = toPoint + origin;
		view->StrokeBezier(points);
		}
	else {
		points[0] = fromPoint + origin;
		points[1] = CoordPoint(fromPoint.x, (fromPoint.y + toPoint.y) / 2) + origin;
		points[2] = CoordPoint(toPoint.x, fromPoint.y) + origin;
		points[3] = toPoint + origin;
		view->StrokeBezier(points);
		}

	// arrowhead
	float xOffset = arrowheadWidth / 2;
	if (arrowPointsDown) {
		// arrow points down
		float arrowTop = toPoint.y - arrowheadHeight;
		view->StrokeLine(toPoint + origin,
		                 CoordPoint(toPoint.x - xOffset, arrowTop) + origin);
		view->StrokeLine(toPoint + origin,
		                 CoordPoint(toPoint.x + xOffset, arrowTop) + origin);
		}
	else {
		// arrow points up
		float arrowBottom = toPoint.y + arrowheadHeight;
		view->StrokeLine(toPoint + origin,
		                 CoordPoint(toPoint.x - xOffset, arrowBottom) + origin);
		view->StrokeLine(toPoint + origin,
		                 CoordPoint(toPoint.x + xOffset, arrowBottom) + origin);
		}

	view->PopState();
}


