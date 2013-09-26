/* GlyphRef.cpp */

#include "GlyphRef.h"
#include "TextDisplayNode.h"
#include "Glyph.h"



GlyphRef::GlyphRef()
	: textNode(NULL), glyph(NULL)
{
}


GlyphRef::GlyphRef(TextDisplayNode* textNodeIn, Glyph* glyphIn)
	: textNode(textNodeIn), glyph(glyphIn)
{
}


void GlyphRef::Set(TextDisplayNode* textNodeIn, Glyph* glyphIn)
{
	textNode = textNodeIn;
	glyph = glyphIn;
}


TextDisplayNode* GlyphRef::GetTextNode() const
{
	return textNode;
}


Glyph* GlyphRef::GetGlyph() const
{
	return glyph;
}


bool GlyphRef::IsValid() const
{
	return (textNode != NULL && glyph != NULL);
}


DisplayNode* GlyphRef::CommonAncestorWith(const GlyphRef& otherGlyph) const
{
	// special-case the common case, where both are part of the same textNode
	DisplayNode* firstNode = GetTextNode();
	DisplayNode* lastNode = otherGlyph.GetTextNode();
	if (firstNode == lastNode)
		return firstNode;

	// figure out the levels
	DisplayNode* node;
	int firstLevel = 0;
	for (node = firstNode; node; node = node->Parent())
		++firstLevel;
	int lastLevel = 0;
	for (node = lastNode; node; node = node->Parent())
		++lastLevel;

	// equalize the levels before we start the search
	int i;
	if (firstLevel > lastLevel) {
		for (i = firstLevel - lastLevel; i > 0; --i)
			firstNode = firstNode->Parent();
		}
	else if (lastLevel > firstLevel) {
		for (i = lastLevel - firstLevel; i > 0; --i)
			lastNode = lastNode->Parent();
		}

	// now step up the tree until we find the common ancestor
	while (firstNode != NULL) {
		if (firstNode == lastNode)
			return firstNode;
		firstNode = firstNode->Parent();
		lastNode = lastNode->Parent();
		}

	return NULL;
}


unsigned long GlyphRef::StartOffset()
{
	return textNode->OffsetOfGlyph(glyph);
}


unsigned long GlyphRef::EndOffset()
{
	return textNode->OffsetOfGlyph(glyph) + glyph->NumCharsInText();
}


bool GlyphRef::operator==(const GlyphRef& other)
{
	return (textNode == other.textNode && glyph == other.glyph);
}


bool GlyphRef::operator!=(const GlyphRef& other)
{
	return (textNode != other.textNode || glyph != other.glyph);
}


