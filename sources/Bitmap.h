/* Bitmap.h */

#ifndef _H_Bitmap_
#define _H_Bitmap_

#include <Bitmap.h>

class View;

class Bitmap {
public:
	Bitmap(int width, int height, long colorSpace);
	~Bitmap();
	void    	Lock();
	void    	Unlock();
	View*   	GetView();
	long    	GetColorSpace();
	BBitmap*	NativeBitmap();

protected:
	BBitmap*	bitmap;
	View*   	view;
};


#endif
