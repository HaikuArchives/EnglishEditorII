/* Shape.cpp */

#include "Shape.h"



Shape::Shape()
{
}


void Shape::MoveTo(BPoint point)
{
	shape.MoveTo(point);
	curPoint = point;
}


void Shape::MoveTo(int x, int y)
{
	MoveTo(BPoint(x, y));
}


void Shape::LineTo(BPoint point)
{
	shape.LineTo(point);
	curPoint = point;
}


void Shape::LineTo(int x, int y)
{
	LineTo(BPoint(x, y));
}


void Shape::HorizontalCornerTo(BPoint point)
{
	BPoint controlPoints[3];
	controlPoints[0].x = (curPoint.x + point.x) / 2;
	controlPoints[0].y = curPoint.y;
	controlPoints[1].x = point.x;
	controlPoints[1].y = (curPoint.y + point.y) / 2;
	controlPoints[2] = point;
	shape.BezierTo(controlPoints);
	curPoint = point;
}


void Shape::HorizontalCornerTo(int x, int y)
{
	HorizontalCornerTo(BPoint(x, y));
}


void Shape::VerticalCornerTo(BPoint point)
{
	BPoint controlPoints[3];
	controlPoints[0].Set(curPoint.x, (curPoint.y + point.y) / 2);
	controlPoints[1].Set((curPoint.x + point.x) / 2, point.y);
	controlPoints[2] = point;
	shape.BezierTo(controlPoints);
	curPoint = point;
}


void Shape::VerticalCornerTo(int x, int y)
{
	VerticalCornerTo(BPoint(x, y));
}


void Shape::Close()
{
	shape.Close();
}


void Shape::Clear()
{
	shape.Clear();
}


BShape* Shape::NativeShape()
{
	return &shape;
}


