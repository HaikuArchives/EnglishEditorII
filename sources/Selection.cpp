/* Selection.cpp */

#include "Selection.h"
#include "BlockableDisplayNode.h"
#include "Tweex.h"

const rgb_color Selection::selectionColor = { 0, 0xBB, 255, 128 };


Selection::~Selection()
{
}


void Selection::Promote(DisplayDirector* director)
{
	/* virtual.  Default: do nothing */
}


bool Selection::HandleSpecialKey(string_slice key, DisplayDirector* director)
{
	// default: don't handle
	return false;
}


bool Selection::CanCopy()
{
	// default: false
	return false;
}


DOMString Selection::GetXMLCopy()
{
	return DOMString();
}


bool Selection::CanPaste()
{
	// default: no
	return false;
}


void Selection::Paste(String pasteText, DisplayDirector* director)
{
}


bool Selection::ContainsPoint(CoordPoint point)
{
	return Bounds().Contains(point);
}


CoordPoint Selection::DragStartPoint(CoordPoint destPoint)
{
	/** pure virtual, but only for selections that can be dragged **/
	return CoordPoint();
}


CoordPoint Selection::BlockOrigin(DisplayNode* node)
{
	// find the enclosing block
	for (; node; node = node->Parent()) {
		if (node->GetDisplayType() == Block)
			break;
		}
	if (node == NULL)	// shouldn't happen
		return CoordPoint(0, 0);

	// return the block's origin
	BlockableDisplayNode* blockNode = dynamic_cast<BlockableDisplayNode*>(node);
	return CoordPoint(blockNode->LeftSpace(), blockNode->Top());
}


