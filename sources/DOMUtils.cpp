/* DOMUtils.cpp */

#include "DOMUtils.h"
#include "Node.h"
#include "Element.h"


DOMString DOMUtils::NodeContents(Node* node)
{
	if (node->NodeType() == TEXT_NODE)
		return node->NodeValue();

	DOMString contents;
	for (Node* child = node->FirstChild(); child; child = child->NextSibling()) {
		if (child->NodeType() == TEXT_NODE)
			contents += child->NodeValue();
		else
			contents += NodeContents(child);
		}
	return contents;
}


