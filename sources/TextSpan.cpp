/* TextSpan.cpp */

#include "TextSpan.h"
#include "TextDisplayNode.h"
#include "Text.h"
#include "Glyph.h"
#include "InternalException.h"



TextSpan::TextSpan(GlyphRef firstGlyph, GlyphRef lastGlyph)
{
	text = firstGlyph.GetTextNode()->GetText();
	if (useAssertions && text != lastGlyph.GetTextNode()->GetText())
		throw InternalException("TextSpan ctor: glyphs are not in the same Text.");
	offset = firstGlyph.GetTextNode()->OffsetOfGlyph(firstGlyph.GetGlyph());
	unsigned long lastStartOffset =
		lastGlyph.GetTextNode()->OffsetOfGlyph(lastGlyph.GetGlyph());
	length = lastStartOffset + lastGlyph.GetGlyph()->NumCharsInText() - offset;
}


TextSpan::TextSpan(GlyphRef glyph)
{
	text = glyph.GetTextNode()->GetText();
	offset = glyph.GetTextNode()->OffsetOfGlyph(glyph.GetGlyph());
	length = glyph.GetGlyph()->NumCharsInText();
}


unsigned long TextSpan::EndOffset()
{
	return offset + length;
}


bool TextSpan::IsJustBefore(const TextSpan& otherSpan)
{
	return (text == otherSpan.text && otherSpan.offset == offset + length);
}


bool TextSpan::IsValid()
{
	return (text != NULL);
}


void TextSpan::Set(GlyphRef firstGlyph, GlyphRef lastGlyph)
{
	text = firstGlyph.GetTextNode()->GetText();
	if (useAssertions && text != lastGlyph.GetTextNode()->GetText())
		throw InternalException("TextSpan::Set: glyphs are not in the same Text.");
	offset = firstGlyph.GetTextNode()->OffsetOfGlyph(firstGlyph.GetGlyph());
	unsigned long lastStartOffset =
		lastGlyph.GetTextNode()->OffsetOfGlyph(lastGlyph.GetGlyph());
	length = lastStartOffset + lastGlyph.GetGlyph()->NumCharsInText() - offset;
}


void TextSpan::Set(GlyphRef glyph)
{
	text = glyph.GetTextNode()->GetText();
	offset = glyph.GetTextNode()->OffsetOfGlyph(glyph.GetGlyph());
	length = glyph.GetGlyph()->NumCharsInText();
}


void TextSpan::Clear()
{
	text = NULL;
	offset = length = 0;
}


void TextSpan::Append(const TextSpan& other)
{
	// sanity clauses
	if (useAssertions && other.text != text)
		throw InternalException("TextSpan::Append: Not the same Text.");
	if (useAssertions && other.offset != offset + length)
		throw InternalException("TextSpan::Append: Other span is not just after this one.");

	// append
	length += other.length;
}


void TextSpan::AdjustForInsert(Text* insertText, unsigned long insertOffset, unsigned long insertLength)
{
	if (text == insertText) {
		if (offset >= insertOffset)
			offset += insertLength;
		else if (insertOffset < offset + length)
			length += insertLength;
		}
}


void TextSpan::AdjustForDelete(Text* deleteText, unsigned long deleteOffset, unsigned long deleteLength)
{
	if (text == deleteText) {
		if (offset > deleteOffset)
			offset -= deleteLength;
		else if (deleteOffset < offset + length) {
			deleteOffset -= offset;	// convert to relative position for convenience
			if (deleteOffset + deleteLength > length)
				length = deleteOffset;
			else
				length -= deleteLength;
			}
		}
}


