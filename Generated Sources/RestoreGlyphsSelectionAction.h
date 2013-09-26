/* RestoreGlyphsSelectionAction.h */

#ifndef _H_RestoreGlyphsSelectionAction_
#define _H_RestoreGlyphsSelectionAction_

#include "Action.h"

class Text;

class RestoreGlyphsSelectionAction : public Action {
public:
	RestoreGlyphsSelectionAction();
	void	Do(DisplayDirector* director);
	void	Undo(DisplayDirector* director);

protected:
	Text*        	firstText;
	unsigned long	firstOffset;
	Text*        	lastText;
	unsigned long	lastOffset;
};


#endif
