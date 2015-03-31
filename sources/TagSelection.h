/* TagSelection.h */

#ifndef _H_TagSelection_
#define _H_TagSelection_

#include "Selection.h"
#include "DOMString.h"
#include "Rectangle.h"

class TagEditor;

class TagSelection : public Selection {
public:
	TagSelection();
	~TagSelection();
	void             	ShowTag(DisplayDirector* director);
	void             	HideTag(DisplayDirector* director);
	void             	SelectTag(DisplayDirector* director);
	void             	DeselectTag(DisplayDirector* director);
	bool             	TagIsSelected();
	void             	RestoreTag(DisplayDirector* director);
	virtual DOMString	TagName() = 0;
	virtual Rectangle	TagSpecRect() = 0;
	virtual void     	TagNameChanged(DOMString newTagName, DisplayDirector* director) = 0;
	virtual void     	TagEditDone(DisplayDirector* director);
	void             	AcceptKey(string_slice key, DisplayDirector* director);
	bool             	HandleSpecialKey(string_slice key, DisplayDirector* director);

protected:
	TagEditor*	tagEditor;
};


#endif
