/* TagDocumentSource.h */

#ifndef _H_TagDocumentSource_
#define _H_TagDocumentSource_

#include "DocumentSource.h"
#include "String.h"

class Text;


class TagDocumentSource : public DocumentSource {
public:
	TagDocumentSource(String tagName);
	~TagDocumentSource();
	Document*   	GetDocument();
	string_slice	GetWindowTitle();
	void        	Save();
	String   	TagName();
	void        	SetTagName(String tagName);
	static void 	InitStatics();

protected:
	Document*	document;
	Text*    	tagText;
};


#endif
