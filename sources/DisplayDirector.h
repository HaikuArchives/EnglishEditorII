/* DisplayDirector.h */

#ifndef _H_DisplayDirector_
#define _H_DisplayDirector_

#include "StyleScriptable.h"
#include "CoordPoint.h"
#include "Rectangle.h"
#include "DOMString.h"
#include "string_slice.h"

class DocumentSource;
class DocumentDisplayNode;
class View;
class EditStylesheet;
class Validator;
class Selection;
class Destination;
class Hotspot;
class DisplayNode;
class Node;
class Action;
class SelectionList;

class DisplayDirector : public StyleScriptable {
public:
	DisplayDirector(DocumentSource* docSourceIn);
	void                   	SetupDisplayNodes();
	~DisplayDirector();
	DisplayNode*           	DisplayNodeFor(Node* nodeIn, bool create = true);
	virtual void           	Draw(Rectangle updateRect) = 0;
	virtual void           	KeyDown(string_slice key) = 0;
	virtual void           	MouseDown(int x, int y);
	virtual void           	MouseMoved() = 0;
	virtual void           	ScreenChanged() = 0;
	virtual void           	DoDocAction(DOMString action);
	virtual void           	DocTypeChanged() = 0;
	virtual void           	RequestClose();
	virtual void           	Paste();
	void                   	SetSelection(Selection* newSelection);
	void                   	FindSelection();
	void                   	ExtendSelection();
	void                   	DragSelection(CoordPoint startPoint);
	void                   	RefreshSelection();
	Selection*             	GetSelection();
	void                   	DeleteSelection(Selection* deletedSelection);
	void                   	ClearDeletedSelections();
	void                   	CopyToClipboard();
	void                   	SetDestination(Destination* newDestination);
	void                   	UpdateHotspot(CoordPoint mousePoint);
	void                   	DoAction(Action* action);
	void                   	Undo();
	void                   	Redo();
	virtual void           	Save();
	bool                   	IsDirty();
	virtual bool           	Autoscroll(CoordPoint point);
	virtual void           	ScrollToSelection();
	virtual bool           	DoScrollStep();
	virtual EditStylesheet*	GetStylesheet() = 0;
	virtual Validator*     	GetValidator() = 0;
	virtual int            	DisplayWidth() = 0;
	virtual View*          	DrawingView() = 0;
	virtual View*          	WindowView() = 0;
	int                    	GetDocumentHeight();
	DocumentSource*        	GetDocSource();
	virtual CoordPoint     	ViewToDoc(CoordPoint viewPoint) = 0;
	virtual CoordPoint     	DocToView(CoordPoint docPoint) = 0;
	virtual Rectangle      	ViewToDoc(Rectangle rect) = 0;
	virtual Rectangle      	DocToView(Rectangle rect) = 0;
	virtual Rectangle      	DocRect() = 0;
	virtual void           	StartRefreshCycle() = 0;
	virtual void           	FinishRefreshCycle() = 0;
	virtual void           	RefreshViewRect(Rectangle rect) = 0;
	void                   	RefreshDocRect(Rectangle rect);
	virtual void           	RefreshDocAfter(int y) = 0;
	virtual void           	RefreshAll() = 0;
	DOMString              	FunctionCall(DOMString function, DOMString arg, StyleScriptable* target);
	DOMString              	GetScriptProperty(DOMString property);

protected:
	DocumentSource*     	docSource;
	DocumentDisplayNode*	docDisplayNode;
	Selection*          	selection;
	SelectionList*      	deletedSelections;
	Destination*        	destination;
	Hotspot*            	hotspot;
	Action*             	lastAction;
	Action*             	sentinalAction;
	Action*             	lastSaveAction;
};


#endif
