/* SelectGlyphsAction.h */

#ifndef _H_SelectGlyphsAction_
#define _H_SelectGlyphsAction_

#include "Action.h"

class Text;

class SelectGlyphsAction : public Action {
public:
	SelectGlyphsAction(Text* firstTextIn, unsigned long firstOffsetIn,
                   Text* lastTextIn, unsigned long lastOffsetIn);
	void	Do(DisplayDirector* director);
	void	Undo(DisplayDirector* director);

protected:
	Text*        	firstText;
	unsigned long	firstOffset;
	Text*        	lastText;
	unsigned long	lastOffset;
};


#endif
