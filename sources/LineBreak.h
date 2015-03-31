/* LineBreak.h */

#ifndef _H_LineBreak_
#define _H_LineBreak_

#include "Glyph.h"
#include "DOMString.h"

class LineBreak : public Glyph {
public:
	LineBreak(DOMString text);
	int       	Width(Font* font);
	void      	Draw(GlyphDrawContext* context);
	bool      	IsWhitespace();
	bool      	IsLineBreak();
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
