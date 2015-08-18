/* QuoterDict.cpp */

#include "QuoterDict.h"
#include "Node.h"
#include <vector>

/*
	We don't expect there to be too many different quoters in a message, so
	we just use linear search for this instead of trying to be clever.
*/

struct QuoterItem {
	string_slice	quoter;
	Node*       	attribution;

	QuoterItem(string_slice quoterIn, Node* attributionIn)
		: quoter(quoterIn), attribution(attributionIn) {}
	~QuoterItem()
		{ delete attribution; }
	void	SetAttribution(Node* newAttribution)
				{ delete attribution; attribution = newAttribution; }
};

class QuoterList : public std::vector<QuoterItem*> {};



QuoterDict::QuoterDict()
{
	dict = new QuoterList();
}


QuoterDict::~QuoterDict()
{
	for (QuoterList::iterator q = dict->begin(); q != dict->end(); ++q)
		delete *q;
	delete dict;
}


void QuoterDict::SetAttributionFor(string_slice quoter, Node* attribution)
{
	// see if it's in the dict yet
	int numQuoters = dict->size();
	for (int i=0; i<numQuoters; i++) {
		QuoterItem* item = (*dict)[i];
		if (item->quoter == quoter) {
			item->SetAttribution(attribution);
			return;
			}
		}

	// not there; add it
	dict->push_back(new QuoterItem(quoter, attribution));
}


Node* QuoterDict::GetAttributionFor(string_slice quoter)
{
	// find it
	int numQuoters = dict->size();
	for (int i=0; i<numQuoters; i++) {
		QuoterItem* item = (*dict)[i];
		if (item->quoter == quoter)
			return item->attribution;
		}

	// didn't find it
	return NULL;
}


