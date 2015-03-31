/* InlineableDisplayNode.cpp */

#include "InlineableDisplayNode.h"
#include "BlockableDisplayNode.h"


BlockableDisplayNode* InlineableDisplayNode::EnclosingLeafBlock()
{
	for (DisplayNode* node = this; node; node = node->Parent()) {
		if (node->GetDisplayType() == Block)
			return dynamic_cast<BlockableDisplayNode*>(node);
		}
	return NULL;
}


