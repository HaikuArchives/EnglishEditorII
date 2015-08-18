/* InlineLayoutContext.cpp */

#include "InlineLayoutContext.h"
#include "Glyph.h"
#include "Tab.h"
#include "Font.h"

#include <vector>

struct GlyphInfo {
	Glyph*	glyph;
	int   	width;
	int   	ascent;

	GlyphInfo(Glyph* glyphIn, int widthIn, int ascentIn)
		: glyph(glyphIn), width(widthIn), ascent(ascentIn) {}
	GlyphInfo() : glyph(NULL), width(0), ascent(0) {}	// STL requires this
};

class GlyphsList : public std::vector<GlyphInfo> {};

enum {
	AlignLeft,
	AlignRight,
	AlignCenter,
	AlignJustify
};



InlineLayoutContext::InlineLayoutContext(int lineLengthIn, int textIndent, String alignmentIn)
	: curX(textIndent), curY(0), curLine(0),
	  lineLength(lineLengthIn), curLineHeight(0),
	  haveStarted(false), haveStartedLine(false), inWhitespace(true), curWidth(0),
	  maxLineHeight(0), maxAscent(0), multiLineHeights(false),
	  totalHeight(0), curLineWidth(0), maxLineWidth(0)
{
	curGlyphs = new GlyphsList();
	curLineGlyphs = new GlyphsList();

	// alignment
	if (alignmentIn == "right")
		alignment = AlignRight;
	else if (alignmentIn == "center")
		alignment = AlignCenter;
	else if (alignmentIn == "justify")
		alignment = AlignJustify;
	else
		alignment = AlignLeft;
}


InlineLayoutContext::~InlineLayoutContext()
{
	delete curGlyphs;
	delete curLineGlyphs;
}


void InlineLayoutContext::AddGlyph(Glyph* glyph)
{
	bool glyphIsWhitespace = glyph->IsWhitespace();
	int glyphWidth = glyph->Width(font);

	// handle line break specially
	if (glyph->IsLineBreak()) {
		FinishCurGlyphs();
		inWhitespace = true;
		glyph->SetPosition(curX, curY, curLine);
		NewLine();
		return;
		}

	// handle tab specially
	if (glyph->IsTab()) {
		FinishCurGlyphs();
		inWhitespace = true;
		Tab* tab = dynamic_cast<Tab*>(glyph);
		tab->SetPosition(curX, curY, curLine);
		int tabWidth = 4 * font->SpaceWidth();	/***/
		int width = tabWidth - (curX % tabWidth);
		tab->SetWidth(width);
		curX += width;
		haveStartedLine = true;
		return;
		}

	// starting a new run
	if (glyphIsWhitespace != inWhitespace) {
		FinishCurGlyphs();
		inWhitespace = glyphIsWhitespace;
		}

	// continuing a run
	if (glyphIsWhitespace) {
		// continue whitespace
		glyph->SetPosition(curX, curY, curLine);
		if (glyph->IsPreSpace())
			curWidth += glyphWidth;
		else if (glyphWidth > curWidth)
			curWidth = glyphWidth;
		curGlyphs->push_back(GlyphInfo(glyph, glyphWidth, curAscent));
		}
	else {
		// start a new run of words
		curGlyphs->push_back(GlyphInfo(glyph, glyphWidth, curAscent));
		curWidth += glyphWidth;
		}
}


void InlineLayoutContext::SetFont(Font* newFont)
	// NOTE: doesn't take "newFont"
{
	font = newFont;
	SetLineHeight(font->LineHeight());
	curAscent = font->Ascent();
	if (curAscent > maxAscent)
		maxAscent = curAscent;
}


void InlineLayoutContext::SetLineHeight(int newLineHeight)
{
	if (newLineHeight == curLineHeight)
		return;

	if (maxLineHeight != 0)
		multiLineHeights = true;
	curLineHeight = newLineHeight;
	if (curLineHeight > maxLineHeight)
		maxLineHeight = curLineHeight;
}


void InlineLayoutContext::Finish()
{
	FinishCurGlyphs();
	FinishCurLine(true);
}


int InlineLayoutContext::WidthLeftInLine()
{
	return lineLength - curX;
}


int InlineLayoutContext::TotalHeight()
{
	return totalHeight + maxLineHeight;
}


int InlineLayoutContext::MaxLineWidth()
{
	return maxLineWidth;
}


void InlineLayoutContext::FinishCurGlyphs()
{
	if (inWhitespace) {
		for (GlyphsList::iterator g = curGlyphs->begin(); g != curGlyphs->end(); ++g)
			curLineGlyphs->push_back(*g);
		if (haveStarted)
			curX += curWidth;
		}
	else {
		// do a newline if necessary
		if (curWidth > WidthLeftInLine() && haveStartedLine)
			NewLine();

		// position the glyphs
		for (GlyphsList::iterator g = curGlyphs->begin(); g != curGlyphs->end(); ++g) {
			g->glyph->SetPosition(curX, curY, curLine);
			curX += g->width;
			curLineGlyphs->push_back(*g);
			curLineWidth = curX;
			}

		haveStartedLine = true;
		}

	curGlyphs->clear();
	curWidth = 0;
	haveStarted = true;
}


void InlineLayoutContext::FinishCurLine(bool isLastLine)
{
	GlyphsList::iterator g;

	// adjust for multiple heights
	if (multiLineHeights) {
		for (g = curLineGlyphs->begin(); g != curLineGlyphs->end(); ++g)
			g->glyph->AdjustY(maxAscent - g->ascent);
		}
	multiLineHeights = false;

	// alignment
	if (alignment == AlignRight || alignment == AlignCenter) {
		int adjustment = lineLength - curLineWidth;
		if (alignment == AlignCenter)
			adjustment /= 2;
		for (g = curLineGlyphs->begin(); g != curLineGlyphs->end(); ++g)
			g->glyph->AdjustX(adjustment);
		}
	else if (alignment == AlignJustify && !isLastLine) {
		// first, count the number of whitespace glyphs
		int numWhitespaceGlyphs = 0;
		bool haveStarted = false;
		bool lastWasWhitespace = false;
		for (g = curLineGlyphs->begin(); g != curLineGlyphs->end(); ++g) {
			if (g->glyph->IsWhitespace()) {
				if (haveStarted && !lastWasWhitespace)
					++numWhitespaceGlyphs;
				lastWasWhitespace = true;
				}
			else {
				haveStarted = true;
				lastWasWhitespace = false;
				}
			}
		if (lastWasWhitespace)
			--numWhitespaceGlyphs;
		// now do the adjustment
		float perSpaceAdjustment = float(lineLength - curLineWidth) / numWhitespaceGlyphs;
		float curAdjustment = 0;
		haveStarted = lastWasWhitespace = false;
		for (g = curLineGlyphs->begin(); g != curLineGlyphs->end(); ++g) {
			if (g->glyph->IsWhitespace()) {
				if (haveStarted && !lastWasWhitespace)
					curAdjustment += perSpaceAdjustment;
				lastWasWhitespace = true;
				}
			else {
				haveStarted = true;
				lastWasWhitespace = false;
				}
			g->glyph->AdjustX(curAdjustment);
			}
		}

	curLineGlyphs->clear();

	if (curLineWidth > maxLineWidth)
		maxLineWidth = curLineWidth;
}


void InlineLayoutContext::NewLine()
{
	FinishCurLine();

	curX = 0;
	curY += maxLineHeight;
	curLine += 1;
	totalHeight += maxLineHeight;
	maxLineHeight = curLineHeight;
	maxAscent = curAscent;
	multiLineHeights = false;
	haveStartedLine = false;
}


