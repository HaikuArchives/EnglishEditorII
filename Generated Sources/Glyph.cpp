/* Glyph.cpp */

#include "Glyph.h"
#include "GlyphDrawContext.h"
#include "Font.h"



Glyph::Glyph()
{
}


Glyph::~Glyph()
{
}


void Glyph::AdjustX(int xDelta)
{
	x += xDelta;
}


void Glyph::AdjustY(int yDelta)
{
	y += yDelta;
}


void Glyph::UncacheWidth()
{
	// default: do nothing--not every Glyph actually does cache its width
}


bool Glyph::IsWhitespace()
{
	// default: no
	return false;
}


bool Glyph::IsPunctuation()
{
	// default: no
	return false;
}


bool Glyph::IsLineBreak()
{
	// default: no
	return false;
}


bool Glyph::IsPreSpace()
{
	// default: no
	return false;
}


bool Glyph::IsTab()
{
	// default: no
	return false;
}


int Glyph::GetLine()
{
	return line;
}


bool Glyph::ShouldDraw(GlyphDrawContext* context)
{
	int top = y + context->originY;
	Font* font = context->font;
	return (top >= context->startY && top + font->LineHeight() <= context->endY);
}


