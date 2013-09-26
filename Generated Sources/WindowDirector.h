/* WindowDirector.h */

#ifndef _H_WindowDirector_
#define _H_WindowDirector_

#include "DisplayDirector.h"
#include "Region.h"
#include "Timing.h"

class EnglishEditorView;
class EnglishEditorWind;
class Bitmap;
class ScrollerControl;
class StatusControl;
class MenuControl;
class HMarginControl;
class VMarginControl;
class FrameRateControl;
class MenuDirector;

class WindowDirector : public DisplayDirector {
public:
	WindowDirector(DocumentSource* docSourceIn, EnglishEditorView* viewIn,
               EnglishEditorWind* windIn);
	~WindowDirector();
	void           	Draw(Rectangle updateRect);
	void           	KeyDown(string_slice key);
	void           	MouseDown(int x, int y);
	void           	MouseMoved();
	void           	FrameChanged(Rectangle newFrame);
	void           	ScreenChanged();
	bool           	CloseRequested();
	void           	DoDocAction(DOMString action);
	void           	DocTypeChanged();
	void           	RequestClose();
	void           	KillControls();
	void           	SetScrollTarget(float newScrollTarget);
	bool           	Autoscroll(CoordPoint point);
	void           	ScrollToSelection();
	bool           	DoScrollStep();
	void           	ContinueScrolling();
	void           	ScrollDocumentTo(int newScrollPos);
	void           	ScrollMessageReceived();
	int            	LastScrollPos();
	void           	Save();
	void           	ShowMenu();
	void           	HideMenu();
	bool           	MenuIsVisible();
	int            	GetHMargin();
	void           	SetHMargin(int newHMargin);
	int            	GetVMargin();
	void           	SetVMargin(int newVMargin);
	EditStylesheet*	GetStylesheet();
	Validator*     	GetValidator();
	int            	DisplayWidth();
	Rectangle      	ViewBounds();
	View*          	DrawingView();
	View*          	WindowView();
	Microseconds   	LastScrollFrameTime();
	Microseconds   	LastDrawTime();
	Microseconds   	LastBlitTime();
	int            	GetScrollPos();
	float          	GetVisibleProportion();
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
	DOMString      	FunctionCall(DOMString function, DOMString arg, StyleScriptable* target);

protected:
	EnglishEditorView*	view;
	EnglishEditorWind*	wind;
	Bitmap*           	bitmap;
	int               	hMargin;
	int               	vMargin;
	EditStylesheet*   	stylesheet;
	Validator*        	validator;
	MenuDirector*     	menu;
	int               	scrollPos;
	int               	scrollTarget;
	float             	curAnimatedScrollPos;
	bool              	scrollStepPending;
	bool              	scrollMessagePending;
	int               	refreshCycleNesting;
	Region*           	refreshRegion;
	Microseconds      	lastScrollFrameTime;
	Microseconds      	lastScrollStepTime;
	Microseconds      	lastDrawTime;
	Microseconds      	lastBlitTime;
	Action*           	lastAction;
	Action*           	sentinalAction;
	Action*           	lastSaveAction;
	ScrollerControl*  	scrollerControl;
	StatusControl*    	statusControl;
	MenuControl*      	menuControl;
	HMarginControl*   	hMarginControl;
	VMarginControl*   	vMarginControl;
	FrameRateControl* 	frameRateControl;
	static const int  	DefaultHMargin;
	static const int  	DefaultVMargin;
	static const int  	PageScrollOverlap;
	static const int  	ArrowScrollAmount;
};


#endif
