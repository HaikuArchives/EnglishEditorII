/* GlyphRef.h */

#ifndef _H_GlyphRef_
#define _H_GlyphRef_


class TextDisplayNode;
class Glyph;
class DisplayNode;

class GlyphRef {
public:
	GlyphRef();
	GlyphRef(TextDisplayNode* textNodeIn, Glyph* glyphIn);
	void            	Set(TextDisplayNode* textNodeIn, Glyph* glyphIn);
	TextDisplayNode*	GetTextNode() const;
	Glyph*          	GetGlyph() const;
	bool            	IsValid() const;
	DisplayNode*    	CommonAncestorWith(const GlyphRef& otherGlyph) const;
	unsigned long   	StartOffset();
	unsigned long   	EndOffset();
	bool            	operator==(const GlyphRef& other);
	bool            	operator!=(const GlyphRef& other);

	TextDisplayNode*	textNode;
	Glyph*          	glyph;
};


#endif
