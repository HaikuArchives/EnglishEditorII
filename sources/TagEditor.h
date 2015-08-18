/* TagEditor.h */

#ifndef _H_TagEditor_
#define _H_TagEditor_

#include <GraphicsDefs.h>

#include "DisplayDirector.h"
#include "String.h"

class WindowDirector;
class TagSelection;

class TagEditor : public DisplayDirector {
public:
	TagEditor(String tagName, TagSelection* ownerIn, WindowDirector* windowDirectorIn);
	~TagEditor();
	void           	Select();
	void           	Deselect();
	bool           	IsSelected();
	void           	SetTagName(String newTagName);
	void           	Draw(BRect updateRect);
	void           	KeyDown(string_slice key);
	void           	MouseMoved();
	void           	ScreenChanged();
	EditStylesheet*	GetStylesheet();
	Validator*     	GetValidator();
	int            	DisplayWidth();
	View*          	DrawingView();
	View*          	WindowView();
	BPoint     	ViewToDoc(BPoint viewPoint);
	BPoint     	DocToView(BPoint docPoint);
	BRect      	ViewToDoc(BRect rect);
	BRect      	DocToView(BRect rect);
	BRect      	DocRect();
	void           	StartRefreshCycle();
	void           	FinishRefreshCycle();
	void           	RefreshViewRect(BRect rect);
	void           	RefreshDocAfter(int y);
	void           	RefreshAll();
	void           	DocTypeChanged();
	BRect      	Bounds();
	String      	TagName();

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
