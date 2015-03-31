/* Validator.cpp */

#include "Validator.h"
#include "Element.h"


bool Validator::CanInsertBefore(Element* parent, NodeTypeList newNodeTypes, Node* refChild)
{
	NodeTypeList allNodeTypes;

	// build "allNodeTypes" up to the insertion
	Node* curChild = NULL;
	for (curChild = parent->FirstChild(); curChild; curChild = curChild->NextSibling()) {
		if (curChild == refChild)
			break;
		allNodeTypes.AppendNode(curChild);
		}

	// add in the insertion
	for (newNodeTypes.Rewind(); newNodeTypes.IteratorValid(); newNodeTypes.Step())
		allNodeTypes.AppendNode(newNodeTypes.CurNodeType());

	// copy the remainder of the node types
	for (; curChild; curChild = curChild->NextSibling())
		allNodeTypes.AppendNode(curChild);

	// validate
	return AllowsContents(parent, allNodeTypes);
}


