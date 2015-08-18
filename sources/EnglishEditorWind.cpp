/* EnglishEditorWind.cpp */

#include "EnglishEditorWind.h"
#include "EnglishEditorView.h"
#include "EnglishEditorApp.h"
#include "DocumentSource.h"
#include "WindowDirector.h"
#include "View.h"
#include "NativeView.h"
#include "Preferences.h"
#include "qstring.h"
#include <Screen.h>

static const BRect frameRect(40, 60, 620, 660);

const float EnglishEditorWind::screenMargin = 6;


EnglishEditorWind::EnglishEditorWind(DocumentSource* docSource)
#ifdef USE_WINDOW_SCREEN
	: BWindowScreen(docSource->GetWindowTitle(), B_32_BIT_800x600, &openError)
#else
	: BWindow(frameRect, "", B_TITLED_WINDOW_LOOK, B_NORMAL_WINDOW_FEEL, 0)
#endif
{
	// move to saved position
	BRect defaultFrame = Prefs()->GetRectPref("defaultWindFrame");
	if (defaultFrame.IsValid()) {
		MoveTo(defaultFrame.LeftTop());
		ResizeTo(defaultFrame.Width(), defaultFrame.Height());
		}

	// adjust size for screen
	BRect screenRect = BScreen(this).Frame();
	BRect frame = Frame();
	if (!screenRect.Contains(frame)) {
		float newWidth = frame.Width();
		if (frame.right > screenRect.right)
			newWidth = screenRect.right - screenMargin - frame.left;
		float newHeight = frame.Height();
		if (frame.bottom > screenRect.bottom)
			newHeight = screenRect.bottom - screenMargin - frame.top;
		ResizeTo(newWidth, newHeight);
		}

	Init(docSource);

	Show();
}


EnglishEditorWind::EnglishEditorWind()
	: BWindow(frameRect, "", B_TITLED_WINDOW_LOOK, B_NORMAL_WINDOW_FEEL, 0)
{
	// for subclasses only
}


void EnglishEditorWind::Init(DocumentSource* docSource)
{
	// set the title
	qstring title(docSource->GetWindowTitle());
	SetTitle(title.c_str());

	// make the EnglishEditorView
	// do this *after* setting up the frame so it doesn't have to resize
	BRect editViewFrame = Frame();
	editViewFrame.OffsetTo(0, 0);
	NativeView* editView =
		new NativeView(editViewFrame, "EnglishEditorView", B_FOLLOW_ALL_SIDES,
		               B_WILL_DRAW | B_FRAME_EVENTS);
	AddChild(editView);
	editView->MakeFocus();

	// make the director
	director = new WindowDirector(docSource, new EnglishEditorView(editView), this);
}


EnglishEditorWind::~EnglishEditorWind()
{
	((EnglishEditorApp*) be_app)->WindowClosing(this);
}


void EnglishEditorWind::DispatchMessage(BMessage* message, BHandler* handler)
{
	bool passMessage = true;

	// take over most command-keys, passing them on to the director
	if (message->what == B_KEY_DOWN) {
		uint32 modifiers = message->FindInt32("modifiers");
		if ((modifiers & B_COMMAND_KEY) != 0) {
			int8 c;
			message->FindInt8("byte", &c);
			switch (c) {
				case 'w':
				case 'W':
				case 'q':
				case 'Q':
					// let BeOS handle these in the normal way
					break;

				default:
					// otherwise, pass it on to the view
					BView* eeView = FindView("EnglishEditorView");
					char keyBytes[4];
					int numBytes = 0;
					for (signed char* p = (signed char*) &keyBytes[0]; ; ++p, ++numBytes) {
						status_t result = message->FindInt8("byte", numBytes, p);
						if (result != B_NO_ERROR) {
							*p = 0;
							break;
							}

						}
					eeView->KeyDown(keyBytes, numBytes);
					break;
				}
			}
		}

/***
	if (message->what == B_KEY_DOWN) {
		// since we have no menus, we need to handle most command-keys ourselves
		uint32 modifiers = message->FindInt32("modifiers");
		if ((modifiers & B_COMMAND_KEY) != 0) {
			int8 c;
			message->FindInt8("byte", &c);
			switch (c) {
				case 's':
				case 'S':
					editView->Save();
					break;

				case 'n':
				case 'N':
					((EnglishEditorApp*) be_app)->NewFile();
					break;

				case 'i':
				case 'I':
					editView->ToggleFontStyle(ITALIC);
					break;

				case 'b':
				case 'B':
					editView->ToggleFontStyle(BOLD);
					break;
				}
			}
		}
***/

	if (passMessage)
		BWindow::DispatchMessage(message, handler);
}


void EnglishEditorWind::ScreenChanged(BRect frame, color_space mode)
{
	director->ScreenChanged();
}


bool EnglishEditorWind::QuitRequested()
{
	return director->CloseRequested();
}


void EnglishEditorWind::FrameMoved(BPoint screenPoint)
{
	FrameChanged();
}


void EnglishEditorWind::FrameResized(float width, float height)
{
//***	director->ScreenChanged();
	FrameChanged();
}


void EnglishEditorWind::RequestClose()
{
	PostMessage(B_QUIT_REQUESTED);
}


void EnglishEditorWind::SetFrame(Rectangle newFrame)
{
	MoveTo(newFrame.left, newFrame.top);
	ResizeTo(newFrame.Width(), newFrame.Height());
}


bool EnglishEditorWind::IsOnlyDocumentWind()
{
	int numWindows = be_app->CountWindows();
	for (int i=0; i<numWindows; i++) {
		BWindow* wind = be_app->WindowAt(i);
		EnglishEditorWind* eeWind = dynamic_cast<EnglishEditorWind*>(wind);
		if (eeWind && eeWind != this)
			return false;
		}
	return true;
}


void EnglishEditorWind::FrameChanged()
{
	director->FrameChanged(Frame());
/***
	// only save the change if this is the only EnglishEditorWind up
	if (!IsOnlyDocumentWind())
		return;

	// save as pref
	Prefs()->SetRectPref("defaultWindFrame", Frame());
***/
}


