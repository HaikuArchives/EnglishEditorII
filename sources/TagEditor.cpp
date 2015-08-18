/* TagEditor.cpp */

#include "TagEditor.h"
#include "WindowDirector.h"
#include "TagSelection.h"
#include "TagDocumentSource.h"
#include "Document.h"
#include "Element.h"
#include "Text.h"
#include "Selection.h"
#include "BlockDrawContext.h"
#include "DocumentDisplayNode.h"
#include "TextDisplayNode.h"
#include "CompositeStylesheet.h"
#include "DefaultValidator.h"
#include "BetweenGlyphsSelection.h"
#include "GlyphsSelection.h"
#include "View.h"
#include "Shape.h"
#include "InternalException.h"

const int TagEditor::lineWidth = 3;
const int TagEditor::xOutset = 2;	// was 8
const int TagEditor::yOutset = -1;	// was 2
const int TagEditor::cornerRadius = 8;
const int TagEditor::emptyWidth = 10;
rgb_color TagEditor::bgndColor = { 255, 255, 255, 0 };
int TagEditor::bgndAlpha = 192;
rgb_color TagEditor::selectedColor = Selection::selectionColor;
	// purple: 220, 0, 255
	// darker blue: 0, 108, 192


TagEditor::TagEditor(DOMString tagName, TagSelection* ownerIn, WindowDirector* windowDirectorIn)
	: DisplayDirector(NULL),		// docSource will be set up below
	  windowDirector(windowDirectorIn), owner(ownerIn)
{
	// create docSource
	docSource = new TagDocumentSource(tagName);

	// create the stylesheet
	Element* docElement = docSource->GetDocument()->DocumentElement();
	stylesheet = new CompositeStylesheet(docElement->TagName());
	validator = new DefaultValidator(this);

	SetupDisplayNodes();

/***
	// set the selection
	TextDisplayNode* displayNode =
		dynamic_cast<TextDisplayNode*>(DisplayNodeFor(docElement->FirstChild()));
	if (displayNode == NULL)
		throw InternalException("TagEditor ctor: Can't get TextDisplayNode to select.");
	if (tagName.empty())
		SetSelection(new BetweenGlyphsSelection(displayNode, NULL, NULL, false));
	else {
		SetSelection(new GlyphsSelection(displayNode->FirstSelectableGlyph(), displayNode->LastSelectableGlyph()));
		}
***/
}


TagEditor::~TagEditor()
{
	delete validator;
	delete stylesheet;
	delete docSource;
}


void TagEditor::Select()
{
	Element* docElement = docSource->GetDocument()->DocumentElement();
	TextDisplayNode* displayNode =
		dynamic_cast<TextDisplayNode*>(DisplayNodeFor(docElement->FirstChild()));
	if (displayNode == NULL)
		throw InternalException("TagEditor ctor: Can't get TextDisplayNode to select.");
	if (TagName().empty())
		SetSelection(new BetweenGlyphsSelection(displayNode, NULL, NULL, false));
	else {
		SetSelection(new GlyphsSelection(displayNode->FirstSelectableGlyph(),
		                                 displayNode->LastSelectableGlyph()));
		}
}


void TagEditor::Deselect()
{
	SetSelection(NULL);
}


bool TagEditor::IsSelected()
{
	return (selection != NULL);
}


void TagEditor::SetTagName(DOMString newTagName)
{
	TagDocumentSource* tagSource = dynamic_cast<TagDocumentSource*>(docSource);
	if (useAssertions && tagSource == NULL)
		throw InternalException("TagEditor::RestoreTagName: docSource isn't a TagDocumentSource!");
	tagSource->SetTagName(newTagName);
}


void TagEditor::Draw(Rectangle updateRect)
{
	View* drawView = DrawingView();
	drawView->PushState();

	// make the outline shape
	Rectangle bounds = Bounds();
	Rectangle outlineRect = bounds;
	int lineSpill = (lineWidth + 1) / 2;
	lineSpill -= 1;		//*** fudge factor?
	outlineRect.InsetBy(lineSpill, lineSpill);
	Shape shape;
	shape.MoveTo(outlineRect.left, outlineRect.top + cornerRadius);
	shape.VerticalCornerTo(outlineRect.left + cornerRadius, outlineRect.top);
	shape.LineTo(outlineRect.right - cornerRadius, outlineRect.top);
	shape.HorizontalCornerTo(outlineRect.right, outlineRect.top + cornerRadius);
	shape.LineTo(outlineRect.right, outlineRect.bottom - cornerRadius);
	shape.VerticalCornerTo(outlineRect.right - cornerRadius, outlineRect.bottom);
	shape.LineTo(outlineRect.left + cornerRadius, outlineRect.bottom);
	shape.HorizontalCornerTo(outlineRect.left, outlineRect.bottom - cornerRadius);
	shape.LineTo(outlineRect.left, outlineRect.top + cornerRadius);
	shape.Close();

	// clear and draw the outline
	CoordPoint origin = windowDirector->DocToView(CoordPoint(0, 0));
	rgb_color bgndAlphaColor = bgndColor;
	bgndAlphaColor.alpha = bgndAlpha;
	drawView->MovePenTo(origin);
	drawView->SetDrawingMode(B_OP_ALPHA);
	drawView->SetHighColor(bgndAlphaColor);
	drawView->FillShape(&shape);
	drawView->SetPenSize(lineWidth);
	drawView->SetHighColor(selection ? selectedColor : Selection::selectionColor);
	drawView->StrokeShape(&shape);

	// draw the document
	static const rgb_color blackColor = { 0, 0, 0, 255 };
	drawView->SetHighColor(blackColor);
	drawView->SetDrawingMode(B_OP_COPY);
	int hOrigin = (int) (origin.x + bounds.left + lineWidth + xOutset);
	int vOrigin = (int) (origin.y + bounds.top + lineWidth + yOutset);
	BlockDrawContext
		drawContext(this, drawView, hOrigin, vOrigin,
		            0, docDisplayNode->Height());
	docDisplayNode->BlockDraw(&drawContext);

	// draw selection
	if (selection /*** && selection->IsVisible(pageRect) ***/) {
/***
		bool needsClip = selection->NeedsClip();
		if (needsClip) {
			bitmapView->PushState();
			Rectangle clipRect = DocToView(pageRect);
			clipRect.left = view->Bounds().left;
			clipRect.right = view->Bounds().right;
			Region clipRgn;
			clipRgn.Include(clipRect);
			bitmapView->ConstrainClippingRegion(&clipRgn);
			}
***/

		selection->Draw(drawView, DocToView(CoordPoint(0, 0)));

/***
		if (needsClip)
			bitmapView->PopState();
***/
		}

	drawView->PopState();
}


void TagEditor::KeyDown(string_slice key)
{
	if (selection) {
		StartRefreshCycle();
		selection->AcceptKey(key, this);
		FinishRefreshCycle();
		}
}


void TagEditor::MouseMoved()
{
	// nothing to do
}


void TagEditor::ScreenChanged()
{
	// nothing to do
}


EditStylesheet* TagEditor::GetStylesheet()
{
	return stylesheet;
}


Validator* TagEditor::GetValidator()
{
	return validator;
}


int TagEditor::DisplayWidth()
{
	return windowDirector->DisplayWidth() - 2 * xOutset - 2 * lineWidth;
}


View* TagEditor::DrawingView()
{
	return windowDirector->DrawingView();
}


View* TagEditor::WindowView()
{
	return windowDirector->WindowView();
}


CoordPoint TagEditor::ViewToDoc(CoordPoint viewPoint)
{
	Rectangle bounds = Bounds();
	CoordPoint origin(bounds.left + lineWidth + xOutset,
	                  bounds.top + lineWidth + yOutset);
	return viewPoint - windowDirector->DocToView(origin);
}


CoordPoint TagEditor::DocToView(CoordPoint docPoint)
{
	Rectangle bounds = Bounds();
	CoordPoint origin(bounds.left + lineWidth + xOutset,
	                  bounds.top + lineWidth + yOutset);
	return docPoint + windowDirector->DocToView(origin);
}


Rectangle TagEditor::ViewToDoc(Rectangle rect)
{
	Rectangle bounds = Bounds();
	CoordPoint origin(bounds.left + lineWidth + xOutset,
	                  bounds.top + lineWidth + yOutset);
	origin = windowDirector->DocToView(origin);
	rect.OffsetBy(-origin.x, -origin.y);
	return rect;
}


Rectangle TagEditor::DocToView(Rectangle rect)
{
	Rectangle bounds = Bounds();
	CoordPoint origin(bounds.left + lineWidth + xOutset,
	                  bounds.top + lineWidth + yOutset);
	origin = windowDirector->DocToView(origin);
	rect.OffsetBy(origin);
	return rect;
}


Rectangle TagEditor::DocRect()
{
	Rectangle docRect = Bounds();
	docRect.InsetBy(lineWidth + xOutset, lineWidth + yOutset);
	return windowDirector->DocToView(docRect);
}


void TagEditor::StartRefreshCycle()
{
	windowDirector->StartRefreshCycle();
}


void TagEditor::FinishRefreshCycle()
{
	windowDirector->FinishRefreshCycle();
}


void TagEditor::RefreshViewRect(Rectangle rect)
{
	windowDirector->RefreshViewRect(rect);
}


void TagEditor::RefreshDocAfter(int y)
{
	Rectangle rect = Bounds();
	rect.InsetBy(lineWidth + xOutset, lineWidth + yOutset);
	rect.top += y;
	windowDirector->RefreshViewRect(rect);
}


void TagEditor::RefreshAll()
{
	windowDirector->RefreshAll();
}


void TagEditor::DocTypeChanged()
{
	// never happens
}


Rectangle TagEditor::Bounds()
{
	// first try to get the optimal requested bounds
	Rectangle specRect = owner->TagSpecRect();
	Rectangle bounds;
	bounds.left = specRect.left;
	int width = docDisplayNode->MaxLineWidth();
/***
	if (width < minWidth)
		width = minWidth;
***/
	if (width == 0)
		width = emptyWidth;
	bounds.right = bounds.left + width + 2 * lineWidth + 2 * xOutset;
	bounds.bottom = specRect.bottom;
	bounds.top = bounds.bottom - docDisplayNode->Height() -
	             2 * lineWidth - 2 * yOutset;

	// if it spills over the right edge, slip it back
	int maxRight = windowDirector->DisplayWidth();
	if (bounds.right > maxRight)
		bounds.OffsetBy(maxRight - bounds.right, 0);

	return bounds;
}


DOMString TagEditor::TagName()
{
	Text* tagText =
		dynamic_cast<Text*>(docSource->GetDocument()->DocumentElement()->FirstChild());
	if (useAssertions && tagText == NULL)
		throw InternalException("TagEditor::TagName: Couldn't get tag name.");
	return tagText->Data();
}


