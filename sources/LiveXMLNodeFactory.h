/* LiveXMLNodeFactory.h */

#ifndef _H_LiveXMLNodeFactory_
#define _H_LiveXMLNodeFactory_

#include "StandardXMLNodeFactory.h"

class LiveXMLNodeFactory : public StandardXMLNodeFactory {
public:
	Document*    	MakeDocument();
	Element*     	MakeElement(DOMString name);
	Attr*        	MakeAttr(DOMString name);
	Text*        	MakeText(DOMString value);
	CDATASection*	MakeCDATASection(DOMString value);
	Text*        	MakeIgnorableWhitespace(DOMString value);
};


#endif
