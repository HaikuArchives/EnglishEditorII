/* MenuDirector.h */

#ifndef _H_MenuDirector_
#define _H_MenuDirector_

#include "DisplayDirector.h"

class WindowDirector;
class Element;

class MenuDirector : public DisplayDirector {
public:
	MenuDirector(DocumentSource* docSourceIn, CoordPoint stemPointIn,
             WindowDirector* windowDirectorIn);
	~MenuDirector();
	void            	Draw(Rectangle updateRect);
	void            	KeyDown(string_slice key);
	void            	MouseMoved();
	void            	ScreenChanged();
	EditStylesheet* 	GetStylesheet();
	Validator*      	GetValidator();
	int             	DisplayWidth();
	View*           	DrawingView();
	View*           	WindowView();
	CoordPoint      	ViewToDoc(CoordPoint viewPoint);
	CoordPoint      	DocToView(CoordPoint docPoint);
	Rectangle       	ViewToDoc(Rectangle rect);
	Rectangle       	DocToView(Rectangle rect);
	Rectangle       	DocRect();
	void            	StartRefreshCycle();
	void            	FinishRefreshCycle();
	void            	RefreshViewRect(Rectangle rect);
	void            	RefreshDocAfter(int y);
	void            	RefreshAll();
	void            	DoDocAction(DOMString action);
	void            	DocTypeChanged();
	DOMString       	GetScriptProperty(DOMString property);
	Rectangle       	Bounds();
	static DOMString	MakeKeyName(string_slice key, int modifiers);
	static DOMString	FindKeyActionIn(DOMString keyName, Element* element);

protected:
	CoordPoint        	stemPoint;
	WindowDirector*   	windowDirector;
	EditStylesheet*   	stylesheet;
	Validator*        	validator;
	int               	docWidth;
	static const int  	stemHeight;
	static const int  	stemWidth;
	static const int  	cornerRadius;
	static const int  	borderWidth;

public:
	static const rgb_color	borderColor;

protected:
	static rgb_color      	bgndColor;
	static const int  	hMargin;
	static const int  	vMargin;
};


#endif
