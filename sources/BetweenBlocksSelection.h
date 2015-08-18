/* BetweenBlocksSelection.h */

#ifndef _H_BetweenBlocksSelection_
#define _H_BetweenBlocksSelection_

#include "TagSelection.h"

class BlockableDisplayNode;
class Node;

class BetweenBlocksSelection : public TagSelection {
public:
	BetweenBlocksSelection(BlockableDisplayNode* parentNodeIn,
                       BlockableDisplayNode* beforeNodeIn);
	void      	Draw(View* view, BPoint origin);
	void      	AcceptKey(string_slice key, DisplayDirector* director);
	BRect 	Bounds();
	bool      	IsVisible(BRect pageRect);
	bool      	NeedsClip();
	void      	Promote(DisplayDirector* director);
	Selection*	ExtendTo(BPoint point);
	Action*   	GetRestoreAction();
	bool      	CanPaste();
	void      	Paste(String pasteText, DisplayDirector* director);
	String 	TagName();
	BRect 	TagSpecRect();
	void      	TagNameChanged(String newTagName, DisplayDirector* director);
	void      	TagEditDone(DisplayDirector* director);
	BPoint	Point();
	Node*     	DocParentNode();
	Node*     	DocBeforeNode();
	void      	SelectForward(DisplayDirector* director);
	void      	SelectBackward(DisplayDirector* director);
	void      	InsertText(String newContents, DisplayDirector* director);

protected:
	BlockableDisplayNode*	parentNode;
	BlockableDisplayNode*	beforeNode;
	static const int     	caretWidth;
	static const int     	caretHeight;
	static const int     	caretLineWidth;
	static const int     	tagXOffset;
	static const int     	tagYOffset;
};


#endif
