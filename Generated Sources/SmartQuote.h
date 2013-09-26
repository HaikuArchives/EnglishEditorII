/* SmartQuote.h */

#ifndef _H_SmartQuote_
#define _H_SmartQuote_

#include "Glyph.h"
#include "string_slice.h"

class SmartQuote : public Glyph {
public:
	SmartQuote(bool openingIn);
	virtual string_slice	DisplayString() = 0;
	int                 	Width(Font* font);
	void                	UncacheWidth();
	void                	Draw(GlyphDrawContext* context);
	bool                	IsPunctuation();
	int                 	NumCharsInText();
	Action*             	DeleteFrontAction(TextDisplayNode* textNode);
	Action*             	DeleteBackAction(TextDisplayNode* textNode);
	Selection*          	SelectionIntoFront(TextDisplayNode* textNode);
	Selection*          	SelectionIntoBack(TextDisplayNode* textNode);
	bool                	ShouldBindBefore();
	bool                	ShouldBindAfter();

protected:
	bool 	opening;
	short	width;
};


#endif
