/* Glyph.h */

#ifndef _H_Glyph_
#define _H_Glyph_


class Font;
class GlyphDrawContext;
class TextDisplayNode;
class Selection;
class Action;

class Glyph {
public:
	Glyph();
	virtual~Glyph();
	inline void       	SetPosition(int newX, int newY, int newLine);
	void              	AdjustX(int xDelta);
	void              	AdjustY(int yDelta);
	virtual int       	Width(Font* font) = 0;
	virtual void      	UncacheWidth();
	virtual void      	Draw(GlyphDrawContext* context) = 0;
	virtual bool      	IsWhitespace();
	virtual bool      	IsPunctuation();
	virtual bool      	IsLineBreak();
	virtual bool      	IsPreSpace();
	virtual bool      	IsTab();
	virtual int       	NumCharsInText() = 0;
	virtual Action*   	DeleteFrontAction(TextDisplayNode* textNode) = 0;
	virtual Action*   	DeleteBackAction(TextDisplayNode* textNode) = 0;
	virtual Selection*	SelectionIntoFront(TextDisplayNode* textNode) = 0;
	virtual Selection*	SelectionIntoBack(TextDisplayNode* textNode) = 0;
	virtual bool      	ShouldBindBefore() = 0;
	virtual bool      	ShouldBindAfter() = 0;
	inline int        	GetX();
	inline int        	GetY();
	int               	GetLine();
	bool              	ShouldDraw(GlyphDrawContext* context);

protected:
	int	x;
	int	y;
	int	line;
};

inline void Glyph::SetPosition(int newX, int newY, int newLine)
{
	x = newX;
	y = newY;
	line = newLine;
}



inline int Glyph::GetX()
{
	return x;
}



inline int Glyph::GetY()
{
	return y;
}




#endif
