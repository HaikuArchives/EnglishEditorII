/* TextSpan.h */

#ifndef _H_TextSpan_
#define _H_TextSpan_

#include "GlyphRef.h"

class Text;

#ifndef NULL
#define NULL	(0)
#endif

class TextSpan {
public:
	inline       	TextSpan(Text* textIn, unsigned long offsetIn, unsigned long lengthIn);
	TextSpan(GlyphRef firstGlyph, GlyphRef lastGlyph);
	TextSpan(GlyphRef glyph);
	inline       	TextSpan();
	unsigned long	EndOffset();
	bool         	IsJustBefore(const TextSpan& otherSpan);
	inline bool  	IsEmpty();
	bool         	IsValid();
	inline void  	Set(Text* newText, unsigned long newOffset, unsigned long newLength);
	void         	Set(GlyphRef firstGlyph, GlyphRef lastGlyph);
	void         	Set(GlyphRef glyph);
	void         	Clear();
	void         	Append(const TextSpan& other);
	void         	AdjustForInsert(Text* insertText, unsigned long insertOffset, unsigned long insertLength);
	void         	AdjustForDelete(Text* deleteText, unsigned long deleteOffset, unsigned long deleteLength);

	Text*        	text;
	unsigned long	offset;
	unsigned long	length;
};

inline TextSpan::TextSpan(Text* textIn, unsigned long offsetIn, unsigned long lengthIn)
	: text(textIn), offset(offsetIn), length(lengthIn)
{
}



inline TextSpan::TextSpan()
	: text(NULL), offset(0), length(0)
{
}



inline bool TextSpan::IsEmpty()
{
	return (length == 0);
}



inline void TextSpan::Set(Text* newText, unsigned long newOffset, unsigned long newLength)
{
	text = newText;
	offset = newOffset;
	length = newLength;
}




#endif
