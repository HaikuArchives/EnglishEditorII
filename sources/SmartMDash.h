/* SmartMDash.h */

#ifndef _H_SmartMDash_
#define _H_SmartMDash_

#include "Glyph.h"
#include "string_slice.h"

class SmartMDash : public Glyph {
public:
	SmartMDash();
	int       	Width(Font* font);
	void      	UncacheWidth();
	void      	Draw(GlyphDrawContext* context);
	int       	NumCharsInText();
	Action*   	DeleteFrontAction(TextDisplayNode* textNode);
	Action*   	DeleteBackAction(TextDisplayNode* textNode);
	Selection*	SelectionIntoFront(TextDisplayNode* textNode);
	Selection*	SelectionIntoBack(TextDisplayNode* textNode);
	bool      	IsPunctuation();
	bool      	ShouldBindBefore();
	bool      	ShouldBindAfter();

protected:
	short                    	width;
	static const string_slice	mDash;
};


#endif
