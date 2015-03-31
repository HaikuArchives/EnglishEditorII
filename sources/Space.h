/* Space.h */

#ifndef _H_Space_
#define _H_Space_

#include "Glyph.h"
#include "string_slice.h"

class Space : public Glyph {
public:
	Space(string_slice text);
	int       	Width(Font* font);
	void      	Draw(GlyphDrawContext* context);
	bool      	IsWhitespace();
	int       	NumCharsInText();
	Action*   	DeleteFrontAction(TextDisplayNode* textNode);
	Action*   	DeleteBackAction(TextDisplayNode* textNode);
	Selection*	SelectionIntoFront(TextDisplayNode* textNode);
	Selection*	SelectionIntoBack(TextDisplayNode* textNode);
	bool      	ShouldBindBefore();
	bool      	ShouldBindAfter();

protected:
	int	numCharsInText;
};


#endif
