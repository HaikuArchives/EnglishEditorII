/* TagDocumentSource.h */

#ifndef _H_TagDocumentSource_
#define _H_TagDocumentSource_

#include "DocumentSource.h"
#include "DOMString.h"

class Text;


class TagDocumentSource : public DocumentSource {
public:
	TagDocumentSource(DOMString tagName);
	~TagDocumentSource();
	Document*   	GetDocument();
	string_slice	GetWindowTitle();
	void        	Save();
	DOMString   	TagName();
	void        	SetTagName(DOMString tagName);
	static void 	InitStatics();

protected:
	Document*	document;
	Text*    	tagText;
};


#endif
