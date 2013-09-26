/* InlineDrawContext.cpp */

#include "InlineDrawContext.h"
#include "BlockDrawContext.h"



InlineDrawContext::InlineDrawContext(BlockDrawContext* blockContext, int extraIndent)
	: view(blockContext->view), originX(blockContext->originX + extraIndent),
	  originY(blockContext->originY + blockContext->curY),
	  startY(blockContext->startY + blockContext->originY),
	  endY(blockContext->endY + blockContext->originY)
{
}


