/* Destination.cpp */

#include "Destination.h"

const rgb_color Destination::arrowColor = { 0, 0xBB, 255, 255 };
const float Destination::arrowheadWidth = 4;
const float Destination::arrowheadHeight = 6;
const float Destination::arrowLineWidth = 3;


Destination::Destination(Selection* selectionIn)
	: selection(selectionIn)
{
}


Destination::~Destination()
{
}


