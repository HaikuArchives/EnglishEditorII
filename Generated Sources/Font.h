/* Font.h */

#ifndef _H_Font_
#define _H_Font_

#include "string_slice.h"
#include <Font.h>

/* BeOS version */

class Font {
public:
	Font(string_slice familyName, int size, int style);
	inline int   	LineHeight() const;
	inline int   	Ascent() const;
	inline int   	Descent() const;
	int          	Size() const;
	int          	WidthOf(string_slice str) const;
	int          	CharIndexAtX(int x, string_slice str);
	int          	SpaceWidth();
	inline BFont*	NativeFont();

protected:
	BFont           	font;
	int             	ascent;
	int             	descent;
	int             	lineHeight;
	int             	spaceWidth;

public:
	static const int	Normal;
	static const int	Bold;
	static const int	Italic;
};

inline int Font::LineHeight() const
{
	return lineHeight;
}



inline int Font::Ascent() const
{
	return ascent;
}



inline int Font::Descent() const
{
	return descent;
}



inline BFont* Font::NativeFont()
{
	return &font;
}




#endif
