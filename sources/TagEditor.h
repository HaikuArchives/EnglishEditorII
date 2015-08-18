/* TagEditor.h */

#ifndef _H_TagEditor_
#define _H_TagEditor_

#include "DisplayDirector.h"
#include "DOMString.h"

class WindowDirector;
class TagSelection;

class TagEditor : public DisplayDirector {
public:
	TagEditor(DOMString tagName, TagSelection* ownerIn, WindowDirector* windowDirectorIn);
	~TagEditor();
	void           	Select();
	void           	Deselect();
	bool           	IsSelected();
	void           	SetTagName(DOMString newTagName);
	void           	Draw(Rectangle updateRect);
	void           	KeyDown(string_slice key);
	void           	MouseMoved();
	void           	ScreenChanged();
	EditStylesheet*	GetStylesheet();
	Validator*     	GetValidator();
	int            	DisplayWidth();
	View*          	DrawingView();
	View*          	WindowView();
	CoordPoint     	ViewToDoc(CoordPoint viewPoint);
	CoordPoint     	DocToView(CoordPoint docPoint);
	Rectangle      	ViewToDoc(Rectangle rect);
	Rectangle      	DocToView(Rectangle rect);
	Rectangle      	DocRect();
	void           	StartRefreshCycle();
	void           	FinishRefreshCycle();
	void           	RefreshViewRect(Rectangle rect);
	void           	RefreshDocAfter(int y);
	void           	RefreshAll();
	void           	DocTypeChanged();
	Rectangle      	Bounds();
	DOMString      	TagName();

protected:
	WindowDirector* 	windowDirector;
	TagSelection*   	owner;
	EditStylesheet* 	stylesheet;
	Validator*      	validator;
	static const int	lineWidth;
	static const int	xOutset;
	static const int	yOutset;
	static const int	cornerRadius;
	static const int	emptyWidth;
	static rgb_color    	bgndColor;
	static int      	bgndAlpha;
	static rgb_color    	selectedColor;
};


#endif
