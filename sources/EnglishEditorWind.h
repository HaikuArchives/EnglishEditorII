/* EnglishEditorWind.h */

#ifndef _H_EnglishEditorWind_
#define _H_EnglishEditorWind_

#include <Rect.h>
#include <Window.h>

class EnglishEditorView;
class DocumentSource;
class WindowDirector;

class EnglishEditorWind : public BWindow {
public:
	EnglishEditorWind(DocumentSource* docSource);

protected:
	EnglishEditorWind();

public:
	void        	Init(DocumentSource* docSource);
	~EnglishEditorWind();
	void        	DispatchMessage(BMessage* message, BHandler* handler);
	void        	ScreenChanged(BRect frame, color_space mode);
	bool        	QuitRequested();
	void        	FrameMoved(BPoint screenPoint);
	void        	FrameResized(float width, float height);
	void        	RequestClose();
	void        	SetFrame(BRect newFrame);
	bool        	IsOnlyDocumentWind();
	virtual void	FrameChanged();

protected:
	WindowDirector*   	director;
	static const float	screenMargin;
};


#endif
