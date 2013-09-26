/* InlineableDisplayNode.h */

#ifndef _H_InlineableDisplayNode_
#define _H_InlineableDisplayNode_

#include "DisplayNode.h"
#include "GlyphRef.h"

class InlineLayoutContext;
class InlineDrawContext;
class FindSelectionContext;
class FindDestinationContext;
class FindHotspotContext;
class Selection;
class Destination;
class Hotspot;
class BlockableDisplayNode;

class InlineableDisplayNode : virtual public DisplayNode {
public:
	virtual void         	InlineLayout(InlineLayoutContext* layoutContext) = 0;
	virtual void         	InlineDraw(InlineDrawContext* drawContext) = 0;
	virtual Selection*   	InlineFindSelection(FindSelectionContext* context) = 0;
	virtual Destination* 	InlineFindDestination(FindDestinationContext* context) = 0;
	virtual Hotspot*     	InlineFindHotspot(FindHotspotContext* context) = 0;
	virtual bool         	CanSelectAcross() = 0;
	virtual GlyphRef     	FirstSelectableGlyph() = 0;
	virtual GlyphRef     	LastSelectableGlyph() = 0;
	virtual GlyphRef     	LastGlyph() = 0;
	BlockableDisplayNode*	EnclosingLeafBlock();
};


#endif
