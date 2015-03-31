/* BasicLiveCDATASection.h */

#ifndef _H_BasicLiveCDATASection_
#define _H_BasicLiveCDATASection_

#include "LiveCDATASection.h"
#include "BasicLiveText.h"

class BasicLiveCDATASection : public LiveCDATASection, public BasicLiveText {
public:
	BasicLiveCDATASection(DOMString textIn);
	BasicLiveCDATASection(const BasicLiveCDATASection& other);
	Text*	SplitText(unsigned long offset);
	bool 	IsIgnorableWhitespace();
	Node*	CloneNode(bool deep);
};


#endif
