/* StoredNamedNodeMap.h */

#ifndef _H_StoredNamedNodeMap_
#define _H_StoredNamedNodeMap_

#include "NamedNodeMap.h"

class NodeMapMap;
class NodeMapMapIter;

class StoredNamedNodeMap : public NamedNodeMap {
public:
	StoredNamedNodeMap();
	~StoredNamedNodeMap();
	Node*        	GetNamedItem(String name);
	Node*        	SetNamedItem(Node* arg);
	Node*        	RemoveNamedItem(String name);
	Node*        	Item(unsigned long index);
	unsigned long	Length();
	NamedNodeMap*	Clone();

protected:
	NodeMapMap*    	map;
	NodeMapMapIter*	iter;
	unsigned long  	iterIndex;
};


#endif
