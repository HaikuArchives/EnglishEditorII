/* StoredNamedNodeMap.cpp */

#include "StoredNamedNodeMap.h"
#include "Node.h"
#include <map>

class NodeMapMap : public map<String, Node*> {};
class NodeMapMapIter : public NodeMapMap::iterator {
public:
	NodeMapMapIter(const NodeMapMap::iterator& otherIter)
		: NodeMapMap::iterator(otherIter) {}	
};



StoredNamedNodeMap::StoredNamedNodeMap()
	: iter(NULL)
{
	map = new NodeMapMap();
}


StoredNamedNodeMap::~StoredNamedNodeMap()
{
	delete iter;
	for (NodeMapMap::iterator n = map->begin(); n != map->end(); n++)
		delete (*n).second;
	delete map;
}


Node* StoredNamedNodeMap::GetNamedItem(String name)
{
	return (*map)[name];
}


Node* StoredNamedNodeMap::SetNamedItem(Node* arg)
{
	string_slice name = arg->NodeName();
	Node* oldValue = (*map)[name];
	(*map)[name] = arg;
	return oldValue;
}


Node* StoredNamedNodeMap::RemoveNamedItem(String name)
{
	Node* item = (*map)[name];
	map->erase(name);
	return item;
}


Node* StoredNamedNodeMap::Item(unsigned long index)
{
	// map is deficient and doesn't support random access (nor does map::iterator),
	// even though that's not hard to do with balanced trees.  However, a common
	// use for NamedNodeMap::Item() is to enumerate all the items of the NamedNodeMap.
	// We optimize that case by keeping an iterator and using it for cheap access
	// to the last-accessed item, or the item before or after that.
	if (iter) {
		if (index == iterIndex)
			return (**iter).second;
		else if (index == iterIndex + 1) {
			++iterIndex;
			return (*++*iter).second;
			}
		else if (index == iterIndex - 1) {
			--iterIndex;
			return (*--*iter).second;
			}

		// couldn't optimize this access, so throw away "iter" before we build a
		// new one
		delete iter;
		}

	// the expensive way
	iter = new NodeMapMapIter(map->begin());
	iterIndex = 0;
	for (; *iter != map->end(); ++*iter, ++iterIndex) {
		if (index == iterIndex)
			return (**iter).second;
		}

	// didn't find it
	delete iter;	// not of much use, so get rid of it
	iter = NULL;
	return NULL;

/***
	// ridiculously inefficient because map is deficient
	for (NodeMapMap::iterator n = map->begin(); n != map->end(); n++) {
		if (index-- > 0)
			continue;
		return (*n).second;		// Metrowerks doesn't like "n->second"
		}
	return NULL;
***/
}


unsigned long StoredNamedNodeMap::Length()
{
	return map->size();
}


NamedNodeMap* StoredNamedNodeMap::Clone()
{
	StoredNamedNodeMap* clone = new StoredNamedNodeMap();

	for (NodeMapMap::iterator n = map->begin(); n != map->end(); n++) {
		Node* clonedNode = (*n).second->CloneNode(true);
		clone->SetNamedItem(clonedNode);
		}

	return clone;
}


