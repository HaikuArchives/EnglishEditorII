/* DisplayTextNode.cpp */

#include "DisplayTextNode.h"


DisplayTextNode::DisplayTextNode(Text* textIn)
	: TextDisplayNode(textIn)
{
}


bool DisplayTextNode::IsDisposable()
{
	return true;
}


Selection* DisplayTextNode::StartSelection()
{
	return NULL;
}


Selection* DisplayTextNode::EndSelection()
{
	return NULL;
}


bool DisplayTextNode::CanSelectAcross()
{
	return false;
}


Selection* DisplayTextNode::InlineFindSelection(FindSelectionContext* context)
{
	return NULL;
}


