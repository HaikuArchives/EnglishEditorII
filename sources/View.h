/* View.h */

#ifndef _H_View_
#define _H_View_

#include <Rect.h>

#include <Point.h>
#include <Region.h>
#include "string_slice.h"
#include <View.h>

class Font;
class Bitmap;
class BMessage;
class Shape;

class View : public BView {
public:
					View(BRect frame, const char* name, uint32 resizingMode,
						 uint32 flags);
	void        	SetFont(Font* font);
	void        	SetDrawingMode(int drawingMode);
	void        	DrawString(string_slice str);
	void        	DrawString(string_slice str, int x, int y);
	void        	ClearRect(BRect rect);
	void        	StrokeShape(Shape* shape);
	void        	FillShape(Shape* shape);
	void        	DrawBitmap(Bitmap* bitmap, BRect rect);
	void        	Lock();
	void        	Unlock();
	void        	SendMessage(BMessage* message);
	void        	MouseTrackingPause();
	BPoint  		GetMousePoint();
	int         	GetMouseButtons();

	virtual void	MouseDown(int x, int y);
	virtual void	MouseDown(BPoint point);
	virtual void	CleanMessageQueue();

	virtual void	KeyDown(string_slice key);
	virtual void	KeyDown(const char* bytes, int32 numBytes);

	virtual void	MouseMoved(int transitType);
	virtual void	MouseMoved(BPoint point, uint32 transit, const BMessage* message);

	long        	GetColorSpace();
	int         	CurClicks();
	int         	CurModifiers();
	void        	SetCurClicks(int newCurClicks);
	void        	SetCurModifiers(int newCurModifiers);

protected:
	int        	curClicks;
	int        	curModifiers;
};


#endif
