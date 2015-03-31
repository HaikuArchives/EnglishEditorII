/* UTF8Utils.h */

#ifndef _H_UTF8Utils_
#define _H_UTF8Utils_

#include "string_slice.h"

class UTF8Utils {
public:
	inline static unsigned int	BytesForChar(unsigned char byte);
	inline static bool        	IsFirstByte(char c);
	static unsigned int       	CharsInString(string_slice str);
	static unsigned int       	BytesForNChars(string_slice str, unsigned int n);
};

inline unsigned int UTF8Utils::BytesForChar(unsigned char byte)
{
	return (((0xE5000000 >> ((byte >> 3) & 0x1E)) & 3) + 1);
}



inline bool UTF8Utils::IsFirstByte(char c)
{
	return ((c & 0xC0) != 0x80);
}




#endif
