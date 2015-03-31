/* Bitmap.cpp */

#include "Bitmap.h"
#include "View.h"
#include "NativeView.h"



Bitmap::Bitmap(int width, int height, long colorSpace)
{
	bitmap = new BBitmap(BRect(0, 0, width, height), (color_space) colorSpace, true);
	NativeView* nativeView =
		new NativeView(BRect(0, 0, width, height), "bitmap view",
	                   B_FOLLOW_ALL_SIDES, B_WILL_DRAW | B_FRAME_EVENTS);
	bitmap->AddChild(nativeView);
	view = new View(nativeView);
}


Bitmap::~Bitmap()
{
	delete bitmap;
}


void Bitmap::Lock()
{
	bitmap->Lock();
}


void Bitmap::Unlock()
{
	bitmap->Unlock();
}


View* Bitmap::GetView()
{
	return view;
}


long Bitmap::GetColorSpace()
{
	return bitmap->ColorSpace();
}


BBitmap* Bitmap::NativeBitmap()
{
	return bitmap;
}


