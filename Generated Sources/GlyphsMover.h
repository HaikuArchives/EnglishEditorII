/* GlyphsMover.h */

#ifndef _H_GlyphsMover_
#define _H_GlyphsMover_

#include "TextSpan.h"

class Action;
class Node;
class DisplayNode;
class ElementDisplayNode;

class GlyphsMover {
public:
	GlyphsMover(GlyphRef srcFirstGlyph, GlyphRef srcLastGlyph,
            GlyphRef destPreGlyph, GlyphRef destSpaceGlyph, GlyphRef destPostGlyph,
            bool copy);
	GlyphsMover(ElementDisplayNode* displayNode,
            GlyphRef destPreGlyph, GlyphRef destSpaceGlyph, GlyphRef destPostGlyph,
            bool copy);
	Action*	MakeAction();
	void   	ConfigureDest(GlyphRef srcPreSpaceGlyph, GlyphRef srcFirstGlyph,
                   GlyphRef srcLastGlyph, GlyphRef srcPostSpaceGlyph,
                   GlyphRef destPreGlyph, GlyphRef destSpaceGlyph, GlyphRef destPostGlyph);
	Text*  	FirstTextIn(Node* node);
	Text*  	LastTextIn(Node* node);

protected:
	bool    	copying;
	bool    	singleSrcText;
	bool    	hasNodes;
	TextSpan	srcPreSpace;
	TextSpan	srcFirstText;
	TextSpan	srcLastText;
	TextSpan	srcPostSpace;
	TextSpan	destFirstText;
	TextSpan	destDeleteText;
	TextSpan	destLastText;
	bool    	takePreSpace;
	bool    	takePostSpace;
	bool    	keepSrcPreSpace;
	bool    	keepSrcPostSpace;
	bool    	addPreSpace;
	bool    	addPostSpace;
	bool    	addSrcSpace;
	Node*   	firstMoveNode;
	Node*   	lastMoveNode;
};


#endif
