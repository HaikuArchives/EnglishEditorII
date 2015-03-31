/* PreSpace.cpp */

#include "PreSpace.h"


PreSpace::PreSpace(DOMString textIn)
	: Word(textIn)
{
}


bool PreSpace::IsWhitespace()
{
	return true;
}


bool PreSpace::IsPreSpace()
{
	return true;
}


