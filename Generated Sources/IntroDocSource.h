/* IntroDocSource.h */

#ifndef _H_IntroDocSource_
#define _H_IntroDocSource_

#include "DocumentSource.h"

class IntroDocSource : public DocumentSource {
public:
	IntroDocSource();
	~IntroDocSource();
	Document*   	GetDocument();
	string_slice	GetWindowTitle();
	void        	Save();

protected:
	Document*	document;
	char*    	fileText;
};


#endif
