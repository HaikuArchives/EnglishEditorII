/* GlyphLoc.h */

#ifndef _H_GlyphLoc_
#define _H_GlyphLoc_

#include "GlyphRef.h"
#include <Point.h>

// comparison object for Glyph locations

class GlyphLoc {
public:
	GlyphLoc(GlyphRef glyphRefIn);
	bool	operator<(BPoint point);
	bool	operator>(BPoint point);
	bool	operator<=(BPoint point);
	bool	operator>=(BPoint point);

protected:
	GlyphRef	glyphRef;
};


#endif
