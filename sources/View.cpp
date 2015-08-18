/* View.cpp */

#include "View.h"
#include "NativeView.h"
#include "Font.h"
#include "Bitmap.h"
#include "Message.h"
#include "Shape.h"
#include <Window.h>
#include <Messenger.h>
#include <Screen.h>



View::View(NativeView* viewIn)
	: view(viewIn), curClicks(0), curModifiers(0)
{
	view->SetView(this);
}


void View::MovePenTo(int x, int y)
{
	view->MovePenTo(x, y);
}


void View::MovePenTo(BPoint point)
{
	view->MovePenTo(point);
}


void View::SetHighColor(rgb_color color)
{
	view->SetHighColor(color);
}


void View::SetLowColor(rgb_color color)
{
	view->SetLowColor(color);
}


void View::SetFont(Font* font)
{
	view->SetFont(font->NativeFont());
}


void View::SetDrawingMode(int drawingMode)
{
	view->SetDrawingMode((drawing_mode) drawingMode);
}


void View::SetPenSize(int penSize)
{
	view->SetPenSize(penSize);
}


void View::ConstrainClippingRegion(BRegion* region)
{
	view->ConstrainClippingRegion(region);
}


void View::PushState()
{
	view->PushState();
}


void View::PopState()
{
	view->PopState();
}


void View::DrawString(string_slice str)
{
	view->DrawString(str.begin(), str.length());
}


void View::DrawString(string_slice str, int x, int y)
{
	view->DrawString(str.begin(), str.length(), BPoint(x, y));
}


void View::ClearRect(BRect rect)
{
	view->FillRect(rect, B_SOLID_LOW);
}


void View::StrokeRect(BRect rect)
{
	view->StrokeRect(rect);
}


void View::FillRect(BRect rect)
{
	view->FillRect(rect);
}


void View::StrokeLine(BPoint startPt, BPoint endPt)
{
	view->StrokeLine(startPt, endPt);
}


void View::StrokeBezier(BPoint points[4])
{
	view->StrokeBezier(points);
}


void View::StrokeShape(Shape* shape)
{
	view->StrokeShape(shape->NativeShape());
}


void View::FillShape(Shape* shape)
{
	view->FillShape(shape->NativeShape());
}


void View::StrokeEllipse(BRect rect)
{
	view->StrokeEllipse(rect);
}


void View::FillEllipse(BRect rect)
{
	view->FillEllipse(rect);
}


void View::DrawBitmap(Bitmap* bitmap, BRect rect)
{
	view->DrawBitmap(bitmap->NativeBitmap(), rect, rect);
}


void View::Sync()
{
	view->Sync();
}


void View::Lock()
{
	view->Window()->Lock();
}


void View::Unlock()
{
	view->Window()->Unlock();
}


void View::SendMessage(Message* message)
{
	BMessenger(view).SendMessage(message->NativeMessage());
}


void View::MouseTrackingPause()
{
	snooze(20 * 1000);	// BeBook recommended value: 20 ms.
}


BPoint View::GetMousePoint()
{
	BPoint cursor;
	uint32 buttons;
	view->GetMouse(&cursor, &buttons, false);
	return cursor;
}


int View::GetMouseButtons()
{
	BPoint cursor;
	uint32 buttons;
	view->GetMouse(&cursor, &buttons, true);
	return buttons;
}


void View::Draw(BRect updateRect)
{
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


void View::FrameResized(int newWidth, int newHeight)
{
}


void View::MessageReceived(Message* message)
{
	view->BView::MessageReceived(message->NativeMessage());
}


int View::Width()
{
	return (int) view->Bounds().Width();
}


int View::Height()
{
	return (int) view->Bounds().Height();
}


BRect View::Bounds()
{
	return view->Bounds();
}


long View::GetColorSpace()
{
	return BScreen(view->Window()).ColorSpace();
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


