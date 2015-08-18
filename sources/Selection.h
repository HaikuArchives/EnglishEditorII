/* Selection.h */

#ifndef _H_Selection_
#define _H_Selection_

#include <GraphicsDefs.h>

#include "CoordPoint.h"
#include "Rectangle.h"
#include "DOMString.h"
#include "string_slice.h"

class DisplayDirector;
class DisplayNode;
class View;
class Action;

class Selection {
public:
	virtual~Selection();
	virtual void      	Draw(View* view, CoordPoint origin) = 0;
	virtual void      	AcceptKey(string_slice key, DisplayDirector* director) = 0;
	virtual void      	Promote(DisplayDirector* director);
	virtual Selection*	ExtendTo(CoordPoint point) = 0;
	virtual Action*   	GetRestoreAction() = 0;
	virtual bool      	HandleSpecialKey(string_slice key, DisplayDirector* director);
	virtual bool      	CanCopy();
	virtual DOMString 	GetXMLCopy();
	virtual bool      	CanPaste();
	virtual void      	Paste(String pasteText, DisplayDirector * director);
	virtual Rectangle 	Bounds() = 0;
	virtual bool      	IsVisible(Rectangle pageRect) = 0;
	virtual bool      	NeedsClip() = 0;
	virtual bool      	ContainsPoint(CoordPoint point);
	virtual CoordPoint	DragStartPoint(CoordPoint destPoint);
	CoordPoint        	BlockOrigin(DisplayNode* node);

	static const rgb_color	selectionColor;
};


#endif
