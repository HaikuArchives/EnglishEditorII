/* EmptyPlaceholderSelection.h */

#ifndef _H_EmptyPlaceholderSelection_
#define _H_EmptyPlaceholderSelection_

#include "BetweenGlyphsSelection.h"

class PlaceholderTextDisplayNode;
class Element;

class EmptyPlaceholderSelection : public BetweenGlyphsSelection {
public:
	EmptyPlaceholderSelection(PlaceholderTextDisplayNode* textNodeIn);
	void     	Draw(View* view, CoordPoint origin);
	void     	AcceptKey(string_slice key, DisplayDirector* director);
	Action*  	GetRestoreAction();
	void     	Paste(String pasteText, DisplayDirector* director);
	void     	InsertTag(DOMString tagName, DisplayDirector* director);
	DOMString	TagName();
	Element* 	GetElement();
};


#endif
