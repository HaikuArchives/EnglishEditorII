/* BasicLiveText.h */

#ifndef _H_BasicLiveText_
#define _H_BasicLiveText_

#include "BasicText.h"
#include "LiveText.h"

class BasicLiveText : public LiveText, public BasicText {
public:
	BasicLiveText(DOMString textIn, bool isIgnorableWhitespace = false);
	BasicLiveText(const BasicLiveText& other);
	void 	AppendData(DOMString arg);
	void 	InsertData(unsigned long offset, DOMString arg);
	void 	DeleteData(unsigned long offset, unsigned long count);
	void 	SetData(DOMString arg);
	Text*	SplitText(unsigned long offset);
	bool 	IsIgnorableWhitespace();
	Node*	CloneNode(bool deep);
};


#endif
