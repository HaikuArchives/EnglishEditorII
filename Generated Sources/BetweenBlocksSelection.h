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
	void      	Draw(View* view, CoordPoint origin);
	void      	AcceptKey(string_slice key, DisplayDirector* director);
	Rectangle 	Bounds();
	bool      	IsVisible(Rectangle pageRect);
	bool      	NeedsClip();
	void      	Promote(DisplayDirector* director);
	Selection*	ExtendTo(CoordPoint point);
	Action*   	GetRestoreAction();
	bool      	CanPaste();
	void      	Paste(String pasteText, DisplayDirector* director);
	DOMString 	TagName();
	Rectangle 	TagSpecRect();
	void      	TagNameChanged(DOMString newTagName, DisplayDirector* director);
	void      	TagEditDone(DisplayDirector* director);
	CoordPoint	Point();
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
