/* TextDisplayNode.h */

#ifndef _H_TextDisplayNode_
#define _H_TextDisplayNode_

#include <GraphicsDefs.h>

#include "InlineableDisplayNode.h"
#include "DOMChangeClient.h"
#include <Point.h>

class Text;
class Text;
class GlyphList;
class Glyph;
class Selection;

class TextDisplayNode : public InlineableDisplayNode, public DOMChangeClient {
public:
	TextDisplayNode(Text* textIn);
	~TextDisplayNode();
	DisplayType          	GetDisplayType();
	bool                 	IsLeafBlock();
	bool                 	IsDisposable();
	void                 	Load(DisplayDirector* director);
	void                 	UpdateProperties();
	DisplayType          	DeclaredDisplayType();
	BlockableDisplayNode*	AsBlockableDisplayNode();
	Selection*           	StartSelection();
	Selection*           	EndSelection();
	DisplayNode*         	FirstChild();
	DisplayNode*         	LastChild();
	Font*                	CurFont();
	void                 	InlineLayout(InlineLayoutContext* layoutContext);
	void                 	InlineDraw(InlineDrawContext* context);
	Selection*           	InlineFindSelection(FindSelectionContext* context);
	Destination*         	InlineFindDestination(FindDestinationContext* context);
	Hotspot*             	InlineFindHotspot(FindHotspotContext* context);
	bool                 	CanSelectAcross();
	GlyphRef             	FirstSelectableGlyph();
	GlyphRef             	LastSelectableGlyph();
	GlyphRef             	FirstGlyph();
	GlyphRef             	LastGlyph();
	void                 	DataInserted(unsigned long offset, String arg);
	void                 	DataDeleted(unsigned long offset, unsigned long count);
	void                 	DataSet(String newData);
	void                 	NodeDying();
	Text*                	GetText();
	unsigned long        	TextOffset();
	int                  	OffsetOfGlyph(Glyph* whichGlyph);
	Glyph*               	GlyphBefore(Glyph* whichGlyph);
	Glyph*               	GlyphAfter(Glyph* whichGlyph);
	bool                 	IsPreSpace();
	Selection*           	GetSelectionAtOffset(int offset);
	Glyph*               	GlyphAtPoint(BPoint point);
	Glyph*               	GlyphAtOffset(unsigned long offset);
	void                 	CreateGlyphs();
	void                 	AddGlyph(string_slice glyphText, int glyphType, int prevGlyphType);
	void                 	ClearGlyphs();
	void                 	UpdatePreSpace();

protected:
	Text*     	text;
	GlyphList*	glyphs;
	Font*     	font;
	rgb_color     	color;
	bool      	isPreSpace;
};


#endif
