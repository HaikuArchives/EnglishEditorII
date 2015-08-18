/* EnglishEditorView.cpp */

#include "EnglishEditorView.h"
#include "WindowDirector.h"
#include "Message.h"
#include "Messages.h"
#include "InternalException.h"



EnglishEditorView::EnglishEditorView(NativeView* nativeView)
	: View(nativeView), director(NULL)
{
}


void EnglishEditorView::Draw(BRect updateRect)
{
	if (director == NULL)
		throw InternalException("Attempt to draw EEView without associated DisplayDirector!");
	director->Draw(updateRect);
}


void EnglishEditorView::MouseDown(int x, int y)
{
	director->MouseDown(x, y);
}


void EnglishEditorView::KeyDown(string_slice key)
{
	director->KeyDown(key);
}


void EnglishEditorView::MouseMoved(int transitType)
{
	director->MouseMoved();
}


void EnglishEditorView::FrameResized(int newWidth, int newHeight)
{
	/***/
}


void EnglishEditorView::MessageReceived(Message* message)
{
	switch (message->What()) {
		case ScrollStepMessage:
			director->ScrollMessageReceived();
			break;

		default:
			View::MessageReceived(message);
			break;
		}
}


void EnglishEditorView::SetDirector(WindowDirector* newDirector)
{
	director = newDirector;
}


