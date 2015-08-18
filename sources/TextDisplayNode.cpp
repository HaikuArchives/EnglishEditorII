/* TextDisplayNode.cpp */

#include "TextDisplayNode.h"
#include "Text.h"
#include "Word.h"
#include "SmartenedWord.h"
#include "Space.h"
#include "PreSpace.h"
#include "LineBreak.h"
#include "Tab.h"
#include "SmartMDash.h"
#include "SmartSingleQuote.h"
#include "SmartDoubleQuote.h"
#include "InlineLayoutContext.h"
#include "InlineDrawContext.h"
#include "GlyphDrawContext.h"
#include "FindSelectionContext.h"
#include "FindDestinationContext.h"
#include "CharacterSelection.h"
#include "BetweenGlyphsSelection.h"
#include "BetweenWordsSelection.h"
#include "GlyphsSelection.h"
#include "InlineElementSelection.h"
#include "SpaceDestination.h"
#include "BetweenGlyphsDestination.h"
#include "BlockableDisplayNode.h"
#include "StyleParser.h"
#include "View.h"
#include "Font.h"
#include "UTF8Utils.h"
#include "InternalException.h"

#include <vector>
#include <typeinfo>

class GlyphList : public std::vector<Glyph*> {};

enum {
	NoGlyphType,
	WhitespaceGlyph,
	PreSpaceGlyph,
	LineBreakGlyph,
	TabGlyph,
	WordGlyph,
	SmartenedWordGlyph,
	PunctGlyph,
	MDashGlyph,
	SingleQuoteGlyph,
	DoubleQuoteGlyph,
	OpenParenGlyph,
	EndGlyphType
};



TextDisplayNode::TextDisplayNode(Text* textIn)
	: text(textIn), font(NULL), isPreSpace(false)
{
	glyphs = new GlyphList();
	text->AddChangeClient(this);
}


TextDisplayNode::~TextDisplayNode()
{
	text->RemoveChangeClient(this);
	ClearGlyphs();
	delete glyphs;
	delete font;
}


DisplayType TextDisplayNode::GetDisplayType()
{
	return Inline;
}


bool TextDisplayNode::IsLeafBlock()
{
	return false;
}


bool TextDisplayNode::IsDisposable()
{
	return false;
}


void TextDisplayNode::Load(DisplayDirector* director)
{
	UpdatePreSpace();
	ClearGlyphs();
	CreateGlyphs();

	InlineableDisplayNode::Load(director);
}


void TextDisplayNode::UpdateProperties()
{
	// font
	delete font;
	font = GetFont();
	for (GlyphList::iterator g = glyphs->begin(); g != glyphs->end(); ++g)
		(*g)->UncacheWidth();

	// color
	String colorStr = GetInheritedProperty("color");
	color = StyleParser::ParseColor(colorStr);

	// pre-space
	UpdatePreSpace();
}


DisplayType TextDisplayNode::DeclaredDisplayType()
{
	return Inline;
}


BlockableDisplayNode* TextDisplayNode::AsBlockableDisplayNode()
{
	return NULL;
}


Selection* TextDisplayNode::StartSelection()
{
	Glyph* firstGlyph = (glyphs->empty() ? NULL : glyphs->front());
	return new BetweenGlyphsSelection(this, NULL, firstGlyph, false);
}


Selection* TextDisplayNode::EndSelection()
{
	Glyph* lastGlyph = (glyphs->empty() ? NULL : glyphs->back());
	return new BetweenGlyphsSelection(this, lastGlyph, NULL, true);
}


DisplayNode* TextDisplayNode::FirstChild()
{
	return NULL;
}


DisplayNode* TextDisplayNode::LastChild()
{
	return NULL;
}


Font* TextDisplayNode::CurFont()
{
	return font;
}


void TextDisplayNode::InlineLayout(InlineLayoutContext* layoutContext)
{
	// sanity clause
	if (font == NULL) {
		UpdateProperties();
		if (NeedsLoad())
			Load(GetDisplayDirector());
		}
/***
	if (font == NULL)
		throw InternalException("TextDisplayNode::UpdateProperties() not called before layout.");
***/

	layoutContext->SetFont(font);
	for (GlyphList::iterator gp = glyphs->begin(); gp != glyphs->end(); ++gp) {
		Glyph* glyph = *gp;
		layoutContext->AddGlyph(glyph);
		}
}


void TextDisplayNode::InlineDraw(InlineDrawContext* context)
{
	// sanity clause
	if (font == NULL)
		throw InternalException("TextDisplayNode::UpdateProperties() not called before draw.");

	View* view = context->view;
	view->SetFont(font);
	view->SetHighColor(color);
	GlyphDrawContext
		glyphDrawContext(context->originX, context->originY, view, font,
		                 context->startY, context->endY);
	for (GlyphList::iterator gp = glyphs->begin(); gp != glyphs->end(); ++gp)
		(*gp)->Draw(&glyphDrawContext);
}


Selection* TextDisplayNode::InlineFindSelection(FindSelectionContext* context)
{
	int mouseY = context->mouseY - context->curY;
	int mouseX = context->mouseX - context->xIndent;
	int lineHeight = font->LineHeight();
	int lineBottom = 0;
	Glyph* prevGlyph = NULL;
	for (GlyphList::iterator gp = glyphs->begin(); gp != glyphs->end(); prevGlyph = *gp, ++gp) {
		Glyph* glyph = *gp;

		// see if we're even on the right line
		int glyphY = glyph->GetY();
		if (glyphY >= lineBottom) {
			// we're on a new line
			lineBottom = glyphY + lineHeight;
			if (mouseY < glyphY) {
				// clicked at end of line, presumably after a space
				Space* prevSpace = dynamic_cast<Space*>(prevGlyph);
				if (prevSpace)
					return new BetweenWordsSelection(this, prevSpace);
				else {
					// ...probably should never happen...
					return new BetweenGlyphsSelection(this, prevGlyph, glyph, false);
					}
				}
			}
		if (mouseY > lineBottom)
			continue;

		// see if the mouse is in the current glyph
		int glyphX = glyph->GetX();
		if (mouseX >= glyphX && mouseX < glyphX + glyph->Width(font)) {
			// we've found it!
			// is it a word?
			Word* word = dynamic_cast<Word*>(glyph);
			if (word) {
				// find out what character it's at
				int charIndex = font->CharIndexAtX(mouseX - glyphX, word->GetDisplayWord());
				charIndex = word->DisplayToDocOffset(charIndex);
				if (charIndex == 0)
					return new BetweenGlyphsSelection(this, prevGlyph, glyph, false);
				else if (charIndex == word->NumCharsInText()) {
					Glyph* nextGlyph = NULL;
					if (++gp != glyphs->end())
						nextGlyph = *gp;
					return new BetweenGlyphsSelection(this, glyph, nextGlyph, false);
					}
				else
					return new CharacterSelection(this, word, charIndex);
				}
			// ... a space?
			Space* space = dynamic_cast<Space*>(glyph);
			if (space)
				return new BetweenWordsSelection(this, space);
			// otherwise, we've got a problem...
			return NULL;
			}
		}

	// didn't find anything
	return NULL;
}


Destination* TextDisplayNode::InlineFindDestination(FindDestinationContext* context)
{
	// only GlyphsSelections are allowed here
	Selection* source = context->source;
	if (typeid(*source) != typeid(GlyphsSelection) && typeid(*source) != typeid(InlineElementSelection))
		return NULL;

	// search for a destination
	int mouseY = context->mouseY - context->curY;
	int mouseX = context->mouseX - context->xIndent;
	int lineHeight = font->LineHeight();
	int lineBottom = 0;
	Glyph* prevGlyph = NULL;
	for (GlyphList::iterator gp = glyphs->begin(); gp != glyphs->end(); prevGlyph = *gp, ++gp) {
		Glyph* glyph = *gp;

		// see if we're even on the right line
		int glyphY = glyph->GetY();
		if (glyphY >= lineBottom) {
			// we're on a new line
			lineBottom = glyphY + lineHeight;
			if (mouseY < glyphY) {
				// clicked at end of line, presumably after a space
				Space* prevSpace = dynamic_cast<Space*>(prevGlyph);
				if (prevSpace)
					return new SpaceDestination(source, this, prevSpace);
				else {
					// probably should never happen...
					return new BetweenGlyphsDestination(source, this, prevGlyph, glyph);
					}
				}
			}
		if (mouseY > lineBottom)
			continue;

		// see if the mouse is in the current glyph
		int glyphX = glyph->GetX();
		if (mouseX >= glyphX && mouseX < glyphX + glyph->Width(font)) {
			// we've found it!
			// is it a space?
			Space* space = dynamic_cast<Space*>(glyph);
			if (space)
				return new SpaceDestination(source, this, space);
			// is it before or after?
			if (mouseX > glyphX + glyph->Width(font) / 2) {
				// it's after
				++gp;
				Glyph* nextGlyph = (gp != glyphs->end() ? *gp : NULL);
				if (nextGlyph) {
					Space* nextSpace = dynamic_cast<Space*>(nextGlyph);
					if (nextSpace)
						return new SpaceDestination(source, this, nextSpace);
					}
				return new BetweenGlyphsDestination(source, this, glyph, nextGlyph);
				}
			else {
				// it's before
				Space* prevSpace = dynamic_cast<Space*>(prevGlyph);
				if (prevSpace)
					return new SpaceDestination(source, this, prevSpace);
				else
					return new BetweenGlyphsDestination(source, this, prevGlyph, glyph);
				}
			}
		}

	// didn't find anything
	return NULL;
}


Hotspot* TextDisplayNode::InlineFindHotspot(FindHotspotContext* context)
{
	return NULL;
}


bool TextDisplayNode::CanSelectAcross()
{
	return true;
}


GlyphRef TextDisplayNode::FirstSelectableGlyph()
{
	for (GlyphList::iterator gp = glyphs->begin(); gp != glyphs->end(); ++gp) {
		Glyph* glyph = *gp;
		if (!glyph->IsWhitespace())
			return GlyphRef(this, glyph);
		}

	return GlyphRef();
}


GlyphRef TextDisplayNode::LastSelectableGlyph()
{
	for (GlyphList::reverse_iterator gp = glyphs->rbegin(); gp != glyphs->rend(); ++gp) {
		Glyph* glyph = *gp;
		if (!glyph->IsWhitespace())
			return GlyphRef(this, glyph);
		}

	return GlyphRef();
}


GlyphRef TextDisplayNode::FirstGlyph()
{
	if (glyphs->empty())
		return GlyphRef();

	return GlyphRef(this, glyphs->front());
}


GlyphRef TextDisplayNode::LastGlyph()
{
	if (glyphs->empty())
		return GlyphRef();

	return GlyphRef(this, glyphs->back());
}


void TextDisplayNode::DataInserted(unsigned long offset, String arg)
{
	ClearGlyphs();
	CreateGlyphs();
	BlockableDisplayNode* enclosingLeafBlock = EnclosingLeafBlock();
	if (enclosingLeafBlock) {
		enclosingLeafBlock->BlockLayout();
		enclosingLeafBlock->RefreshAfter();
		}
}


void TextDisplayNode::DataDeleted(unsigned long offset, unsigned long count)
{
	ClearGlyphs();
	CreateGlyphs();
	BlockableDisplayNode* enclosingLeafBlock = EnclosingLeafBlock();
	if (enclosingLeafBlock) {
		enclosingLeafBlock->BlockLayout();
		enclosingLeafBlock->RefreshAfter();
		}
}


void TextDisplayNode::DataSet(String newData)
{
	ClearGlyphs();
	CreateGlyphs();
	BlockableDisplayNode* enclosingLeafBlock = EnclosingLeafBlock();
	if (enclosingLeafBlock) {
		enclosingLeafBlock->BlockLayout();
		enclosingLeafBlock->RefreshAfter();
		}
}


void TextDisplayNode::NodeDying()
{
	delete this;
}


Text* TextDisplayNode::GetText()
{
	return text;
}


unsigned long TextDisplayNode::TextOffset()
{
	// eventually, when sentence handling is added, one Text node will potentially
	// have several associated TextDisplayNodes.  When that happens, this method
	// will change.
	return 0;
}


int TextDisplayNode::OffsetOfGlyph(Glyph* whichGlyph)
{
	int offset = TextOffset();
	for (GlyphList::iterator gp = glyphs->begin(); gp != glyphs->end(); ++gp) {
		Glyph* glyph = *gp;
		if (glyph == whichGlyph)
			return offset;
		offset += glyph->NumCharsInText();
		}
	return -1;
}


Glyph* TextDisplayNode::GlyphBefore(Glyph* whichGlyph)
{
	Glyph* prevGlyph = NULL;
	for (GlyphList::iterator gp = glyphs->begin(); gp != glyphs->end(); ++gp) {
		Glyph* glyph = *gp;
		if (glyph == whichGlyph)
			return prevGlyph;
		prevGlyph = glyph;
		}
	return NULL;
}


Glyph* TextDisplayNode::GlyphAfter(Glyph* whichGlyph)
{
	for (GlyphList::iterator gp = glyphs->begin(); gp != glyphs->end(); ++gp) {
		Glyph* glyph = *gp;
		if (glyph == whichGlyph) {
			++gp;
			if (gp == glyphs->end())
				return NULL;
			return *gp;
			}
		}
	return NULL;
}


bool TextDisplayNode::IsPreSpace()
{
	return isPreSpace;
}


Selection* TextDisplayNode::GetSelectionAtOffset(int offset)
{
	Glyph* prevGlyph = NULL;
	for (GlyphList::iterator gp = glyphs->begin(); gp != glyphs->end() && offset >= 0; ++gp) {
		Glyph* glyph = *gp;
		if (offset == 0)
			return new BetweenGlyphsSelection(this, prevGlyph, glyph, true);
		int numChars = glyph->NumCharsInText();
		if (offset < numChars) {
			// it's in this glyph
			// is it a word?
			Word* word = dynamic_cast<Word*>(glyph);
			if (word)
				return new CharacterSelection(this, word, offset);
			// ...a space?
			Space* space = dynamic_cast<Space*>(glyph);
			if (space)
				return new BetweenWordsSelection(this, space);
			// ...otherwise we don't know what it is
			return NULL;
			}
		offset -= numChars;
		prevGlyph = glyph;
		}
	return new BetweenGlyphsSelection(this, prevGlyph, NULL, true);
}


Glyph* TextDisplayNode::GlyphAtPoint(BPoint point)
	// NOTE: returns the NON-WHITESPACE glyph nearest the point
{
	int lineHeight = font->LineHeight();
	int lineBottom = 0;
	Glyph* prevGlyph = NULL;
	for (GlyphList::iterator gp = glyphs->begin(); gp != glyphs->end(); ++gp) {
		Glyph* glyph = *gp;

		// see if we're even on the right line
		int glyphY = glyph->GetY();
		if (glyphY >= lineBottom) {
			// we're on a new line
			lineBottom = glyphY + lineHeight;
			if (point.y < glyphY) {
				// clicked at end of line
				return prevGlyph;
				}
			}
		if (point.y > lineBottom) {
			if (!glyph->IsWhitespace())
				prevGlyph = glyph;
			continue;
			}

		// see if the point is in the current glyph
		int glyphX = glyph->GetX();
		if (point.x >= glyphX && point.x < glyphX + glyph->Width(font)) {
			// we've found it!  return it, unless it's whitespace, in which case
			// return the previous non-whitespace glyph
			return (glyph->IsWhitespace() ? prevGlyph : glyph);
			}

		// continue
		if (!glyph->IsWhitespace())
			prevGlyph = glyph;
		}

	return prevGlyph;
}


Glyph* TextDisplayNode::GlyphAtOffset(unsigned long offset)
{
	for (GlyphList::iterator gp = glyphs->begin(); gp != glyphs->end(); ++gp) {
		Glyph* glyph = *gp;
		unsigned long glyphLen = glyph->NumCharsInText();

		if (offset < glyphLen)
			return glyph;

		offset -= glyphLen;
		}

	return NULL;
}


void TextDisplayNode::CreateGlyphs()
{
	String data = text->Data();
	const char* p = data.begin();
	const char* stopper = data.end();
	const char* glyphStart = p;
	int glyphType = NoGlyphType;
	int prevGlyphType = NoGlyphType;
	bool smartenWord = false;
	for (; ; ++p) {
		// get the next character
		int charGlyphType;
		if (p == stopper)
			charGlyphType = EndGlyphType;

		else {
			char c = *p;
			switch (c) {
				case ' ':
					charGlyphType = WhitespaceGlyph;
					break;

				case '\t':
					charGlyphType = (isPreSpace ? TabGlyph : WhitespaceGlyph);
					break;

				case '\n':
				case '\r':
					charGlyphType = (isPreSpace ? LineBreakGlyph : WhitespaceGlyph);
					break;

				case '.':
				case ',':
				case '!':
				case '?':
				case ')':
				case ']':
				case ';':
				case ':':
					charGlyphType = PunctGlyph;
					break;

				case '(':
				case '[':
					charGlyphType = OpenParenGlyph;
					break;

				case '"':
					charGlyphType = DoubleQuoteGlyph;
					break;

				case '\'':
					if (glyphType == NoGlyphType || glyphType == WhitespaceGlyph ||
							glyphType == OpenParenGlyph) {
						charGlyphType = SingleQuoteGlyph;
						}
					else {
						charGlyphType = WordGlyph;
						smartenWord = true;
						}
					break;

				case '-':
					++p;
					if (p < stopper && *p == '-') {
						// double-hyphen--smarten it into an m-dash
						charGlyphType = MDashGlyph;
						--p;
						}
					else {
						--p;
						charGlyphType = WordGlyph;
						}
					break;

				default:
					charGlyphType = WordGlyph;
					break;
				}
			}

		if (charGlyphType != glyphType && glyphType != NoGlyphType) {
			if (glyphType == WordGlyph && smartenWord)
				glyphType = SmartenedWordGlyph;
			else if (glyphType == WhitespaceGlyph && isPreSpace)
				glyphType = PreSpaceGlyph;
			AddGlyph(string_slice(glyphStart, p), glyphType, prevGlyphType);
			prevGlyphType = glyphType;
			glyphType = NoGlyphType;
			smartenWord = false;
			}
		switch (charGlyphType) {
			case EndGlyphType:
				return;
			case WordGlyph:
			case WhitespaceGlyph:
				// accumulate characters into the glyph
				if (glyphType == NoGlyphType)
					glyphStart = p;
				glyphType = charGlyphType;
				break;
			case MDashGlyph:
				// handle this a little specially, since "p" is pointing at two
				// characters instead of just one
				AddGlyph(string_slice(p, p + 2), charGlyphType, prevGlyphType);
				prevGlyphType = MDashGlyph;
				p++;	// consume second '-'
				break;
			default:
				AddGlyph(string_slice(p, p + 1), charGlyphType, prevGlyphType);
				prevGlyphType = charGlyphType;
				break;
			}
		}
}


void TextDisplayNode::AddGlyph(string_slice glyphText, int glyphType, int prevGlyphType)
{
	if (glyphType == NoGlyphType || glyphText.empty())
		return;

	bool opening;
	switch (glyphType) {
		case WordGlyph:
		case PunctGlyph:
		case OpenParenGlyph:
			glyphs->push_back(new Word(glyphText, (glyphType != WordGlyph)));
			break;

		case WhitespaceGlyph:
			glyphs->push_back(new Space(glyphText));
			break;

		case PreSpaceGlyph:
			glyphs->push_back(new PreSpace(glyphText));
			break;

		case LineBreakGlyph:
			glyphs->push_back(new LineBreak(glyphText));
			break;

		case TabGlyph:
			glyphs->push_back(new Tab());
			break;

		case SmartenedWordGlyph:
			{
			bool trailingSingleQuote = false;
			if (glyphText[glyphText.length() - 1] == '\'') {
				// trailing "'" is a single-quote, not an apostrophe
				glyphText.resize(glyphText.length() - 1);
				trailingSingleQuote = true;
				}
			if (!glyphText.empty())
				glyphs->push_back(new SmartenedWord(glyphText));
			if (trailingSingleQuote)
				glyphs->push_back(new SmartSingleQuote(false));
			}
			break;

		case MDashGlyph:
			glyphs->push_back(new SmartMDash());
			break;

		case SingleQuoteGlyph:
			opening =
				(prevGlyphType == NoGlyphType || prevGlyphType == WhitespaceGlyph ||
				 prevGlyphType == OpenParenGlyph);
			glyphs->push_back(new SmartSingleQuote(opening));
			break;

		case DoubleQuoteGlyph:
			opening =
				(prevGlyphType == NoGlyphType || prevGlyphType == WhitespaceGlyph ||
				 prevGlyphType == OpenParenGlyph);
			glyphs->push_back(new SmartDoubleQuote(opening));
			break;
		}
}


void TextDisplayNode::ClearGlyphs()
{
	for (GlyphList::iterator gp = glyphs->begin(); gp != glyphs->end(); ++gp)
		delete *gp;
	glyphs->clear();
}


void TextDisplayNode::UpdatePreSpace()
{
	bool newPreSpace = (GetInheritedProperty("white-space") == "pre");
	if (isPreSpace != newPreSpace)
		needsLoad = true;
	isPreSpace = newPreSpace;
}


