/* SmartenedWord.h */

#ifndef _H_SmartenedWord_
#define _H_SmartenedWord_

#include "Word.h"

class SmartenedWord : public Word {
public:
	SmartenedWord(String wordIn);
	int         	Width(Font* font);
	void        	Draw(GlyphDrawContext* context);
	string_slice	GetDisplayWord();
	virtual int 	DocToDisplayOffset(int docOffset);
	virtual int 	DisplayToDocOffset(int displayOffset);

protected:
	String	displayWord;
};


#endif
