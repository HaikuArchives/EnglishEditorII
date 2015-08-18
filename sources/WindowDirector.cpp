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
#include "Region.h"
#include "Message.h"
#include "System.h"
#include "Messages.h"
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
	bitmap = new Bitmap(view->Width(), view->Height(), view->GetColorSpace());

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
	Rectangle frame = wind->Frame();
	Element* docElement = documentNode->DocumentElement();
	if (docElement) {
		// window frame
		DOMString attr = docElement->GetAttribute("ee2:wind-left");
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


void WindowDirector::Draw(Rectangle updateRect)
{
	Microseconds startTime = Timing::SystemTime();

	CoordPoint origin = DocToView(CoordPoint(0, 0));

	// calculate the visible page (in document coordinates)
	Rectangle pageRect = view->Bounds();
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
			Rectangle clipRect = DocToView(pageRect);
			// don't clip top if scrolled to top, or bottom if scrolled to bottom
			if (scrollPos == 0)
				clipRect.top = view->Bounds().top;
			else if (scrollPos == LastScrollPos())
				clipRect.bottom = view->Bounds().bottom;
			clipRect.left = view->Bounds().left;
			clipRect.right = view->Bounds().right;
			Region clipRgn;
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
	Rectangle borderRect = view->Bounds();
	borderRect.InsetBy(hMargin, vMargin);
	bitmap->GetView()->StrokeRect(borderRect);
***/

	// sync
	bitmapView->Sync();

	// finish timing draw
	Microseconds curTime = Timing::SystemTime();
	lastDrawTime = curTime - startTime;

	// copy to the screen
	view->DrawBitmap(bitmap, updateRect);
	view->Sync();
	bitmap->Unlock();

	// finish timing blit
	lastBlitTime = Timing::SystemTime() - curTime;

	// time the frame rate
	Microseconds now = Timing::SystemTime();
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
		SetScrollTarget(scrollTarget - (view->Height() - 2*vMargin - PageScrollOverlap));
	else if (key == spaceKey && selection == NULL) {
		if (docSource->CanCloseOnSpacebar() && scrollPos == LastScrollPos())
			wind->RequestClose();
		else
			SetScrollTarget(scrollTarget + (view->Height() - 2*vMargin - PageScrollOverlap));
		}
	else if (key == Keys::PageDownKey)
		SetScrollTarget(scrollTarget + (view->Height() - 2*vMargin - PageScrollOverlap));
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
				DOMString keyName = MenuDirector::MakeKeyName(key, view->CurModifiers());
				Element* menuDocElement =
					menuDocSource->GetDocument()->DocumentElement();
				DOMString action =
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

	CoordPoint mousePoint(x, y);

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


void WindowDirector::FrameChanged(Rectangle newFrame)
{
	// record the change in the document
	Element* docElement = docSource->GetDocument()->DocumentElement();
	docElement->SetAttribute("xmlns:ee2", "http://folta.net/steve/EnglishEditorII");
	docElement->SetAttribute("ee2:wind-left", DOMString(newFrame.left));
	docElement->SetAttribute("ee2:wind-top", DOMString(newFrame.top));
	docElement->SetAttribute("ee2:wind-width", DOMString(newFrame.Width()));
	docElement->SetAttribute("ee2:wind-height", DOMString(newFrame.Height()));
	docElement->SetAttribute("ee2:h-margin", DOMString(hMargin));
	docElement->SetAttribute("ee2:v-margin", DOMString(vMargin));

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
	int oldWidth = bitmapView->Width();
	int newWidth = view->Width();
	int oldHeight = bitmapView->Height();
	int newHeight = view->Height();
	bitmap->Unlock();

	// if nothing's changed, kick out
	if (newColorSpace == oldColorSpace && newWidth == oldWidth && newHeight == oldHeight)
		return;

	// rebuild the bitmap
	int maxWidth = oldWidth;
	if (newWidth > maxWidth)
		maxWidth = newWidth;
	int maxHeight = oldHeight;
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


void WindowDirector::DoDocAction(DOMString action)
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


bool WindowDirector::Autoscroll(CoordPoint point)
{
	static const float AutoscrollMultiplier = 5;

	Rectangle pageRect = view->Bounds();
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
	Rectangle visibleRect = ViewToDoc(DocRect());
	if (selection->IsVisible(visibleRect))
		return;

	// need to scroll
	// if the selection fits on the screen, scroll it to the middle
	Rectangle selectionRect = selection->Bounds();
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

//***	Microseconds startTime = Timing::SystemTime();

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
	Microseconds now = Timing::SystemTime();
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
		Message scrollStepMessage(ScrollStepMessage);
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
		float scrollStop = docDisplayNode->Height() - (view->Height() - 2 * vMargin);
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
	float lastScrollPos = docDisplayNode->Height() - (view->Height() - 2 * vMargin);
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

	Rectangle menuControlRect = menuControl->GetRect();
	CoordPoint stemPoint((menuControlRect.left + menuControlRect.right) / 2,
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
	docElement->SetAttribute("ee2:h-margin", DOMString(hMargin));
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
	docElement->SetAttribute("ee2:v-margin", DOMString(vMargin));
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
	return view->Width() - 2 * hMargin;
}


Rectangle WindowDirector::ViewBounds()
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


Microseconds WindowDirector::LastScrollFrameTime()
{
	return lastScrollFrameTime;
}


Microseconds WindowDirector::LastDrawTime()
{
	return lastDrawTime;
}


Microseconds WindowDirector::LastBlitTime()
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


CoordPoint WindowDirector::ViewToDoc(CoordPoint viewPoint)
{
	return CoordPoint(viewPoint.x - hMargin, viewPoint.y - vMargin + scrollPos);
}


CoordPoint WindowDirector::DocToView(CoordPoint docPoint)
{
	return CoordPoint(docPoint.x + hMargin, docPoint.y + vMargin - scrollPos);
}


Rectangle WindowDirector::ViewToDoc(Rectangle rect)
{
	Rectangle docRect = rect;
	docRect.OffsetBy(-hMargin, -(vMargin - scrollPos));
	return docRect;
}


Rectangle WindowDirector::DocToView(Rectangle rect)
{
	Rectangle viewRect = rect;
	viewRect.OffsetBy(hMargin, vMargin - scrollPos);
	return viewRect;
}


Rectangle WindowDirector::DocRect()
{
	Rectangle rect = view->Bounds();
	rect.InsetBy(hMargin, vMargin);
	return rect;
}


void WindowDirector::StartRefreshCycle()
{
	refreshCycleNesting++;
	if (refreshRegion == NULL)
		refreshRegion = new Region();
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


void WindowDirector::RefreshViewRect(Rectangle rect)
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
	Rectangle rect = view->Bounds();
	rect.InsetBy(hMargin, vMargin);
	rect.top += y - scrollPos;
	RefreshViewRect(rect);
}


void WindowDirector::RefreshAll()
{
	RefreshViewRect(view->Bounds());
}


DOMString WindowDirector::FunctionCall(DOMString function, DOMString arg, StyleScriptable* target)
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

	return DOMString();
}


