/* DisplayContext.h */

#ifndef _H_DisplayContext_
#define _H_DisplayContext_


class DisplayDirector;

class DisplayContext {
public:
	DisplayContext(DisplayDirector* directorIn) : director(directorIn) {}

	DisplayDirector*	director;
};


#endif
