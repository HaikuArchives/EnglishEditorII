/* TagSelection.h */

#ifndef _H_TagSelection_
#define _H_TagSelection_

#include <Rect.h>

#include "Selection.h"
#include "String.h"

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
	virtual String	TagName() = 0;
	virtual BRect	TagSpecRect() = 0;
	virtual void     	TagNameChanged(String newTagName, DisplayDirector* director) = 0;
	virtual void     	TagEditDone(DisplayDirector* director);
	void             	AcceptKey(string_slice key, DisplayDirector* director);
	bool             	HandleSpecialKey(string_slice key, DisplayDirector* director);

protected:
	TagEditor*	tagEditor;
};


#endif
