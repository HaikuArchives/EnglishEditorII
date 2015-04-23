/* string_storage.h */

#ifndef _H_string_storage_
#define _H_string_storage_

#include "qstring.h"

class string_storage : public qstring {
public:
	inline     	string_storage(string_slice str);
	inline void	AddRef();
	inline void	ReleaseRef();
	inline int 	NumRefs();

protected:
	int	refCount;
};

inline string_storage::string_storage(string_slice str)
	: qstring(str), refCount(1)
{
}



inline void string_storage::AddRef()
{
	refCount += 1;
}



inline void string_storage::ReleaseRef()
{
	if (--refCount <= 0)
		delete this;
}



inline int string_storage::NumRefs()
{
	return refCount;
}




#endif
