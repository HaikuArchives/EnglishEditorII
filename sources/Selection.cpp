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


String Selection::GetXMLCopy()
{
	return String();
}


bool Selection::CanPaste()
{
	// default: no
	return false;
}


void Selection::Paste(String pasteText, DisplayDirector* director)
{
}


bool Selection::ContainsPoint(BPoint point)
{
	return Bounds().Contains(point);
}


BPoint Selection::DragStartPoint(BPoint destPoint)
{
	/** pure virtual, but only for selections that can be dragged **/
	return BPoint();
}


BPoint Selection::BlockOrigin(DisplayNode* node)
{
	// find the enclosing block
	for (; node; node = node->Parent()) {
		if (node->GetDisplayType() == Block)
			break;
		}
	if (node == NULL)	// shouldn't happen
		return BPoint(0, 0);

	// return the block's origin
	BlockableDisplayNode* blockNode = dynamic_cast<BlockableDisplayNode*>(node);
	return BPoint(blockNode->LeftSpace(), blockNode->Top());
}


