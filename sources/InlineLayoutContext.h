/* InlineLayoutContext.h */

#ifndef _H_InlineLayoutContext_
#define _H_InlineLayoutContext_

#include "String.h"

class Glyph;
class Font;
class GlyphsList;

class InlineLayoutContext {
public:
	InlineLayoutContext(int lineLengthIn, int textIndent, String alignmentIn);
	~InlineLayoutContext();
	void	AddGlyph(Glyph* glyph);
	void	SetFont(Font* newFont);
	void	SetLineHeight(int newLineHeight);
	void	Finish();
	int 	WidthLeftInLine();
	int 	TotalHeight();
	int 	MaxLineWidth();
	void	FinishCurGlyphs();
	void	FinishCurLine(bool isLastLine = false);
	void	NewLine();

protected:
	int        	curX;
	int        	curY;
	int        	curLine;
	Font*      	font;
	int        	lineLength;
	int        	alignment;
	int        	curLineHeight;
	int        	curAscent;
	bool       	haveStarted;
	bool       	haveStartedLine;
	GlyphsList*	curGlyphs;
	bool       	inWhitespace;
	int        	curWidth;
	GlyphsList*	curLineGlyphs;
	int        	maxLineHeight;
	int        	maxAscent;
	bool       	multiLineHeights;
	int        	totalHeight;
	int        	curLineWidth;
	int        	maxLineWidth;
};


#endif
