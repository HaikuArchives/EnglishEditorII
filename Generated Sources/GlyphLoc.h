/* GlyphLoc.h */

#ifndef _H_GlyphLoc_
#define _H_GlyphLoc_

#include "GlyphRef.h"
#include "CoordPoint.h"

// comparison object for Glyph locations

class GlyphLoc {
public:
	GlyphLoc(GlyphRef glyphRefIn);
	bool	operator<(CoordPoint point);
	bool	operator>(CoordPoint point);
	bool	operator<=(CoordPoint point);
	bool	operator>=(CoordPoint point);

protected:
	GlyphRef	glyphRef;
};


#endif
