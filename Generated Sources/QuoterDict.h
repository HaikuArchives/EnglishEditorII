/* QuoterDict.h */

#ifndef _H_QuoterDict_
#define _H_QuoterDict_

#include "string_slice.h"

class Node;
class QuoterList;

class QuoterDict {
public:
	QuoterDict();
	~QuoterDict();
	void 	SetAttributionFor(string_slice quoter, Node* attribution);
	Node*	GetAttributionFor(string_slice quoter);

protected:
	QuoterList*	dict;
};


#endif
