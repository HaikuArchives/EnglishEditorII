/* DisplayDirector.cpp */

#include "DisplayDirector.h"
#include "DocumentDisplayNode.h"
#include "ElementDisplayNode.h"
#include "TextDisplayNode.h"
#include "Document.h"
#include "Element.h"
#include "Text.h"
#include "XMLFileSource.h"
#include "Selection.h"
#include "Destination.h"
#include "Hotspot.h"
#include "Action.h"
#include "NullAction.h"
#include "FindSelectionContext.h"
#include "FindDestinationContext.h"
#include "FindHotspotContext.h"
#include "EnglishEditorApp.h"
#include "System.h"
#include "View.h"
#include "InternalException.h"
#include <vector>

class SelectionList : public vector<Selection*> {};



DisplayDirector::DisplayDirector(DocumentSource* docSourceIn)
	: docSource(docSourceIn), selection(NULL), destination(NULL), hotspot(NULL)
{
	// misc ivars
	deletedSelections = new SelectionList();
	sentinalAction = new NullAction();
	lastAction = sentinalAction;
	lastSaveAction = sentinalAction;
}


void DisplayDirector::SetupDisplayNodes()
	// to be called by any subclass' ctor, after the stylesheet is set up
{
	// find the document's DisplayNode
	docDisplayNode =
		dynamic_cast<DocumentDisplayNode*>(DisplayNodeFor(docSource->GetDocument()));
	if (docDisplayNode == NULL)
		throw InternalException("Couldn't get the DocumentDisplayNode!");

	// load
	docDisplayNode->Load(this);
	docDisplayNode->UpdateProperties();

	// layout
	docDisplayNode->BlockLayout();

	// notify the docSource (we do this *after* setting up the display nodes
	// so it will be able to set a selection, etc.)
	docSource->AttachedToDirector(this);
}


DisplayDirector::~DisplayDirector()
{
	if (lastAction)
		lastAction->DeleteAll();
	ClearDeletedSelections();
	delete deletedSelections;
}


DisplayNode* DisplayDirector::DisplayNodeFor(Node* nodeIn, bool create)
{
	Node* node = nodeIn;	/***/
	DisplayNode* existingNode = dynamic_cast<DisplayNode*>(node->GetChangeClient());
	if (existingNode)
		return existingNode;

	// doesn't exist yet, have to make one
	if (!create)
		return NULL;
	switch (node->NodeType()) {
		case DOCUMENT_NODE:
			return new DocumentDisplayNode(dynamic_cast<Document*>(node), this);
			break;

		case ELEMENT_NODE:
			return new ElementDisplayNode(dynamic_cast<Element*>(node));
			break;

		case TEXT_NODE:
		case CDATA_SECTION_NODE:
			return new TextDisplayNode(dynamic_cast<Text*>(node));
			break;
		}

	return NULL;
/***
	DOMLiveNode* node = dynamic_cast<DOMLiveNode*>(nodeIn);
	if (node == NULL)
		return NULL;

	DisplayNode* existingNode = dynamic_cast<DisplayNode*>(node->GetChangeClient());
	if (existingNode)
		return existingNode;

	// doesn't exist yet, have to make one
	if (!create)
		return NULL;
	switch (node->NodeType()) {
		case DOCUMENT_NODE:
			DocumentDisplayNode* newDocNode =
				new DocumentDisplayNode(dynamic_cast<LiveDocument*>(node), this);
			return newDocNode;
			break;

		case ELEMENT_NODE:
			ElementDisplayNode* newElementNode =
				new ElementDisplayNode(dynamic_cast<LiveElement*>(node));
			return newElementNode;
			break;

		case TEXT_NODE:
		case CDATA_SECTION_NODE:
			TextDisplayNode* newTextNode =
				new TextDisplayNode(dynamic_cast<LiveText*>(node));
			return newTextNode;
			break;
		}

	return NULL;
***/
}


void DisplayDirector::MouseDown(int x, int y)
{
	CoordPoint mousePoint(x, y);
	View* view = WindowView();

	// hit the hotspot
	if (hotspot && hotspot->ContainsPoint(ViewToDoc(mousePoint)))
		hotspot->Clicked(this);

	// extend the selection
	else if ((view->CurModifiers() & B_SHIFT_KEY) != 0)
		ExtendSelection();

	// promote or drag the selection
	else if (selection && selection->ContainsPoint(ViewToDoc(mousePoint))) {
		if (view->CurClicks() > 1) {
			StartRefreshCycle();
			selection->Promote(this);
			FinishRefreshCycle();
			ExtendSelection();
			}
		else
			DragSelection(mousePoint);
		}

	// otherwise find a new selection
	else {
		if (DocRect().Contains(mousePoint))
			FindSelection();
		else
			SetSelection(NULL);
		}
}


void DisplayDirector::DoDocAction(DOMString action)
{
	// default: ignore
}


void DisplayDirector::RequestClose()
{
	// default: don't close
}


void DisplayDirector::Paste()
{
	if (selection == NULL || !selection->CanPaste())
		return;

	StartRefreshCycle();

	String clipText = System::GetClipboardText();
	if (!clipText.empty())
		selection->Paste(clipText, this);

	FinishRefreshCycle();
}


void DisplayDirector::SetSelection(Selection* newSelection)
{
	StartRefreshCycle();
	if (selection) {
		RefreshDocRect(selection->Bounds());
		DeleteSelection(selection);
		}
	selection = newSelection;
	if (newSelection)
		RefreshDocRect(newSelection->Bounds());
	FinishRefreshCycle();
}


void DisplayDirector::FindSelection()
{
//***	be_app->HideCursor();
	View* view = WindowView();
	CoordPoint point;
	CoordPoint lastPoint(-1000000, -1000000);
	bool scrolling = false;
	for (;; lastPoint = point) {
		// get & check mouse state
		int buttons = view->GetMouseButtons();
		if (buttons == 0)
			break;
		point = view->GetMousePoint();
		bool autoscrolling = Autoscroll(point);
		if (point == lastPoint && !autoscrolling && !scrolling) {
			view->MouseTrackingPause();
			continue;
			}

		// move the selection
		StartRefreshCycle();
		CoordPoint docPoint = ViewToDoc(point);
		FindSelectionContext context(docPoint.x, docPoint.y);
		Selection* newSelection = docDisplayNode->BlockFindSelection(&context);
		SetSelection(newSelection);
		scrolling = DoScrollStep();
		FinishRefreshCycle();
		ClearDeletedSelections();
		}
//***	be_app->ShowCursor();	//*** didn't work
}


void DisplayDirector::ExtendSelection()
{
	if (selection == NULL)
		return;

	View* view = WindowView();
	CoordPoint lastPoint;
	Selection* rootSelection = selection;
	bool scrolling = false;
	while (true) {
		// get the mouse point and check if moved/finished
		int buttons = view->GetMouseButtons();
		if (buttons == 0)
			break;
		CoordPoint point = view->GetMousePoint();
		Autoscroll(point);
		point = ViewToDoc(point);
		if (point == lastPoint && !scrolling) {
			view->MouseTrackingPause();
			continue;
			}

		// change the selection
		Selection* newSelection = rootSelection->ExtendTo(point);
		if (newSelection != selection) {
			StartRefreshCycle();
			RefreshSelection();

			if (selection != rootSelection)
				delete selection;
			selection = newSelection;

			scrolling = DoScrollStep();

			RefreshSelection();
			FinishRefreshCycle();
			}

		lastPoint = point;
		}

	// clean up root selection
	if (selection != rootSelection)
		delete rootSelection;
}


void DisplayDirector::DragSelection(CoordPoint startPoint)
{
	if (selection == NULL)
		return;

	View* view = WindowView();
	CoordPoint lastPoint = ViewToDoc(startPoint);
	bool scrolling = false;
	bool wasRightButton = false;
	bool optionDown = false;
	while (true) {
		// get the mouse point and check if moved/finished
		CoordPoint point = view->GetMousePoint();
		int buttons = view->GetMouseButtons();
		if (buttons == 0)
			break;
		wasRightButton = ((buttons & (B_SECONDARY_MOUSE_BUTTON | B_TERTIARY_MOUSE_BUTTON)) != 0);
		optionDown = ((view->CurModifiers() & B_OPTION_KEY) != 0);
		scrolling |= Autoscroll(point);
		point = ViewToDoc(point);
		if (point == lastPoint && !scrolling) {
			view->MouseTrackingPause();
			continue;
			}

		// change the destination
		Destination* newDestination = NULL;
		if (!selection->ContainsPoint(point)) {
			FindDestinationContext destContext(selection, point.x, point.y);
			newDestination = docDisplayNode->BlockFindDestination(&destContext);
			}
		if (newDestination != destination) {
			StartRefreshCycle();

			SetDestination(newDestination);

			scrolling = DoScrollStep();

			FinishRefreshCycle();
			}
		else
			scrolling = DoScrollStep();

		lastPoint = point;
		}

	// do the move/copy
	if (destination) {
		// figure out if it's a move or a copy
		bool doing = true;
		bool copying = false;
/***
		if (wasRightButton) {
			BPopUpMenu* menu = new BPopUpMenu("Move/Copy");
			BMenuItem* moveItem = new BMenuItem("Move", NULL);
			menu->AddItem(moveItem);
			BMenuItem* copyItem = new BMenuItem("Copy", NULL);
			menu->AddItem(copyItem);
			BMenuItem* chosenItem = menu->Go(ConvertToScreen(DocToView(lastPoint)), false, true);
			copying = (chosenItem == copyItem);
			if (chosenItem == NULL)
				doing = false;
			delete menu;
			}
		else if (optionDown)
			copying = true;
***/
		if (optionDown)
			copying = true;

		if (doing) {
			StartRefreshCycle();

			// do the move or copy
			Action* moveAction = NULL;
			if (copying)
				moveAction = destination->GetCopyAction();
			else
				moveAction = destination->GetMoveAction();
			if (moveAction)
				DoAction(moveAction);

			SetDestination(NULL);

			FinishRefreshCycle();
			}
		else
			SetDestination(NULL);
		}
}


void DisplayDirector::RefreshSelection()
{
	// helpful for refreshing the selection before triggering a re-layout that moves it
	//** NOTE:  in many situations it's better to use SetSelection(NULL) instead
	if (selection)
		RefreshDocRect(selection->Bounds());
}


Selection* DisplayDirector::GetSelection()
{
	return selection;
}


void DisplayDirector::DeleteSelection(Selection* deletedSelection)
{
	// deletion of selections is deferred, since selections often do things
	// that result in the selection being changed, which "deletes" the selection.
	// However, the selection may still need to exist to do other things.
	deletedSelections->push_back(deletedSelection);
}


void DisplayDirector::ClearDeletedSelections()
{
	for (SelectionList::iterator s = deletedSelections->begin(); s != deletedSelections->end(); ++s)
		delete *s;
	deletedSelections->clear();
}


void DisplayDirector::CopyToClipboard()
{
	if (selection == NULL)
		return;

	DOMString xmlData = selection->GetXMLCopy();
	System::CopyToClipboard(xmlData, xmlData);
}


void DisplayDirector::SetDestination(Destination* newDestination)
{
	if (destination) {
		RefreshDocRect(destination->ArrowBounds());
		delete destination;
		}
	destination = newDestination;
	if (newDestination)
		RefreshDocRect(newDestination->ArrowBounds());
}


void DisplayDirector::UpdateHotspot(CoordPoint mousePoint)
{
	mousePoint = ViewToDoc(mousePoint);

	if (hotspot && hotspot->ContainsPoint(mousePoint))
		return;

	// find the hotspot
	delete hotspot;
	FindHotspotContext context(this, mousePoint.x, mousePoint.y);
	hotspot = docDisplayNode->BlockFindHotspot(&context);
}


void DisplayDirector::DoAction(Action* action)
{
	// do it
	action->Do(this);

	// put it in the undo list
	if (lastAction) {
		// kill undone actions and link new action into the list
		lastAction->DeleteRest();
		action->InsertAfter(lastAction);

		// incorporate into lastAction
		if (lastAction != lastSaveAction && lastAction->CanIncorporateNext(action)) {
			// incorporate
			lastAction->IncorporateNext(action);
			action->Unlink();
			delete action;

			// delete lastAction if it was nulled
			if (lastAction->IsNulled()) {
				Action* prevAction = lastAction->PrevAction();
				lastAction->Unlink();
				delete lastAction;
				lastAction = prevAction;
				}
			}
		else
			lastAction = action;
		}
	else
		lastAction = action;

	// scroll to the selection
	ScrollToSelection();
}


void DisplayDirector::Undo()
{
	StartRefreshCycle();
	if (lastAction != sentinalAction) {
		lastAction->Undo(this);
		lastAction = lastAction->PrevAction();
		}
	FinishRefreshCycle();
	ClearDeletedSelections();
	ScrollToSelection();
}


void DisplayDirector::Redo()
{
	StartRefreshCycle();
	Action* nextAction = lastAction->NextAction();
	if (nextAction) {
		nextAction->Do(this);
		lastAction = nextAction;
		}
	FinishRefreshCycle();
	ClearDeletedSelections();
	ScrollToSelection();
}


void DisplayDirector::Save()
{
	docSource->Save();
	lastSaveAction = lastAction;
}


bool DisplayDirector::IsDirty()
{
	return (lastAction != lastSaveAction);
}


bool DisplayDirector::Autoscroll(CoordPoint point)
{
	// default: do nothing (scrolling not supported by default)
	return false;
}


void DisplayDirector::ScrollToSelection()
{
	// default: do nothing (scrolling not supported by default)
}


bool DisplayDirector::DoScrollStep()		// true => still scrolling
{
	// default: do nothing (scrolling not supported by default)
	return false;
}


int DisplayDirector::GetDocumentHeight()
{
	return docDisplayNode->Height();
}


DocumentSource* DisplayDirector::GetDocSource()
{
	return docSource;
}


void DisplayDirector::RefreshDocRect(Rectangle rect)
{
	RefreshViewRect(DocToView(rect));
}


DOMString DisplayDirector::FunctionCall(DOMString function, DOMString arg, StyleScriptable* target)
{
	if (function == "actionAllowed") {
		bool result = false;
		arg = target->Eval(arg).trim();
		if (arg == "Save()")
			result = IsDirty();
		else if (arg == "Undo()")
			result = (lastAction != sentinalAction);
		else if (arg == "Redo()")
			result = (lastAction->NextAction() != NULL);
		else if (arg == "Copy()" || arg == "Cut()")
			result = (selection && selection->CanCopy());
		else if (arg == "Paste()")
			result = (selection && selection->CanPaste() && !System::GetClipboardText().empty());
		else if (arg.startsWith("doc-source.")) {
			arg = arg.substr(11, arg.length() - 11);
			return docSource->FunctionCall("actionAllowed", arg, target);
			}
		return (result ? "true" : "");
		}
	else if (function == "Undo")
		Undo();
	else if (function == "Redo")
		Redo();
	else if (function == "Save")
		Save();
	else if (function == "Copy")
		CopyToClipboard();
	else if (function == "Paste")
		Paste();
	else
		return StyleScriptable::FunctionCall(function, arg, target);

	return DOMString();
}


DOMString DisplayDirector::GetScriptProperty(DOMString property)
{
	if (property == "doc-source")
		return docSource->AsString();
	else if (property == "app")
		return System::GetApplication()->AsString();
	else
		return StyleScriptable::GetScriptProperty(property);
}


