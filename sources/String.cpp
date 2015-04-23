/* String.cpp */

#include "String.h"
#include <stdio.h>	// for sprintf



String::String(long val)
{
	char buf[128];
	sprintf(buf, "%ld", val);
	storage = new string_storage(buf);
	slice = *storage;
}


String& String::operator+=(string_slice str)
{
	// optimize adding to empty string
	if (slice.begin() == slice.end()) {
		// also implies storage == NULL
		slice = str;
		}

	// optimize adding characters that already exists at the end of the slice
	else if (str.begin() == slice.end())
		slice = string_slice(slice.begin(), str.end());

	else if (storage) {
		detach();
		*storage += str;
		slice = *storage;
		}
	else {
		storage = new string_storage(slice);
		*storage += str;
		slice = *storage;
		}

	return *this;
}


String& String::operator+=(const String& other)
{
	if (slice.empty()) {
		// optimize adding to empty string
		// implies "storage == NULL"
		storage = other.storage;
		if (storage)
			storage->AddRef();
		slice = other.slice;
		}

	else
		*this += other.slice;

	return *this;
}


String& String::operator+=(const char* str)
{
	return *this += string_slice(str);
}


String& String::operator=(const String& other)
{
	if (storage) {
		storage->ReleaseRef();
		storage = NULL;
		}

	slice = other.slice;
	storage = other.storage;
	if (storage)
		storage->AddRef();

	return *this;
}


void String::insert(unsigned int pos, const String& other)
{
	// build the new string
	string_storage* newStorage = new string_storage(slice.substr(0, pos));
	*newStorage += other.slice;
	*newStorage += slice.substr(pos);

	// install it
	if (storage)
		storage->ReleaseRef();
	storage = newStorage;
	slice = *storage;
}


void String::erase(unsigned int pos, unsigned int n)
{
	// special-case erase from the beginning
	unsigned int len = slice.length();
	if (pos == 0) {
		if (n > len)
			n = len;
		slice = string_slice(slice.begin() + n, slice.end());
		if (storage && slice.empty()) {
			storage->ReleaseRef();
			storage = NULL;
			}
		}

	// special-case erasing the end
	else if (pos + n >= len) {
		n = len - pos;	// don't go past the end
		slice = string_slice(slice.begin(), slice.end() - n);
		}

	// deleting from the middle
	else {
		// build the new string
		string_storage* newStorage = new string_storage(slice.substr(0, pos));
		*newStorage += slice.substr(pos + n);

		// install it
		if (storage)
			storage->ReleaseRef();
		storage = newStorage;
		slice = *storage;
		}
}


void String::erase()
{
	slice = string_slice();
	if (storage) {
		storage->ReleaseRef();
		storage = NULL;
		}
}


String String::substr(unsigned int pos, unsigned int n)
{
	String newStr;
	newStr.slice = slice.substr(pos, n);
	if (storage) {
		newStr.storage = storage;
		storage->AddRef();
		}
	return newStr;
}


String String::trim()
{
	String newStr;
	newStr.slice = slice.trim();
	if (storage) {
		newStr.storage = storage;
		storage->AddRef();
		}
	return newStr;
}


String& String::detach()
{
	// make the new storage
	string_storage* newStorage = new string_storage(slice);

	// install it
	if (storage)
		storage->ReleaseRef();
	storage = newStorage;
	slice = *storage;

	return *this;
}


const char* String::c_str()
{
	if (slice.begin() == slice.end())
		return "";

	else {
		if (storage == NULL || storage->NumRefs() != 1 ||
				slice.begin() != string_slice(*storage).begin() ||
				slice.end() != string_slice(*storage).end())
			detach();
		}

	return storage->c_str();
}


const char* String::data()
{
	return slice.begin();
}


