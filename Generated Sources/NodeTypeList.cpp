/* NodeTypeList.cpp */

#include "NodeTypeList.h"
#include "Node.h"
#include "Text.h"
#include "InternalException.h"
#include <vector>

class NodeTypeInternalList : public vector<DOMString> {
public:
	int	refCount;
};



NodeTypeList::NodeTypeList()
	: curIndex(-1)
{
	nodeTypes = new NodeTypeInternalList();
	nodeTypes->refCount = 1;
}


NodeTypeList::NodeTypeList(const NodeTypeList& other)
{
	nodeTypes = other.nodeTypes;
	nodeTypes->refCount += 1;
}


NodeTypeList::~NodeTypeList()
{
	if (--nodeTypes->refCount == 0)
		delete nodeTypes;
}


void NodeTypeList::AppendNode(DOMString nodeType)
{
	nodeTypes->push_back(nodeType);
}


void NodeTypeList::AppendNode(Node* node)
{
	DOMString nodeName;
	switch (node->NodeType()) {
		case ELEMENT_NODE:
			nodeName = node->NodeName();
			break;
		case TEXT_NODE:
			// don't add it if it's ignoreable whitespace
			if (!dynamic_cast<Text*>(node)->IsIgnorableWhitespace())
				nodeName = "#text";
			break;
		case CDATA_SECTION_NODE:
		case ENTITY_REFERENCE_NODE:
			nodeName = "#text";
			break;
		case PROCESSING_INSTRUCTION_NODE:
		case COMMENT_NODE:
			// don't add these
			break;
		default:
			// all others mean there's something wrong
			throw InternalException("NodeTypeList::AppendNode:  Attempt to add illegal node type.");
			break;
		}

	if (!nodeName.empty())
		AppendNode(nodeName);
}


int NodeTypeList::NumNodeTypes()
{
	return nodeTypes->size();
}


DOMString NodeTypeList::NodeTypeAt(int index)
{
	return (*nodeTypes)[index];
}


void NodeTypeList::Rewind()
{
	curIndex = (nodeTypes->size() > 0 ? 0 : -1);
}


void NodeTypeList::Step()
{
	if (curIndex >= 0) {
		curIndex += 1;
		if (((unsigned int) curIndex) >= nodeTypes->size())
			curIndex = -1;
		}
}


bool NodeTypeList::IteratorValid()
{
	return (curIndex >= 0);
}


DOMString NodeTypeList::CurNodeType()
{
	if (curIndex < 0)
		throw InternalException("Attempt to access NodeTypeList::CurNodeType() with invalid iterator.");
	return (*nodeTypes)[curIndex];
}


