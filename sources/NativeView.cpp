/* NativeView.cpp */

#include "NativeView.h"
#include "View.h"
#include <Message.h>
#include <Window.h>
#include <MessageQueue.h>



NativeView::NativeView(BRect frame, const char* name, uint32 resizingMode, uint32 flags)
	: BView(frame, name, resizingMode, flags),
	  view(NULL)
{
}


NativeView::~NativeView()
{
}


void NativeView::Draw(BRect updateRect)
{
	view->Draw(updateRect);
}


void NativeView::FrameResized(float width, float height)
{
	view->FrameResized(width, height);
}


void NativeView::KeyDown(const char* bytes, int32 numBytes)
{
	view->SetCurModifiers(Window()->CurrentMessage()->FindInt32("modifiers"));
	view->KeyDown(string_slice(bytes, bytes + numBytes));
}


void NativeView::MouseDown(BPoint point)
{
	BMessage* curMessage = Window()->CurrentMessage();
	view->SetCurClicks(curMessage->FindInt32("clicks"));
	view->SetCurModifiers(curMessage->FindInt32("modifiers"));
	view->MouseDown(point.x, point.y);
}


void NativeView::MouseMoved(BPoint point, uint32 transit, const BMessage* message)
{
	CleanMessageQueue();
	view->MouseMoved(transit);
}


void NativeView::MessageReceived(BMessage* nativeMessage)
{
	BMessage message(nativeMessage);
	view->MessageReceived(&message);
}


void NativeView::SetView(View* newView)
{
	view = newView;
}


View* NativeView::GetView()
{
	return view;
}


void NativeView::CleanMessageQueue()
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


