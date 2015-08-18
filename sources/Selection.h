/* Selection.h */

#ifndef _H_Selection_
#define _H_Selection_

#include <GraphicsDefs.h>
#include <Rect.h>

#include <Point.h>
#include "String.h"
#include "string_slice.h"

class DisplayDirector;
class DisplayNode;
class View;
class Action;

class Selection {
public:
	virtual~Selection();
	virtual void      	Draw(View* view, BPoint origin) = 0;
	virtual void      	AcceptKey(string_slice key, DisplayDirector* director) = 0;
	virtual void      	Promote(DisplayDirector* director);
	virtual Selection*	ExtendTo(BPoint point) = 0;
	virtual Action*   	GetRestoreAction() = 0;
	virtual bool      	HandleSpecialKey(string_slice key, DisplayDirector* director);
	virtual bool      	CanCopy();
	virtual String 	GetXMLCopy();
	virtual bool      	CanPaste();
	virtual void      	Paste(String pasteText, DisplayDirector * director);
	virtual BRect 	Bounds() = 0;
	virtual bool      	IsVisible(BRect pageRect) = 0;
	virtual bool      	NeedsClip() = 0;
	virtual bool      	ContainsPoint(BPoint point);
	virtual BPoint	DragStartPoint(BPoint destPoint);
	BPoint        	BlockOrigin(DisplayNode* node);

	static const rgb_color	selectionColor;
};


#endif
