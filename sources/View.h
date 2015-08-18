/* View.h */

#ifndef _H_View_
#define _H_View_

#include <Rect.h>

#include <Point.h>
#include <Region.h>
#include "string_slice.h"
#include <View.h>

class NativeView;
class Font;
class Bitmap;
class Message;
class Shape;

class View {
public:
	View(NativeView* viewIn);
	void        	MovePenTo(int x, int y);
	void        	MovePenTo(BPoint point);
	void        	SetHighColor(rgb_color color);
	void        	SetLowColor(rgb_color color);
	void        	SetFont(Font* font);
	void        	SetDrawingMode(int drawingMode);
	void        	SetPenSize(int penSize);
	void        	ConstrainClippingRegion(BRegion* region);
	void        	PushState();
	void        	PopState();
	void        	DrawString(string_slice str);
	void        	DrawString(string_slice str, int x, int y);
	void        	ClearRect(BRect rect);
	void        	StrokeRect(BRect rect);
	void        	FillRect(BRect rect);
	void        	StrokeLine(BPoint startPt, BPoint endPt);
	void        	StrokeBezier(BPoint points[4]);
	void        	StrokeShape(Shape* shape);
	void        	FillShape(Shape* shape);
	void        	StrokeEllipse(BRect rect);
	void        	FillEllipse(BRect rect);
	void        	DrawBitmap(Bitmap* bitmap, BRect rect);
	void        	Sync();
	void        	Lock();
	void        	Unlock();
	void        	SendMessage(Message* message);
	void        	MouseTrackingPause();
	BPoint  	GetMousePoint();
	int         	GetMouseButtons();
	virtual void	Draw(BRect updateRect);
	virtual void	MouseDown(int x, int y);
	virtual void	KeyDown(string_slice key);
	virtual void	MouseMoved(int transitType);
	virtual void	FrameResized(int newWidth, int newHeight);
	virtual void	MessageReceived(Message* message);
	int         	Width();
	int         	Height();
	BRect   	Bounds();
	long        	GetColorSpace();
	int         	CurClicks();
	int         	CurModifiers();
	void        	SetCurClicks(int newCurClicks);
	void        	SetCurModifiers(int newCurModifiers);

protected:
	NativeView*	view;
	int        	curClicks;
	int        	curModifiers;
};


#endif
