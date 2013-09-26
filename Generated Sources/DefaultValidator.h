/* DefaultValidator.h */

#ifndef _H_DefaultValidator_
#define _H_DefaultValidator_

#include "Validator.h"

class DisplayDirector;

class DefaultValidator : public Validator {
public:
	DefaultValidator(DisplayDirector* directorIn);
	bool	AllowsContents(Element* parentNode, NodeTypeList nodeTypes);

protected:
	DisplayDirector*	director;
};


#endif
