/* PreSpace.h */

#ifndef _H_PreSpace_
#define _H_PreSpace_

#include "Word.h"

class PreSpace : public Word {
public:
	PreSpace(String textIn);
	bool	IsWhitespace();
	bool	IsPreSpace();
};


#endif
