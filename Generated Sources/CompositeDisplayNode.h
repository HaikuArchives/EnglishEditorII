/* CompositeDisplayNode.h */

#ifndef _H_CompositeDisplayNode_
#define _H_CompositeDisplayNode_

#include "BlockableDisplayNode.h"
#include "InlineableDisplayNode.h"
#include "DOMChangeClient.h"

#include "Timing.h"

class Element;

class CompositeDisplayNode : public BlockableDisplayNode, public InlineableDisplayNode, public DOMChangeClient {
public:
	CompositeDisplayNode(DisplayNode* parentIn);
	~CompositeDisplayNode();
	DisplayType 	GetDisplayType();
	bool        	IsLeafBlock();
	void        	UpdateProperties();
	Selection*  	StartSelection();
	Selection*  	EndSelection();
	DisplayNode*	FirstChild();
	DisplayNode*	LastChild();
	void        	BlockLayout(bool force = false);
	void        	BlockDraw(BlockDrawContext* context);
	Selection*  	BlockFindSelection(FindSelectionContext* context);
	Destination*	BlockFindDestination(FindDestinationContext* context);
	Hotspot*    	BlockFindHotspot(FindHotspotContext* context);
	int         	TopMargin();
	int         	BottomMargin();
	int         	GetChildTop(DisplayNode* child);
	void        	CalcVerticalMargins();
	int         	MaxLineWidth();
	void        	InlineLayout(InlineLayoutContext* layoutContext);
	void        	InlineDraw(InlineDrawContext* context);
	Selection*  	InlineFindSelection(FindSelectionContext* context);
	Destination*	InlineFindDestination(FindDestinationContext* context);
	Hotspot*    	InlineFindHotspot(FindHotspotContext* context);
	GlyphRef    	FirstSelectableGlyph();
	GlyphRef    	LastSelectableGlyph();
	GlyphRef    	LastGlyph();
	void        	ChildInsertedBefore(Node* newChild, Node* refChild);
	void        	ChildRemoved(Node* oldChild);
	void        	ChildReplaced(Node* newChild, Node* oldChild);
	void        	ClearChildren();
	void        	LoadFromElement(Element* element, DisplayDirector* director);
	void        	LoadFromContentsTemplate(Element* templateElement, Element* sourceElement, DisplayDirector* director);
	void        	LoadChildren(DisplayDirector* director);
	void        	LayoutChildBlocks(bool force);
	void        	LayoutInlineChildren();
	void        	DrawChildBlocks(BlockDrawContext* context);
	void        	DrawInlineChildren(BlockDrawContext* context);
	Selection*  	FindSelectionInChildBlocks(FindSelectionContext* context);
	Selection*  	FindSelectionInInlineChildren(FindSelectionContext* context);
	Destination*	FindDestinationInChildBlocks(FindDestinationContext* context);
	Destination*	FindDestinationInInlineChildren(FindDestinationContext* context);
	Hotspot*    	FindHotspotInChildBlocks(FindHotspotContext* context);
	Hotspot*    	FindHotspotInInlineChildren(FindHotspotContext* context);
	void        	CheckIfBlock();
	void        	UnlinkChildren(DisplayNode* firstToUnlink, DisplayNode* lastToUnlink);
	void        	LinkChildrenBefore(DisplayNode* firstToLink, DisplayNode* lastToLink, DisplayNode* beforeChild);
	void        	AppendChildren(DisplayNode* firstToLink, DisplayNode* lastToLink);
	void        	AppendChild(DisplayNode* newChild);

protected:
	DisplayNode*	firstChild;
	DisplayNode*	lastChild;
	bool        	isBlock;
	bool        	isLeafBlock;
	int         	topMargin;
	int         	bottomMargin;
	int         	maxLineWidth;
};


#endif
