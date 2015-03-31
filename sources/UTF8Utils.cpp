/* UTF8Utils.cpp */

#include "UTF8Utils.h"


unsigned int UTF8Utils::CharsInString(string_slice str)
{
	unsigned int numChars = 0;
	string_slice::const_iterator stopper = str.end();
	for (string_slice::const_iterator p = str.begin(); p != stopper; numChars++)
		p += BytesForChar(*p);
	return numChars;
}


unsigned int UTF8Utils::BytesForNChars(string_slice str, unsigned int n)
{
	unsigned int numBytes = 0;
	unsigned int len = str.length();
	while (n > 0 && numBytes < len) {
		numBytes += BytesForChar(str[numBytes]);
		n -= 1;
		}
	return numBytes;
}


