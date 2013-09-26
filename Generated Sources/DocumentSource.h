/* DocumentSource.h */

#ifndef _H_DocumentSource_
#define _H_DocumentSource_

#include "StyleScriptable.h"
#include "string_slice.h"

class Document;
class DisplayDirector;

class DocumentSource : public StyleScriptable {
public:
	virtual Document*      	GetDocument() = 0;
	virtual string_slice   	GetWindowTitle() = 0;
	virtual DocumentSource*	MakeMenuDocSource();
	virtual void           	AttachedToDirector(DisplayDirector* director);
	virtual void           	Save() = 0;
	virtual bool           	CanCloseOnSpacebar();
};


#endif
