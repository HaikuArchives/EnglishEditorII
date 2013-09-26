/* BlockableDisplayNode.h */

#ifndef _H_BlockableDisplayNode_
#define _H_BlockableDisplayNode_

#include "DisplayNode.h"

class BlockDrawContext;
class FindSelectionContext;
class FindDestinationContext;
class FindHotspotContext;
class Selection;
class Destination;
class Hotspot;
class Node;

class BlockableDisplayNode : virtual public DisplayNode {
public:
	BlockableDisplayNode(DisplayNode* parentIn);
	virtual int                  	Width();
	virtual int                  	LeftSpace();
	virtual void                 	BlockLayout(bool force = false) = 0;
	virtual void                 	BlockDraw(BlockDrawContext* context) = 0;
	virtual Selection*           	BlockFindSelection(FindSelectionContext* context) = 0;
	virtual Destination*         	BlockFindDestination(FindDestinationContext* context) = 0;
	virtual Hotspot*             	BlockFindHotspot(FindHotspotContext* context) = 0;
	virtual int                  	TopMargin() = 0;
	virtual int                  	BottomMargin() = 0;
	virtual int                  	GetChildTop(DisplayNode* child) = 0;
	virtual void                 	CalcVerticalMargins() = 0;
	virtual int                  	MaxLineWidth() = 0;
	virtual Node*                	FirstNode() = 0;
	virtual Node*                	LastNode() = 0;
	virtual BlockableDisplayNode*	AsBlockableDisplayNode();
	inline int                   	Height();
	int                          	Top();
	int                          	Bottom();
	int                          	DisplayWidth();
	int                          	DisplayLeft();
	void                         	HeightChangedBy(int heightDelta);
	void                         	HeightChangedTo(int newHeight);
	void                         	VerticalMarginsChanged();
	void                         	RefreshAfter();

protected:
	int	height;
};

inline int BlockableDisplayNode::Height()
{
	return height;
}




#endif
