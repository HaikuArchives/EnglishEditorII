/* BetweenGlyphsDestination.h */

#ifndef _H_BetweenGlyphsDestination_
#define _H_BetweenGlyphsDestination_

#include "Destination.h"

class TextDisplayNode;
class Glyph;
class InlineArrow;

class BetweenGlyphsDestination : public Destination {
public:
	BetweenGlyphsDestination(Selection* selectionIn, TextDisplayNode* textNodeIn,
                         Glyph* firstGlyphIn, Glyph* secondGlyphIn);
	~BetweenGlyphsDestination();
	Rectangle	ArrowBounds();
	void     	DrawArrow(View* view, CoordPoint origin);
	Action*  	GetMoveAction();
	Action*  	GetCopyAction();
	Action*  	MakeMoveAction(bool copying);

protected:
	TextDisplayNode*	textNode;
	Glyph*          	firstGlyph;
	Glyph*          	secondGlyph;
	InlineArrow*    	arrow;
};


#endif
