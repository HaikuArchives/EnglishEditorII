/* View.h */

#ifndef _H_View_
#define _H_View_

#include "CoordPoint.h"
#include "Rectangle.h"
#include "Region.h"
#include "string_slice.h"
#include <View.h>

class NativeView;
class Font;
class Bitmap;
class Message;
class Shape;

// drawing modes
enum {
	CopyDrawingMode = B_OP_COPY,
	AlphaDrawingMode = B_OP_ALPHA
};

// transit types
enum {
	MouseEntering = B_ENTERED_VIEW,
	MouseInside = B_INSIDE_VIEW,
	MouseExiting = B_EXITED_VIEW
};

// modifiers
enum {
	ShiftModifier = B_SHIFT_KEY,
	OptionModifier = B_OPTION_KEY,
	CommandModifier = B_COMMAND_KEY,
	ControlModifier = B_CONTROL_KEY
};

// mouse buttons
enum {
	PrimaryMouseButton = B_PRIMARY_MOUSE_BUTTON,
	SecondaryMouseButton = B_SECONDARY_MOUSE_BUTTON,
	TertiaryMouseButton = B_TERTIARY_MOUSE_BUTTON
};

class View {
public:
	View(NativeView* viewIn);
	void        	MovePenTo(int x, int y);
	void        	MovePenTo(CoordPoint point);
	void        	SetHighColor(rgb_color color);
	void        	SetLowColor(rgb_color color);
	void        	SetFont(Font* font);
	void        	SetDrawingMode(int drawingMode);
	void        	SetPenSize(int penSize);
	void        	ConstrainClippingRegion(Region* region);
	void        	PushState();
	void        	PopState();
	void        	DrawString(string_slice str);
	void        	DrawString(string_slice str, int x, int y);
	void        	ClearRect(Rectangle rect);
	void        	StrokeRect(Rectangle rect);
	void        	FillRect(Rectangle rect);
	void        	StrokeLine(CoordPoint startPt, CoordPoint endPt);
	void        	StrokeBezier(CoordPoint points[4]);
	void        	StrokeShape(Shape* shape);
	void        	FillShape(Shape* shape);
	void        	StrokeEllipse(Rectangle rect);
	void        	FillEllipse(Rectangle rect);
	void        	DrawBitmap(Bitmap* bitmap, Rectangle rect);
	void        	Sync();
	void        	Lock();
	void        	Unlock();
	void        	SendMessage(Message* message);
	void        	MouseTrackingPause();
	CoordPoint  	GetMousePoint();
	int         	GetMouseButtons();
	virtual void	Draw(Rectangle updateRect);
	virtual void	MouseDown(int x, int y);
	virtual void	KeyDown(string_slice key);
	virtual void	MouseMoved(int transitType);
	virtual void	FrameResized(int newWidth, int newHeight);
	virtual void	MessageReceived(Message* message);
	int         	Width();
	int         	Height();
	Rectangle   	Bounds();
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
