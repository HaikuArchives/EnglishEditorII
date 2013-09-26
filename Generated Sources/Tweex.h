/* Tweex.h */

#ifndef _H_Tweex_
#define _H_Tweex_

#include "string_slice.h"
#include "Color.h"

class DocumentSource;
class Document;

class Tweex {
public:
	static string_slice	GetString(string_slice tweakName);
	static int         	GetInt(string_slice tweakName);
	static Color       	GetColor(string_slice tweakName);

protected:
	static DocumentSource*	docSource;
	static Document*      	tweexDoc;
};


#endif
