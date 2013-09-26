/* Timing.cpp */

#include "Timing.h"
#include <OS.h>


Microseconds Timing::SystemTime()
{
	return system_time();
}


