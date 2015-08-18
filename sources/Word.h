/* Word.h */

#ifndef _H_Word_
#define _H_Word_

#include "Glyph.h"
#include "String.h"

class qstring;

class Word : public Glyph {
public:
	Word(String wordIn, bool isPunctuationIn = false);
	int                 	Width(Font* font);
	void                	UncacheWidth();
	void                	Draw(GlyphDrawContext* context);
	int                 	NumCharsInText();
	Action*             	DeleteFrontAction(TextDisplayNode* textNode);
	Action*             	DeleteBackAction(TextDisplayNode* textNode);
	Selection*          	SelectionIntoFront(TextDisplayNode* textNode);
	Selection*          	SelectionIntoBack(TextDisplayNode* textNode);
	bool                	IsPunctuation();
	bool                	ShouldBindBefore();
	bool                	ShouldBindAfter();
	virtual string_slice	GetDisplayWord();
	virtual int         	DocToDisplayOffset(int docOffset);
	virtual int         	DisplayToDocOffset(int displayOffset);

protected:
	String	word;
	short    	width;
	bool     	isPunctuation;
};


#endif
