/* Control.cpp */

#include "Control.h"
#include "DisplayDirector.h"
#include "View.h"
#include "CoordPoint.h"

const rgb_color Control::controlColor = { 0, 0xBA, 255, 255 };
const float Control::visibleZone = 64;


Control::~Control()
{
}


int Control::CurAlpha(DisplayDirector* director)
{
	// find out where the mouse is and make sure it's in the window
	View* windowView = director->WindowView();
	CoordPoint mousePoint = windowView->GetMousePoint();
	if (!windowView->Bounds().BRect::Contains(mousePoint))
		return 0;

	// calculate the alpha
	Rectangle rect = GetRect();
	int xDistance = 0;
	if (mousePoint.x < rect.left)
		xDistance = (int) (rect.left - mousePoint.x);
	else if (mousePoint.x > rect.right)
		xDistance = (int) (mousePoint.x - rect.right);
	int yDistance = 0;
	if (mousePoint.y < rect.top)
		yDistance = (int) (rect.top - mousePoint.y);
	else if (mousePoint.y > rect.bottom)
		yDistance = (int) (mousePoint.y - rect.bottom);
	float distance = sqrt(xDistance * xDistance + yDistance * yDistance);
	if (distance > visibleZone)
		distance = visibleZone;
	float visibility = (visibleZone - distance) / visibleZone;
	return (int) (visibility * 255);
}


