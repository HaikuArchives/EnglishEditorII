/* MenuDirector.cpp */

#include "MenuDirector.h"
#include "WindowDirector.h"
#include "DocumentSource.h"
#include "Document.h"
#include "Element.h"
#include "DocumentDisplayNode.h"
#include "Selection.h"
#include "Hotspot.h"
#include "CompositeStylesheet.h"
#include "DefaultValidator.h"
#include "BlockDrawContext.h"
#include "StyleParser.h"
#include "Shape.h"
#include "View.h"
#include "Keys.h"

const int MenuDirector::stemHeight = 0;
const int MenuDirector::stemWidth = 0;
const int MenuDirector::cornerRadius = 8;
const int MenuDirector::borderWidth = 3;
const rgb_color MenuDirector::borderColor = StyleParser::ParseColor("rgba(0, 255, 0, 128)");
rgb_color MenuDirector::bgndColor = { 255, 255, 255, 192 };
const int MenuDirector::hMargin = 12;
const int MenuDirector::vMargin = 8;


MenuDirector::MenuDirector(DocumentSource* docSourceIn, CoordPoint stemPointIn, WindowDirector* windowDirectorIn)
	: DisplayDirector(docSourceIn), stemPoint(stemPointIn),
	  windowDirector(windowDirectorIn)
{
	// stylesheet & validator
	Document* documentNode = docSource->GetDocument();
	stylesheet = new CompositeStylesheet(documentNode->DocumentElement()->TagName());
	validator = new DefaultValidator(this);

	// create & layout the display nodes at full width
	docWidth = windowDirector->DisplayWidth() - 2 * hMargin - 2 * borderWidth;
	SetupDisplayNodes();

	// relayout at the document's width
	docWidth = docDisplayNode->MaxLineWidth();
	docDisplayNode->BlockLayout(true);
}


MenuDirector::~MenuDirector()
{
	delete validator;
	delete stylesheet;
	delete docSource;
}


void MenuDirector::Draw(Rectangle updateRect)
{
	View* drawView = DrawingView();
	drawView->PushState();

	// make the outline shape
	Rectangle contentRect = DocRect();
	int lineSpill = borderWidth / 2;
	contentRect.InsetBy(-(hMargin + lineSpill), -(vMargin + lineSpill));
	int shiftBy = (int) ((stemPoint.x - lineSpill) - contentRect.right);
	contentRect.OffsetBy(shiftBy, 0);
	Shape shape;
	shape.MoveTo(stemPoint.x - lineSpill, stemPoint.y);
	shape.LineTo(contentRect.right - stemWidth, contentRect.top - cornerRadius);
	shape.VerticalCornerTo(contentRect.right - stemWidth - cornerRadius, contentRect.top);
	shape.LineTo(contentRect.left + cornerRadius, contentRect.top);
	shape.HorizontalCornerTo(contentRect.left, contentRect.top + cornerRadius);
	shape.LineTo(contentRect.left, contentRect.bottom - cornerRadius);
	shape.VerticalCornerTo(contentRect.left + cornerRadius, contentRect.bottom);
	shape.LineTo(contentRect.right - cornerRadius, contentRect.bottom);
	shape.HorizontalCornerTo(contentRect.right, contentRect.bottom - cornerRadius);
	shape.LineTo(stemPoint.x - lineSpill, stemPoint.y);
	shape.Close();

	// clear the outline
	drawView->MovePenTo(CoordPoint(0, 0));
	drawView->SetDrawingMode(B_OP_ALPHA);
	drawView->SetHighColor(bgndColor);
	drawView->FillShape(&shape);

	// draw the document
	static const rgb_color blackColor = { 0, 0, 0, 255 };
	drawView->SetHighColor(blackColor);
	drawView->SetDrawingMode(B_OP_COPY);
	contentRect = DocRect();
//***	contentRect.InsetBy(hMargin + lineSpill, vMargin + lineSpill);	// restore it to the DocRect()
	BlockDrawContext
		drawContext(this, drawView, contentRect.left, contentRect.top,
		            0, docDisplayNode->Height());
	docDisplayNode->BlockDraw(&drawContext);

	// draw selection
	CoordPoint origin = contentRect.LeftTop();
	if (selection)
		selection->Draw(drawView, origin);

	// draw hotspot
	if (hotspot)
		hotspot->Draw(drawView, origin);

	// draw the outline
	drawView->MovePenTo(CoordPoint(0, 0));
	drawView->SetDrawingMode(B_OP_ALPHA);
	drawView->SetPenSize(borderWidth);
	drawView->SetHighColor(borderColor);
	drawView->StrokeShape(&shape);

	drawView->PopState();
}


void MenuDirector::KeyDown(string_slice key)
{
	// not really used...
	if (selection) {
		StartRefreshCycle();
		selection->AcceptKey(key, this);
		FinishRefreshCycle();
		}
}


void MenuDirector::MouseMoved()
{
	StartRefreshCycle();
	UpdateHotspot(WindowView()->GetMousePoint());
	FinishRefreshCycle();
}


void MenuDirector::ScreenChanged()
{
	// nothing to do
}


EditStylesheet* MenuDirector::GetStylesheet()
{
	return stylesheet;
}


Validator* MenuDirector::GetValidator()
{
	return validator;
}


int MenuDirector::DisplayWidth()
{
	return docWidth;
}


View* MenuDirector::DrawingView()
{
	return windowDirector->DrawingView();
}


View* MenuDirector::WindowView()
{
	return windowDirector->WindowView();
}


CoordPoint MenuDirector::ViewToDoc(CoordPoint viewPoint)
{
	Rectangle docRect = DocRect();
	return CoordPoint(viewPoint.x - docRect.left, viewPoint.y - docRect.top);
}


CoordPoint MenuDirector::DocToView(CoordPoint docPoint)
{
	Rectangle docRect = DocRect();
	return CoordPoint(docPoint.x + docRect.left, docPoint.y + docRect.top);
}


Rectangle MenuDirector::ViewToDoc(Rectangle rect)
{
	Rectangle docRect = DocRect();
	rect.OffsetBy(-docRect.left, -docRect.top);
	return rect;
}


Rectangle MenuDirector::DocToView(Rectangle rect)
{
	Rectangle docRect = DocRect();
	rect.OffsetBy(docRect.left, docRect.top);
	return rect;
}


Rectangle MenuDirector::DocRect()
{
	Rectangle docRect;
//***	int lineSpill = borderWidth / 2;	//*** was I supposed to use this?
	docRect.top = stemPoint.y + stemHeight + cornerRadius + borderWidth + vMargin;
	docRect.bottom = docRect.top + docDisplayNode->Height();
	docRect.right = stemPoint.x - borderWidth - hMargin;
	docRect.left = docRect.right - docDisplayNode->MaxLineWidth();
	return docRect;
}


void MenuDirector::StartRefreshCycle()
{
	windowDirector->StartRefreshCycle();
}


void MenuDirector::FinishRefreshCycle()
{
	windowDirector->FinishRefreshCycle();
}


void MenuDirector::RefreshViewRect(Rectangle rect)
{
	windowDirector->RefreshViewRect(rect);
}


void MenuDirector::RefreshDocAfter(int y)
{
	Rectangle rect = DocRect();
	rect.top += y;
	windowDirector->RefreshViewRect(rect);
}


void MenuDirector::RefreshAll()
{
	windowDirector->RefreshAll();
}


void MenuDirector::DoDocAction(DOMString action)
{
	// pass them to the windowDirector
	windowDirector->DoDocAction(action);
}


void MenuDirector::DocTypeChanged()
{
	// never happens
}


DOMString MenuDirector::GetScriptProperty(DOMString property)
{
	if (property == "window-director")
		return windowDirector->AsString();
	else
		return DisplayDirector::GetScriptProperty(property);
}


Rectangle MenuDirector::Bounds()
{
	Rectangle bounds = DocRect();
	bounds.InsetBy(-(hMargin + borderWidth), -(vMargin + borderWidth));
	bounds.top -= stemHeight + cornerRadius;
	return bounds;
}


DOMString MenuDirector::MakeKeyName(string_slice key, int modifiers)
{
	DOMString keyName;

	// modifiers
	if ((modifiers & B_COMMAND_KEY) != 0)
		keyName += "Cmd-";
	if ((modifiers & B_OPTION_KEY) != 0)
		keyName += "Opt-";
	if ((modifiers & B_SHIFT_KEY) != 0)
		keyName += "Shift-";
	if ((modifiers & B_CONTROL_KEY) != 0)
		keyName += "Ctrl-";

	// keyname proper
	if (key == Keys::PageUpKey)
		keyName += "PageUp";
	else if (key == Keys::PageDownKey)
		keyName += "PageDown";
	else if (key == Keys::HomeKey)
		keyName += "Home";
	else if (key == Keys::EndKey)
		keyName += "End";
	else if (key == Keys::UpArrowKey)
		keyName += "UpArrow";
	else if (key == Keys::DownArrowKey)
		keyName += "DownArrow";
	else if (key == Keys::LeftArrowKey)
		keyName += "LeftArrow";
	else if (key == Keys::RightArrowKey)
		keyName += "RightArrow";
	else if (key == Keys::EscKey)
		keyName += "Esc";
	else if (key == Keys::DelKey)
		keyName += "Del";
	else if (key == "\b")
		keyName += "Backspace";
	else if (key.length() == 1) {
		char c = key[0];
		if (c >= 'a' && c <= 'z')
			c -= 'a' - 'A';
		char charStr[4];
		charStr[0] = c;
		charStr[1] = 0;
		keyName += charStr;
		}
	else
		keyName += key;

	return keyName;
}


DOMString MenuDirector::FindKeyActionIn(DOMString keyName, Element* element)
{
	for (Node* node = element->FirstChild(); node; node = node->NextSibling()) {
		Element* child = dynamic_cast<Element*>(node);
		if (child == NULL)
			continue;

		DOMString tagName = child->TagName();
		if (tagName == "menu-item" || tagName == "long-menu-item") {
			DOMString actionKey = child->GetAttribute("key");
			if (actionKey == keyName)
				return child->GetAttribute("action");
			}
		else {
			DOMString action = FindKeyActionIn(keyName, child);
			if (!action.empty())
				return action;
			}
		}

	return DOMString();
}


