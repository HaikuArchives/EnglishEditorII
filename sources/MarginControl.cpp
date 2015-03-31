/* MarginControl.cpp */

#include "MarginControl.h"

const float MarginControl::barLength = 40;
const float MarginControl::arrowLength = 10;
const float MarginControl::arrowXOffset = 6;
const float MarginControl::arrowYOffset = 6;
const float MarginControl::minHMargin = 1;
const float MarginControl::minVMargin = 1;


MarginControl::MarginControl(WindowDirector* directorIn)
	: director(directorIn)
{
}


