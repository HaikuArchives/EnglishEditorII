/* String.h */

#ifndef _H_String_
#define _H_String_

#include "string_slice.h"
#include "string_storage.h"		// so copy constructor can be inline

class String {
public:
	inline             	String();
	inline             	String(string_slice str);
	inline             	String(const char* str);
	inline             	String(const String& other);
	String(long val);
	inline~String();
	String&            	operator+=(string_slice str);
	String&            	operator+=(const String& other);
	String&            	operator+=(const char* str);
	String&            	operator=(const String& other);
	void               	insert(unsigned int pos, const String& other);
	void               	erase(unsigned int pos, unsigned int n);
	void               	erase();
	String             	substr(unsigned int pos, unsigned int n);
	String             	trim();
	String&            	detach();
	const char*        	c_str();
	const char*        	data();
	inline bool        	operator==(const String& other) const;
	inline bool        	operator==(string_slice other);
	inline bool        	operator!=(const String& other) const;
	inline bool        	operator!=(string_slice other) const;
	inline bool        	operator<(const String& other) const;
	inline bool        	operator<=(const String& other) const;
	inline bool        	operator>(const String& other) const;
	inline bool        	operator>=(const String& other) const;
	inline unsigned int	length() const;
	inline bool        	empty() const;
	inline             	operator string_slice();
	inline bool        	startsWith(const String& other);
	inline const char* 	begin();
	inline const char* 	end();

protected:
	string_slice   	slice;
	string_storage*	storage;
};

inline String::String()
	: storage(NULL)
{
}



inline String::String(string_slice str)
	: slice(str), storage(NULL)
{
}



inline String::String(const char* str)
	: slice(str), storage(NULL)
{
}



inline String::String(const String& other)
	: slice(other.slice), storage(other.storage)
{
	if (storage)
		storage->AddRef();
}



inline String::~String()
{
	if (storage)
		storage->ReleaseRef();
}



inline bool String::operator==(const String& other) const
{
	return (slice == other.slice);
}



inline bool String::operator==(string_slice other)
{
	return (slice == other);
}



inline bool String::operator!=(const String& other) const
{
	return (slice != other.slice);
}



inline bool String::operator!=(string_slice other) const
{
	return (slice != other);
}



inline bool String::operator<(const String& other) const
{
	return (slice < other.slice);
}



inline bool String::operator<=(const String& other) const
{
	return (slice < other.slice || slice == other.slice);
}



inline bool String::operator>(const String& other) const
{
	return !(slice < other.slice || slice == other.slice);
}



inline bool String::operator>=(const String& other) const
{
	return !(slice < other.slice);
}



inline unsigned int String::length() const
{
	return slice.length();
}



inline bool String::empty() const
{
	return (slice.length() == 0);
}



inline String::operator string_slice()
{
	return slice;
}



inline bool String::startsWith(const String& other)
{
	return slice.startsWith(other.slice);
}



inline const char* String::begin()
{
	return slice.begin();
}



inline const char* String::end()
{
	return slice.end();
}




#endif
