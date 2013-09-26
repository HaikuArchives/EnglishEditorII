/* NativeView.h */

#ifndef _H_NativeView_
#define _H_NativeView_

#include <View.h>

class View;
class Message;

class NativeView : public BView {
public:
	NativeView(BRect frame, const char* name, uint32 resizingMode, uint32 flags);
	~NativeView();
	void 	Draw(BRect updateRect);
	void 	FrameResized(float width, float height);
	void 	KeyDown(const char* bytes, int32 numBytes);
	void 	MouseDown(BPoint point);
	void 	MouseMoved(BPoint point, uint32 transit, const BMessage* message);
	void 	MessageReceived(BMessage* nativeMessage);
	void 	SetView(View* newView);
	View*	GetView();
	void 	CleanMessageQueue();

protected:
	View*	view;
};


#endif
