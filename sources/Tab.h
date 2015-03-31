/* Tab.h */

#ifndef _H_Tab_
#define _H_Tab_

#include "Glyph.h"

class Tab : public Glyph {
public:
	Tab();
	void      	SetWidth(int newWidth);
	int       	Width(Font* font);
	void      	Draw(GlyphDrawContext* context);
	bool      	IsWhitespace();
	bool      	IsTab();
	int       	NumCharsInText();
	Action*   	DeleteFrontAction(TextDisplayNode* textNode);
	Action*   	DeleteBackAction(TextDisplayNode* textNode);
	Selection*	SelectionIntoFront(TextDisplayNode* textNode);
	Selection*	SelectionIntoBack(TextDisplayNode* textNode);
	bool      	ShouldBindBefore();
	bool      	ShouldBindAfter();

protected:
	int	width;
};


#endif
