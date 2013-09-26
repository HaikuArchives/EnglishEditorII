/* GlyphLoc.cpp */

#include "GlyphLoc.h"
#include "Glyph.h"
#include "TextDisplayNode.h"
#include "Font.h"



GlyphLoc::GlyphLoc(GlyphRef glyphRefIn)
	: glyphRef(glyphRefIn)
{
}


bool GlyphLoc::operator<(CoordPoint point)
{
	if (glyphRef.GetGlyph()->GetY() > point.y)
		return false;
	Font* font = glyphRef.GetTextNode()->CurFont();
	if (glyphRef.GetGlyph()->GetY() + font->LineHeight() < point.y)
		return true;
	return (glyphRef.GetGlyph()->GetX() + glyphRef.GetGlyph()->Width(font) < point.x);
}


bool GlyphLoc::operator>(CoordPoint point)
{
	if (glyphRef.GetGlyph()->GetY() > point.y)
		return true;
	Font* font = glyphRef.GetTextNode()->CurFont();
	if (glyphRef.GetGlyph()->GetY() + font->LineHeight() < point.y)
		return false;
	return (glyphRef.GetGlyph()->GetX() > point.x);
}


bool GlyphLoc::operator<=(CoordPoint point)
{
	return !(*this > point);
}


bool GlyphLoc::operator>=(CoordPoint point)
{
	return !(*this < point);
}


