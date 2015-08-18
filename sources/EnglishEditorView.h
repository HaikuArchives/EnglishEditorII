/* EnglishEditorView.h */

#ifndef _H_EnglishEditorView_
#define _H_EnglishEditorView_

#include "View.h"

class WindowDirector;

class EnglishEditorView : public View {
public:
	EnglishEditorView(NativeView* nativeView);
	void	Draw(BRect updateRect);
	void	MouseDown(int x, int y);
	void	KeyDown(string_slice key);
	void	MouseMoved(int transitType);
	void	FrameResized(int newWidth, int newHeight);
	void	MessageReceived(BMessage* message);
	void	SetDirector(WindowDirector* newDirector);

protected:
	WindowDirector*	director;
};


#endif
