/* MenuDirector.h */

#ifndef _H_MenuDirector_
#define _H_MenuDirector_

#include <GraphicsDefs.h>

#include "DisplayDirector.h"

class WindowDirector;
class Element;

class MenuDirector : public DisplayDirector {
public:
	MenuDirector(DocumentSource* docSourceIn, BPoint stemPointIn,
             WindowDirector* windowDirectorIn);
	~MenuDirector();
	void            	Draw(BRect updateRect);
	void            	KeyDown(string_slice key);
	void            	MouseMoved();
	void            	ScreenChanged();
	EditStylesheet* 	GetStylesheet();
	Validator*      	GetValidator();
	int             	DisplayWidth();
	View*           	DrawingView();
	View*           	WindowView();
	BPoint      	ViewToDoc(BPoint viewPoint);
	BPoint      	DocToView(BPoint docPoint);
	BRect       	ViewToDoc(BRect rect);
	BRect       	DocToView(BRect rect);
	BRect       	DocRect();
	void            	StartRefreshCycle();
	void            	FinishRefreshCycle();
	void            	RefreshViewRect(BRect rect);
	void            	RefreshDocAfter(int y);
	void            	RefreshAll();
	void            	DoDocAction(String action);
	void            	DocTypeChanged();
	String       	GetScriptProperty(String property);
	BRect       	Bounds();
	static String	MakeKeyName(string_slice key, int modifiers);
	static String	FindKeyActionIn(String keyName, Element* element);

protected:
	BPoint        	stemPoint;
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
