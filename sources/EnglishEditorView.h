/* EnglishEditorView.h */

#ifndef _H_EnglishEditorView_
#define _H_EnglishEditorView_

#include "View.h"

class WindowDirector;

class EnglishEditorView : public View {
public:
	EnglishEditorView(BRect frame, const char* name, uint32 resizingMode, uint32 flags);
	void	Draw(BRect updateRect);
	void	MouseDown(int x, int y);
	void	KeyDown(string_slice key);
	void	MouseMoved(int transitType);
	void	MessageReceived(BMessage* message);
	void	SetDirector(WindowDirector* newDirector);

protected:
	WindowDirector*	director;
};


#endif
