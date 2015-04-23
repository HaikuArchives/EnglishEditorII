/* DOMException.h */

#ifndef _H_DOMException_
#define _H_DOMException_


enum {
	INDEX_SIZE_ERR = 1,
	DOMSTRING_SIZE_ERR = 2,
	HIERARCHY_REQUEST_ERR = 3,
	WRONG_DOCUMENT_ERR = 4,
	INVALID_CHARACTER_ERR = 5,
	NO_DATA_ALLOWED_ERR = 6,
	NO_MODIFICATION_ALLOWED_ERR = 7,
	NOT_FOUND_ERR = 8,
	NOT_SUPPORTED_ERR = 9,
	INUSE_ATTRIBUTE_ERR = 10
};

class DOMException {
public:
	DOMException(unsigned short codeIn);
	unsigned short	Code();
	const char*   	Name();

protected:
	unsigned short	code;
};


#endif
