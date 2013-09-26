/* OSException.h */

#ifndef _H_OSException_
#define _H_OSException_

#include "DisplayableException.h"
#include "string_slice.h"

#include <SupportDefs.h>

class OSException : public DisplayableException {
public:
	OSException(string_slice descriptionIn, status_t errIn);
	string_slice	GetDescription();
	status_t    	GetError();
	void        	Display();

protected:
	string_slice	description;
	status_t    	err;
};


#endif
