/* InternalException.h */

#ifndef _H_InternalException_
#define _H_InternalException_

#include "DisplayableException.h"
#include "string_slice.h"

static const bool useAssertions = true;

class InternalException : public DisplayableException {
public:
	InternalException(string_slice descIn);
	string_slice	Description();
	void        	Display();

protected:
	string_slice	desc;
};


#endif
