/* View.cpp */

#include "View.h"
#include "Font.h"
#include "Bitmap.h"
#include <Message.h>
#include "Shape.h"
#include <Window.h>
#include <Messenger.h>
#include <MessageQueue.h>
#include <Screen.h>



View::View(BRect frame, const char* name, uint32 resizingMode, uint32 flags)
	:
	BView(frame, name, resizingMode, flags),
	curClicks(0),
	curModifiers(0)
{
}


void View::SetFont(Font* font)
{
	BView::SetFont(font->NativeFont());
}


void View::SetDrawingMode(int drawingMode)
{
	BView::SetDrawingMode((drawing_mode) drawingMode);
}


void View::DrawString(string_slice str)
{
	BView::DrawString(str.begin(), str.length());
}


void View::DrawString(string_slice str, int x, int y)
{
	BView::DrawString(str.begin(), str.length(), BPoint(x, y));
}


void View::ClearRect(BRect rect)
{
	BView::FillRect(rect, B_SOLID_LOW);
}


void View::StrokeShape(Shape* shape)
{
	BView::StrokeShape(shape->NativeShape());
}


void View::FillShape(Shape* shape)
{
	BView::FillShape(shape->NativeShape());
}


void View::DrawBitmap(Bitmap* bitmap, BRect rect)
{
	BView::DrawBitmap(bitmap->NativeBitmap(), rect, rect);
}


void View::Lock()
{
	Window()->Lock();
}


void View::Unlock()
{
	Window()->Unlock();
}


void View::SendMessage(BMessage* message)
{
	BMessenger(this).SendMessage(message);
}


void View::MouseTrackingPause()
{
	snooze(20 * 1000);	// BeBook recommended value: 20 ms.
}


BPoint View::GetMousePoint()
{
	BPoint cursor;
	uint32 buttons;
	GetMouse(&cursor, &buttons, false);
	return cursor;
}


int View::GetMouseButtons()
{
	BPoint cursor;
	uint32 buttons;
	GetMouse(&cursor, &buttons, true);
	return buttons;
}


void View::MouseDown(int x, int y)
{
}


void View::KeyDown(string_slice key)
{
}


void View::MouseMoved(int transitType)
{
}


long View::GetColorSpace()
{
	return BScreen(Window()).ColorSpace();
}


int View::CurClicks()
{
	return curClicks;
}


int View::CurModifiers()
{
	return curModifiers;
}


void View::SetCurClicks(int newCurClicks)
{
	curClicks = newCurClicks;
}


void View::SetCurModifiers(int newCurModifiers)
{
	curModifiers = newCurModifiers;
}

// #pragma mark - Wrappers

void View::KeyDown(const char* bytes, int32 numBytes)
{
	BView::KeyDown(bytes, numBytes);
	SetCurModifiers(Window()->CurrentMessage()->FindInt32("modifiers"));
	KeyDown(string_slice(bytes, bytes + numBytes));
}

void View::MouseDown(BPoint point)
{
	BView::MouseDown(point);
	BMessage* curMessage = Window()->CurrentMessage();
	SetCurClicks(curMessage->FindInt32("clicks"));
	SetCurModifiers(curMessage->FindInt32("modifiers"));
	MouseDown(point.x, point.y);
}

void View::MouseMoved(BPoint point, uint32 transit, const BMessage* message)
{
	BView::MouseMoved(point, transit, message);
	CleanMessageQueue();
	MouseMoved(transit);
}

void View::CleanMessageQueue()
{
	// remove all mouse-moved messages to avoid queue poisoning
	BMessageQueue* messageQueue = Window()->MessageQueue();
	while (true) {
		BMessage* message = messageQueue->FindMessage(B_MOUSE_MOVED, 0);
		if (message == NULL)
			break;
		messageQueue->RemoveMessage(message);
	}
}
