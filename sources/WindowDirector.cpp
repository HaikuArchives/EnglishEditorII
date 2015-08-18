/* WindowDirector.cpp */

#include "WindowDirector.h"
#include "EnglishEditorView.h"
#include "EnglishEditorWind.h"
#include "Bitmap.h"
#include "DocumentSource.h"
#include "Document.h"
#include "Element.h"
#include "DocumentDisplayNode.h"
#include "Selection.h"
#include "Destination.h"
#include "Hotspot.h"
#include "DOMEditStylesheet.h"
#include "CompositeStylesheet.h"
#include "DefaultValidator.h"
#include "MenuDirector.h"
#include "BlockDrawContext.h"
#include "ScrollerControl.h"
#include "StatusControl.h"
#include "MenuControl.h"
#include "HMarginControl.h"
#include "VMarginControl.h"
#include "FrameRateControl.h"
#include "XMLFileSource.h"
#include "Confirmer.h"
#include <Region.h>
#include <Message.h>
#include "System.h"
#include "Constants.h"
#include "Keys.h"
#include "Preferences.h"
#include "qstring.h"
#include "InternalException.h"

const int WindowDirector::DefaultHMargin = 60;
const int WindowDirector::DefaultVMargin = 60;
const int WindowDirector::PageScrollOverlap = 30;
const int WindowDirector::ArrowScrollAmount = 13;


WindowDirector::WindowDirector(DocumentSource* docSourceIn, EnglishEditorView* viewIn, EnglishEditorWind* windIn)
	: DisplayDirector(docSourceIn), view(viewIn), wind(windIn),
	  hMargin(DefaultHMargin), vMargin(DefaultVMargin), menu(NULL),
	  scrollPos(0), scrollTarget(0), curAnimatedScrollPos(0),
	  scrollStepPending(false), scrollMessagePending(false),
	  refreshCycleNesting(0), refreshRegion(NULL),
	  lastScrollFrameTime(0), lastScrollStepTime(0), lastDrawTime(0), lastBlitTime(0)
{
	// stylesheet & validator
	Document* documentNode = docSource->GetDocument();
	stylesheet = new CompositeStylesheet(documentNode->DocumentElement()->TagName());
	validator = new DefaultValidator(this);

	// install into the view
	view->SetDirector(this);

	// make the bitmap
	bitmap = new Bitmap(view->Bounds().Width(), view->Bounds().Height(), view->GetColorSpace());

	// controls
	scrollerControl = new ScrollerControl(this);
	statusControl = new StatusControl(this);
	menuControl = new MenuControl(this);
	hMarginControl = new HMarginControl(this);
	vMarginControl = new VMarginControl(this);
	frameRateControl = new FrameRateControl(this);

	// get the default margins
	int hMarginPref = Prefs()->GetInt32Pref("h-margin");
	if (hMarginPref > 0)
		hMargin = hMarginPref;
	int vMarginPref = Prefs()->GetInt32Pref("v-margin");
	if (vMarginPref > 0)
		vMargin = vMarginPref;

	// restore the window position
	BRect frame = wind->Frame();
	Element* docElement = documentNode->DocumentElement();
	if (docElement) {
		// window frame
		String attr = docElement->GetAttribute("ee2:wind-left");
		if (!attr.empty())
			frame.left = string_slice(attr).asInt();
		attr = docElement->GetAttribute("ee2:wind-top");
		if (!attr.empty())
			frame.top = string_slice(attr).asInt();
		attr = docElement->GetAttribute("ee2:wind-width");
		if (!attr.empty())
			frame.right = frame.left + string_slice(attr).asInt();
		attr = docElement->GetAttribute("ee2:wind-height");
		if (!attr.empty())
			frame.bottom = frame.top + string_slice(attr).asInt();
		wind->SetFrame(frame);

		// margins
		attr = docElement->GetAttribute("ee2:h-margin");
		if (!attr.empty())
			hMargin = string_slice(attr).asInt();
		attr = docElement->GetAttribute("ee2:v-margin");
		if (!attr.empty())
			vMargin = string_slice(attr).asInt();
		}

	SetupDisplayNodes();
}


WindowDirector::~WindowDirector()
{
	delete docSource;
	delete menu;

	delete bitmap;

	delete validator;
	delete stylesheet;

	// controls
	delete scrollerControl;
	delete statusControl;
	delete menuControl;
	delete hMarginControl;
	delete vMarginControl;
	delete frameRateControl;
}


void WindowDirector::Draw(BRect updateRect)
{
	bigtime_t startTime = system_time();

	BPoint origin = DocToView(BPoint(0, 0));

	// calculate the visible page (in document coordinates)
	BRect pageRect = view->Bounds();
	pageRect.InsetBy(hMargin, vMargin);
	bool drawingWholeDoc = updateRect.Contains(pageRect);
	pageRect.OffsetBy(-hMargin, -vMargin + scrollPos);

	// draw document into the offscreen bitmap
	bitmap->Lock();
	View* bitmapView = bitmap->GetView();
	bitmapView->ClearRect(updateRect);
	int vOrigin = vMargin - scrollPos;
	BlockDrawContext
		drawContext(this, bitmapView, hMargin, vOrigin,
		            pageRect.top, pageRect.bottom);
	docDisplayNode->BlockDraw(&drawContext);

	// draw selection
	if (selection && selection->IsVisible(pageRect)) {
		bool needsClip = selection->NeedsClip();
		if (needsClip) {
			bitmapView->PushState();
			BRect clipRect = DocToView(pageRect);
			// don't clip top if scrolled to top, or bottom if scrolled to bottom
			if (scrollPos == 0)
				clipRect.top = view->Bounds().top;
			else if (scrollPos == LastScrollPos())
				clipRect.bottom = view->Bounds().bottom;
			clipRect.left = view->Bounds().left;
			clipRect.right = view->Bounds().right;
			BRegion clipRgn;
			clipRgn.Include(clipRect);
			bitmapView->ConstrainClippingRegion(&clipRgn);
			}

		selection->Draw(bitmapView, origin);

		if (needsClip)
			bitmapView->PopState();
		}

	// draw destination
	if (destination)
		destination->DrawArrow(bitmapView, origin);

	// draw hotspot
	if (hotspot)
		hotspot->Draw(bitmapView, origin);

	// draw controls
	if (scrollerControl)
		scrollerControl->Draw(this);
	if (frameRateControl)
		frameRateControl->Draw(this);
	if (menuControl)
		menuControl->Draw(this);
	if (hMarginControl)
		hMarginControl->Draw(this);
	if (vMarginControl)
		vMarginControl->Draw(this);
	if (statusControl)
		statusControl->Draw(this);

	// draw menu
	if (menu)
		menu->Draw(updateRect);

/***
	// TEST: draw border
	BRect borderRect = view->Bounds();
	borderRect.InsetBy(hMargin, vMargin);
	bitmap->GetView()->StrokeRect(borderRect);
***/

	// sync
	bitmapView->Sync();

	// finish timing draw
	bigtime_t curTime = system_time();
	lastDrawTime = curTime - startTime;

	// copy to the screen
	view->DrawBitmap(bitmap, updateRect);
	view->Sync();
	bitmap->Unlock();

	// finish timing blit
	lastBlitTime = system_time() - curTime;

	// time the frame rate
	bigtime_t now = system_time();
	if (lastScrollStepTime != 0 && drawingWholeDoc)
		lastScrollFrameTime = now - lastScrollStepTime;
	lastScrollStepTime = (scrollStepPending ? now : 0);
}


void WindowDirector::KeyDown(string_slice key)
{
	static const string_slice spaceKey = " ";

	try {

	// give the selection a shot at special keys
	if (selection && selection->HandleSpecialKey(key, this)) {
		// don't do anything else
		}

	// scrolling keys
	else if (key == Keys::PageUpKey)
		SetScrollTarget(scrollTarget - (view->Bounds().Height() - 2*vMargin - PageScrollOverlap));
	else if (key == spaceKey && selection == NULL) {
		if (docSource->CanCloseOnSpacebar() && scrollPos == LastScrollPos())
			wind->RequestClose();
		else
			SetScrollTarget(scrollTarget + (view->Bounds().Height() - 2*vMargin - PageScrollOverlap));
		}
	else if (key == Keys::PageDownKey)
		SetScrollTarget(scrollTarget + (view->Bounds().Height() - 2*vMargin - PageScrollOverlap));
	else if (key == Keys::HomeKey)
		SetScrollTarget(-0.5 * GetDocumentHeight());
	else if (key == Keys::EndKey)
		SetScrollTarget(GetDocumentHeight() * 1.5);
	else if (key == Keys::UpArrowKey)
		SetScrollTarget(scrollTarget - ArrowScrollAmount);
	else if (key == Keys::DownArrowKey)
		SetScrollTarget(scrollTarget + ArrowScrollAmount);

	// ESC => undo/redo
	else if (key == Keys::EscKey) {
		if ((view->CurModifiers() & B_SHIFT_KEY) != 0)
			Redo();
		else
			Undo();
		}

	// command keys
	else if ((view->CurModifiers() & B_COMMAND_KEY) != 0) {
		switch (*key.begin()) {
			case 's':
			case 'S':
				Save();
				break;

			case 'z':
			case 'Z':
				if ((view->CurModifiers() & B_SHIFT_KEY) != 0)
					Redo();
				else
					Undo();
				break;

			case 'c':
			case 'C':
				CopyToClipboard();
				break;

			default:
				// give the menu a whack at it
				{
				DocumentSource* menuDocSource = docSource->MakeMenuDocSource();
				if (menuDocSource == NULL)
					break;
				String keyName = MenuDirector::MakeKeyName(key, view->CurModifiers());
				Element* menuDocElement =
					menuDocSource->GetDocument()->DocumentElement();
				String action =
					MenuDirector::FindKeyActionIn(keyName, menuDocElement).detach();
				delete menuDocSource;
				if (!action.empty())
					DoDocAction(action);
				}
				break;
			}
		}

	// let the selection have it
	else if (selection) {
		StartRefreshCycle();
		selection->AcceptKey(key, this);
		FinishRefreshCycle();
		ClearDeletedSelections();
		}

	} catch (DisplayableException& e) {
		e.Display();
		}
}


void WindowDirector::MouseDown(int x, int y)
{
	try {

	BPoint mousePoint(x, y);

	// menu
	if (menu) {
		if (menu->DocRect().Contains(mousePoint)) {
			menu->MouseDown(x, y);
			return;
			}
		else if (!menuControl->GetRect().Contains(mousePoint)) {
			// sorta a hack--menuControl is just gonna re-show the menu if
			// we hide it now; just let it hide it
			HideMenu();
			}
		}

	// controls get it first
	if (scrollerControl && scrollerControl->GetRect().Contains(mousePoint))
		scrollerControl->MouseDown(x, y, this);
	else if (menuControl && menuControl->GetRect().Contains(mousePoint))
		menuControl->MouseDown(x, y, this);
	else if (hMarginControl && hMarginControl->GetRect().Contains(mousePoint))
		hMarginControl->MouseDown(x, y, this);
	else if (vMarginControl && vMarginControl->GetRect().Contains(mousePoint))
		vMarginControl->MouseDown(x, y, this);
	else if (frameRateControl && frameRateControl->GetRect().Contains(mousePoint))
		frameRateControl->MouseDown(x, y, this);

	// hotspot & selection handling
	else
		DisplayDirector::MouseDown(x, y);

	} catch (DisplayableException& e) {
		e.Display();
		}
}


void WindowDirector::MouseMoved()
{
	UpdateHotspot(view->GetMousePoint());
	if (menu)
		menu->MouseMoved();

	// update the display, scrolling if necessary
	if (scrollTarget != scrollPos)
		DoScrollStep();
	else
		Draw(view->Bounds());
}


void WindowDirector::FrameChanged(BRect newFrame)
{
	// record the change in the document
	Element* docElement = docSource->GetDocument()->DocumentElement();
	docElement->SetAttribute("xmlns:ee2", "http://folta.net/steve/EnglishEditorII");
	docElement->SetAttribute("ee2:wind-left", String(newFrame.left));
	docElement->SetAttribute("ee2:wind-top", String(newFrame.top));
	docElement->SetAttribute("ee2:wind-width", String(newFrame.Width()));
	docElement->SetAttribute("ee2:wind-height", String(newFrame.Height()));
	docElement->SetAttribute("ee2:h-margin", String(hMargin));
	docElement->SetAttribute("ee2:v-margin", String(vMargin));

	// size might have changed, so update bitmap
	ScreenChanged();
}


void WindowDirector::ScreenChanged()
{
	// get the vital stats
	bitmap->Lock();
	View* bitmapView = bitmap->GetView();
	long newColorSpace = view->GetColorSpace();
	long oldColorSpace = bitmap->GetColorSpace();
	float oldWidth = bitmapView->Bounds().Width();
	float newWidth = view->Bounds().Width();
	float oldHeight = bitmapView->Bounds().Height();
	float newHeight = view->Bounds().Height();
	bitmap->Unlock();

	// if nothing's changed, kick out
	if (newColorSpace == oldColorSpace && newWidth == oldWidth && newHeight == oldHeight)
		return;

	// rebuild the bitmap
	float maxWidth = oldWidth;
	if (newWidth > maxWidth)
		maxWidth = newWidth;
	float maxHeight = oldHeight;
	if (newHeight > maxHeight)
		maxHeight = newHeight;
	delete bitmap;
	bitmap = new Bitmap(maxWidth, maxHeight, newColorSpace);

	// relayout
	if (newWidth != oldWidth)
		docDisplayNode->BlockLayout(true);

	// refresh
	RefreshViewRect(view->Bounds());
}


bool WindowDirector::CloseRequested()
{
	if (!IsDirty())
		return true;

	// confirm close without saving
	qstring prompt = "\"";
	prompt += docSource->GetWindowTitle();
	prompt += "\" has unsaved changes.  Close it anyway?";
	int choice = Confirmer::Confirm(prompt.c_str(), "Cancel", "Don’t Save", "Save");
	if (choice == 2)
		Save();
	return (choice != 0);
}


void WindowDirector::DoDocAction(String action)
{
	HideMenu();

	Eval(action);
/***
	if (action == "Undo()")
		Undo();
	else if (action == "Redo()")
		Redo();
	else if (action == "Save()")
		Save();
***/
}


void WindowDirector::DocTypeChanged()
{
	// stylesheet & validator
	Document* documentNode = docSource->GetDocument();
	delete stylesheet;
	delete validator;
	stylesheet = new CompositeStylesheet(documentNode->DocumentElement()->TagName());
	validator = new DefaultValidator(this);

	// update display
	// find the document's DisplayNode
	docDisplayNode =
		dynamic_cast<DocumentDisplayNode*>(DisplayNodeFor(docSource->GetDocument()));
	if (docDisplayNode == NULL)
		throw InternalException("WindowDirector::DocTypeChanged: Couldn't get the DocumentDisplayNode!");
	// load
	docDisplayNode->Load(this);
	docDisplayNode->UpdateProperties();
	// layout
	docDisplayNode->BlockLayout();
}


void WindowDirector::RequestClose()
{
	wind->RequestClose();
}


void WindowDirector::KillControls()
{
	delete scrollerControl;
	scrollerControl = NULL;
	delete statusControl;
	statusControl = NULL;
	delete menuControl;
	menuControl = NULL;
	delete hMarginControl;
	hMarginControl = NULL;
	delete vMarginControl;
	vMarginControl = NULL;
	delete frameRateControl;
	frameRateControl = NULL;
}


void WindowDirector::SetScrollTarget(float newScrollTarget)		// *must* be called prior to any DoScrollStep() cycle
{
	scrollTarget = (int) newScrollTarget;

	// take this call as the signal that scrolling is about to start...
	// set up scrolling internals
	curAnimatedScrollPos = scrollPos;
//***	lastScrollStepTime = 0;		// signal timing mechanism that last cycle is done
	// start scroll messages
	ContinueScrolling();
}


bool WindowDirector::Autoscroll(BPoint point)
{
	static const float AutoscrollMultiplier = 5;

	BRect pageRect = view->Bounds();
	pageRect.InsetBy(hMargin, vMargin);

	// up
	if (point.y < pageRect.top)
		SetScrollTarget(scrollPos - (pageRect.top - point.y) * AutoscrollMultiplier);

	// down
	else if (point.y > pageRect.bottom)
		SetScrollTarget(scrollPos + (point.y - pageRect.bottom) * AutoscrollMultiplier);

	else
		return false;

	return true;
}


void WindowDirector::ScrollToSelection()
{
	if (selection == NULL)
		return;

	// if the selection is visible, don't scroll
	BRect visibleRect = ViewToDoc(DocRect());
	if (selection->IsVisible(visibleRect))
		return;

	// need to scroll
	// if the selection fits on the screen, scroll it to the middle
	BRect selectionRect = selection->Bounds();
	if (selectionRect.Height() <= visibleRect.Height())
		SetScrollTarget((selectionRect.top + selectionRect.bottom - visibleRect.Height()) / 2);
	// otherwise, scroll to the top of the selection
	else
		SetScrollTarget(selectionRect.top);
}


bool WindowDirector::DoScrollStep()		// true => still scrolling
{
	static const float CloseEnough = 1;
	static const float MinScrollAmount = 0.5;	// was 0.5, then 1

	static const float XenoFactorPerSecond = 0.8;

	scrollStepPending = false;

	if (scrollPos == scrollTarget)
		return false;

	StartRefreshCycle();

//***	bigtime_t startTime = system_time();

	// go a fraction of the distance
	bool scrolling = true;
	float newScrollPos, scrollAmount;
	float frameRate = (lastScrollFrameTime ? 1000000.0/lastScrollFrameTime : 1000000.0/lastDrawTime);
	if (scrollTarget < curAnimatedScrollPos) {
		// up
		newScrollPos = scrollTarget -
			(scrollTarget - curAnimatedScrollPos) * pow((1 - XenoFactorPerSecond), 1 / frameRate);
		scrollAmount = curAnimatedScrollPos - newScrollPos;
		if (scrollAmount < MinScrollAmount)
			newScrollPos = curAnimatedScrollPos - MinScrollAmount;
		if (newScrollPos - scrollTarget <= CloseEnough) {
			newScrollPos = scrollTarget;
			scrolling = false;
			}
		}
	else {
		// t - (t-y)( (1-z)^(1/f) )
		newScrollPos = scrollTarget -
			(scrollTarget - curAnimatedScrollPos) * pow((1 - XenoFactorPerSecond), 1 / frameRate);
		scrollAmount = newScrollPos - curAnimatedScrollPos;
		if (scrollAmount < MinScrollAmount)
			newScrollPos = curAnimatedScrollPos + MinScrollAmount;
		if (scrollTarget - newScrollPos <= CloseEnough) {
			newScrollPos = scrollTarget;
			scrolling = false;
			}
		}
	ScrollDocumentTo(floor(newScrollPos + 0.5));
	curAnimatedScrollPos = newScrollPos;
	if (scrollPos == 0 || scrollPos == LastScrollPos())
		scrolling = false;
	if (!scrolling)
		scrollTarget = scrollPos;
	else
		ContinueScrolling();

	FinishRefreshCycle();

/***
	// time the frame rate
	bigtime_t now = system_time();
	if (lastScrollStepTime != 0)
		lastScrollFrameTime = now - lastScrollStepTime;
	lastScrollStepTime = now;
***/

	return scrolling;
}


void WindowDirector::ContinueScrolling()
{
	// sometimes (eg. during operation of the ScrollerControl) the scrolling happens in
	// a loop before returning the message queue handing.  In that case, if we send a
	// scroll-continuation message every time, we end up "poisoning" our response as the
	// queue fills up (and the GetMouse(..., true) call takes longer and longer to look
	// through the queue?).  Avoid this by making sure we don't send a second scroll-continuation
	// message if there's already in the queue.
	if (!scrollMessagePending) {
		BMessage scrollStepMessage(ScrollStepMessage);
		view->SendMessage(&scrollStepMessage);
		scrollMessagePending = true;
		}
	scrollStepPending = true;
}


void WindowDirector::ScrollDocumentTo(int newScrollPos)
{
	// scroll, pinning to beginning and end
//***	StartRefreshCycle();
	scrollPos = newScrollPos;
	if (scrollPos < 0)
		scrollPos = 0;
	else {
		float scrollStop = docDisplayNode->Height() - (view->Bounds().Height() - 2 * vMargin);
		if (scrollStop < 0)
			scrollStop = 0;
		if (scrollPos > scrollStop)
			scrollPos = (int) scrollStop;
		}
	RefreshViewRect(view->Bounds());
//***	FinishRefreshCycle();
}


void WindowDirector::ScrollMessageReceived()
{
	StartRefreshCycle();
	scrollMessagePending = false;
	DoScrollStep();
	FinishRefreshCycle();
}


int WindowDirector::LastScrollPos()
{
	float lastScrollPos = docDisplayNode->Height() - (view->Bounds().Height() - 2 * vMargin);
	if (lastScrollPos < 0)
		lastScrollPos = 0;
	return (int) lastScrollPos;
}


void WindowDirector::Save()
{
	StartRefreshCycle();

	DisplayDirector::Save();
	RefreshViewRect(statusControl->GetRect());

	FinishRefreshCycle();
}


void WindowDirector::ShowMenu()
{
	if (menu)
		return;

	DocumentSource* menuDocSource = docSource->MakeMenuDocSource();
	if (menuDocSource == NULL)
		return;

	StartRefreshCycle();

	BRect menuControlRect = menuControl->GetRect();
	BPoint stemPoint((menuControlRect.left + menuControlRect.right) / 2,
	                     menuControlRect.bottom);
	menu = new MenuDirector(menuDocSource, stemPoint, this);

	RefreshViewRect(menu->Bounds());
	FinishRefreshCycle();
}


void WindowDirector::HideMenu()
{
	if (menu == NULL)
		return;

	StartRefreshCycle();
	RefreshViewRect(menu->Bounds());

	delete menu;
	menu = NULL;

	FinishRefreshCycle();
}


bool WindowDirector::MenuIsVisible()
{
	return (menu != NULL);
}


int WindowDirector::GetHMargin()
{
	return hMargin;
}


void WindowDirector::SetHMargin(int newHMargin)
{
	hMargin = newHMargin;
	docDisplayNode->BlockLayout(true);
	RefreshViewRect(ViewBounds());

	// record the change
	Element* docElement = docSource->GetDocument()->DocumentElement();
	docElement->SetAttribute("ee2:h-margin", String(hMargin));
}


int WindowDirector::GetVMargin()
{
	return vMargin;
}


void WindowDirector::SetVMargin(int newVMargin)
{
	vMargin = newVMargin;
	RefreshViewRect(ViewBounds());

	// record the change
	Element* docElement = docSource->GetDocument()->DocumentElement();
	docElement->SetAttribute("ee2:v-margin", String(vMargin));
}


EditStylesheet* WindowDirector::GetStylesheet()
{
	return stylesheet;
}


Validator* WindowDirector::GetValidator()
{
	return validator;
}


int WindowDirector::DisplayWidth()
{
	return view->Bounds().Width() - 2 * hMargin;
}


BRect WindowDirector::ViewBounds()
{
	return view->Bounds();
}


View* WindowDirector::DrawingView()
{
	return bitmap->GetView();
}


View* WindowDirector::WindowView()
{
	return view;
}


bigtime_t WindowDirector::LastScrollFrameTime()
{
	return lastScrollFrameTime;
}


bigtime_t WindowDirector::LastDrawTime()
{
	return lastDrawTime;
}


bigtime_t WindowDirector::LastBlitTime()
{
	return lastBlitTime;
}


int WindowDirector::GetScrollPos()
{
	return scrollPos;
}


float WindowDirector::GetVisibleProportion()
{
	float proportion =
		(view->Bounds().Height() - 2 * vMargin) / docDisplayNode->Height();
	if (proportion > 1.0)
		proportion = 1.0;
	return proportion;
}


BPoint WindowDirector::ViewToDoc(BPoint viewPoint)
{
	return BPoint(viewPoint.x - hMargin, viewPoint.y - vMargin + scrollPos);
}


BPoint WindowDirector::DocToView(BPoint docPoint)
{
	return BPoint(docPoint.x + hMargin, docPoint.y + vMargin - scrollPos);
}


BRect WindowDirector::ViewToDoc(BRect rect)
{
	BRect docRect = rect;
	docRect.OffsetBy(-hMargin, -(vMargin - scrollPos));
	return docRect;
}


BRect WindowDirector::DocToView(BRect rect)
{
	BRect viewRect = rect;
	viewRect.OffsetBy(hMargin, vMargin - scrollPos);
	return viewRect;
}


BRect WindowDirector::DocRect()
{
	BRect rect = view->Bounds();
	rect.InsetBy(hMargin, vMargin);
	return rect;
}


void WindowDirector::StartRefreshCycle()
{
	refreshCycleNesting++;
	if (refreshRegion == NULL)
		refreshRegion = new BRegion();
}


void WindowDirector::FinishRefreshCycle()
{
	--refreshCycleNesting;
	if (refreshCycleNesting > 0 || refreshRegion == NULL)
		return;

	// refresh controls
/***
	BRegion controlRefreshRegion;
	BPoint mousePoint;
	uint32 buttons;
	GetMouse(&mousePoint, &buttons);
	RefreshControls(mousePoint, &controlRefreshRegion);
	refreshRegion->Include(&controlRefreshRegion);
***/

	// draw
	if (refreshRegion->CountRects() > 0) {
		view->Lock();
		Draw(view->Bounds());
		view->Unlock();
		}

	// clean up
	delete refreshRegion;
	refreshRegion = NULL;
}


void WindowDirector::RefreshViewRect(BRect rect)
{
	if (refreshRegion)
		refreshRegion->Include(rect);
	else {
		view->Lock();
		Draw(rect);
		view->Unlock();
		}
}


void WindowDirector::RefreshDocAfter(int y)
{
	BRect rect = view->Bounds();
	rect.InsetBy(hMargin, vMargin);
	rect.top += y - scrollPos;
	RefreshViewRect(rect);
}


void WindowDirector::RefreshAll()
{
	RefreshViewRect(view->Bounds());
}


String WindowDirector::FunctionCall(String function, String arg, StyleScriptable* target)
{
	if (function == "actionAllowed") {
		arg = target->Eval(arg).trim();
		if (arg == "SavePosition()")
			return "true";
		else
			return DisplayDirector::FunctionCall(function, arg, target);
		}
	else if (function == "SavePosition") {
		Prefs()->SetRectPref("defaultWindFrame", wind->Frame());
		Prefs()->SetInt32Pref("h-margin", hMargin);
		Prefs()->SetInt32Pref("v-margin", vMargin);
		}
	else
		return DisplayDirector::FunctionCall(function, arg, target);

	return String();
}


