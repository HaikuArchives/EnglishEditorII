/* EmptyPlaceholderSelection.h */

#ifndef _H_EmptyPlaceholderSelection_
#define _H_EmptyPlaceholderSelection_

#include "BetweenGlyphsSelection.h"

class PlaceholderTextDisplayNode;
class Element;

class EmptyPlaceholderSelection : public BetweenGlyphsSelection {
public:
	EmptyPlaceholderSelection(PlaceholderTextDisplayNode* textNodeIn);
	void     	Draw(View* view, BPoint origin);
	void     	AcceptKey(string_slice key, DisplayDirector* director);
	Action*  	GetRestoreAction();
	void     	Paste(String pasteText, DisplayDirector* director);
	void     	InsertTag(String tagName, DisplayDirector* director);
	String	TagName();
	Element* 	GetElement();
};


#endif
