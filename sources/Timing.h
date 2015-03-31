/* Timing.h */

#ifndef _H_Timing_
#define _H_Timing_

#include <SupportDefs.h>

typedef bigtime_t Microseconds;

class Timing {
public:
	static Microseconds	SystemTime();
};


#endif
