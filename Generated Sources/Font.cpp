/* Font.cpp */

#include "Font.h"
#include "UTF8Utils.h"
#include <math.h>

const int Font::Normal = 0;
const int Font::Bold = 0x01;
const int Font::Italic = 0x02;


Font::Font(string_slice familyName, int size, int style)
{
	int face = 0;
	if (style & Bold)
		face |= B_BOLD_FACE;
	if (style & Italic)
		face |= B_ITALIC_FACE;
	if (face == 0)
		face = B_REGULAR_FACE;
	font_family family;
	familyName.copy(family);
	family[familyName.length()] = 0;
	font.SetFamilyAndFace(family, face);
	font.SetSize(size);

	font_height fontInfo;
	font.GetHeight(&fontInfo);
	ascent = (int) ceil(fontInfo.ascent);
	descent = (int) ceil(fontInfo.descent);
	lineHeight = (int) ceil(fontInfo.ascent + fontInfo.descent + fontInfo.leading);

	spaceWidth = WidthOf(" ");
}


int Font::Size() const
{
	return (int) font.Size();
}


int Font::WidthOf(string_slice str) const
{
	return (int) ceil(font.StringWidth(str.begin(), str.length()));
}


int Font::CharIndexAtX(int x, string_slice str)
{
	float pixelsLeft = x;
	int numChars = UTF8Utils::CharsInString(str);
	float* escapements = new float[numChars];
	font.GetEscapements(str.begin(), numChars, escapements);
	float fontSize = font.Size();
	int whichChar = 0;
	const char* p = str.begin();
	for (; whichChar < numChars; whichChar++) {
		float charWidth = escapements[whichChar] * fontSize;
		pixelsLeft -= charWidth / 2;
		if (pixelsLeft <= 0)
			break;
		pixelsLeft -= charWidth / 2;
		p += UTF8Utils::BytesForChar(*p);
		}
	delete escapements;
	return (p - str.begin());
}


int Font::SpaceWidth()
{
	return spaceWidth;
}


