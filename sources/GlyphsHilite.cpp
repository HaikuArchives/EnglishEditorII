/* GlyphsHilite.cpp */

#include "GlyphsHilite.h"
#include "TextDisplayNode.h"
#include "Glyph.h"
#include "Selection.h"
#include "BlockableDisplayNode.h"
#include "View.h"
#include "Font.h"
#include "Shape.h"

const int GlyphsHilite::lineWidth = 3;
const int GlyphsHilite::xOutset = 2;
const float GlyphsHilite::yOutset = 0;
const float GlyphsHilite::xCornerLength = 0.4;		// as a fraction of the glyph width
const float GlyphsHilite::yCornerLength = 0.5;		// as a fraction of the glyph height
const int GlyphsHilite::minCornerRadius = 8;
const int GlyphsHilite::tagXInset = 6;
const int GlyphsHilite::tagYInset = 10;
const int GlyphsHilite::lineSpill = lineWidth - 1;


GlyphsHilite::GlyphsHilite(GlyphRef firstGlyphIn, GlyphRef lastGlyphIn, rgb_color colorIn)
	: firstGlyph(firstGlyphIn), lastGlyph(lastGlyphIn), color(colorIn)
{
}


void GlyphsHilite::Draw(View* view, CoordPoint origin, Rectangle tagBounds)
{
	int top, bottom, startX, endX, farLeft, farRight, xRadius, yRadius;

	// set up drawing
	view->PushState();
	view->SetPenSize(lineWidth);
/***
	rgb_color color = Selection::selectionColor;
	color.alpha = 128;
***/
	view->SetDrawingMode(AlphaDrawingMode);
	view->SetHighColor(color);
	bool needsShape = false;

	// clip out the tagBounds
	if (tagBounds.IsValid()) {
		Rectangle allRect(0, 0, 32767, 32767);
		Region clipRegion;
		clipRegion.Set(allRect);
		tagBounds.OffsetBy(origin);
		clipRegion.Exclude(tagBounds);
		view->ConstrainClippingRegion(&clipRegion);
		}

	// common calcs
	// x- and y- radius
	Font* firstFont = firstGlyph.GetTextNode()->CurFont();
	Font* lastFont = lastGlyph.GetTextNode()->CurFont();
	xRadius = (int) (firstGlyph.GetGlyph()->Width(firstFont) * xCornerLength);
	float lastXRadius = lastGlyph.GetGlyph()->Width(lastFont) * yCornerLength;
	if (lastXRadius < xRadius)
		xRadius = (int) lastXRadius;
	if (xRadius < minCornerRadius)
		xRadius = minCornerRadius;
	yRadius = (int) ((firstFont->Ascent() + yOutset) * yCornerLength);
	// startX/endX
	startX = firstGlyph.GetGlyph()->GetX() - xOutset;
	endX = lastGlyph.GetGlyph()->GetX() + lastGlyph.GetGlyph()->Width(lastFont) + xOutset;
	// other
	BlockableDisplayNode* leafBlock = firstGlyph.GetTextNode()->EnclosingLeafBlock();
	origin += CoordPoint(leafBlock->LeftSpace(), leafBlock->Top());
	Shape shape;

	// all on one line
	int firstLine = firstGlyph.GetGlyph()->GetLine();
	int lastLine = lastGlyph.GetGlyph()->GetLine();
	if (firstLine == lastLine) {
		// draw
		top = (int) (firstGlyph.GetGlyph()->GetY() - yOutset);
		bottom = (int) (lastGlyph.GetGlyph()->GetY() + lastFont->Ascent() + lastFont->Descent() + yOutset);
		shape.MoveTo(startX, top + yRadius);
		shape.VerticalCornerTo(startX + xRadius, top);
		shape.LineTo(endX - xRadius, top);
		shape.HorizontalCornerTo(endX, top + yRadius);
		shape.LineTo(endX, bottom - yRadius);
		shape.VerticalCornerTo(endX - xRadius, bottom);
		shape.LineTo(startX + xRadius, bottom);
		shape.HorizontalCornerTo(startX, bottom - yRadius);
		shape.LineTo(startX, top + yRadius);
		shape.Close();
		view->MovePenTo(origin);
		view->StrokeShape(&shape);
		}

	// two lines
	else if (lastLine == firstLine + 1) {
		if (startX >= endX) {
			// discontinuous
			// first line
			top = (int) (firstGlyph.GetGlyph()->GetY() - yOutset);
			bottom = (int) (top + firstFont->Ascent() + firstFont->Descent() + 2 * yOutset);
			float right = leafBlock->Width();
			shape.MoveTo(right, top);
			shape.LineTo(startX + xRadius, top);
			shape.HorizontalCornerTo(startX, top + yRadius);
			shape.LineTo(startX, bottom - yRadius);
			shape.VerticalCornerTo(startX + xRadius, bottom);
			shape.LineTo(right, bottom);
			// second line
			top = (int) (lastGlyph.GetGlyph()->GetY() - yOutset);
			bottom = (int) (top + lastFont->Ascent() + lastFont->Descent() + 2 * yOutset);
			shape.MoveTo(0, top);
			shape.LineTo(endX - xRadius, top);
			shape.HorizontalCornerTo(endX, top + yRadius);
			shape.LineTo(endX, bottom - yRadius);
			shape.VerticalCornerTo(endX - xRadius, bottom);
			shape.LineTo(0, bottom);
			shape.MoveTo(0, bottom);	// makes the shape open-ended (at least on BeOS 4.0)
			// draw
			shape.Close();
			view->MovePenTo(origin);
			view->StrokeShape(&shape);
			}
		else {
			// continuous
			needsShape = true;
			}
		}

	// three or more lines
	else
		needsShape = true;

	// more than two lines, or two lines with overlap
	if (needsShape) {
		// set up
		top = (int) (firstGlyph.GetGlyph()->GetY() + yOutset);
		int upperMid = (int) (top + yOutset + firstFont->Ascent() + firstFont->Descent());
		int lowerMid = (int) (lastGlyph.GetGlyph()->GetY() - yOutset);
		bottom = (int) (lastGlyph.GetGlyph()->GetY() +
			lastFont->Ascent() + lastFont->Descent() + yOutset);
		farLeft = -xOutset;
		farRight = leafBlock->Width() + xOutset;
		// top-left corner
		shape.MoveTo(startX, top + yRadius);
		shape.VerticalCornerTo(startX + xRadius, top);
		// line to top-right
		shape.LineTo(farRight - xRadius, top);
		// top-right corner
		shape.HorizontalCornerTo(farRight, top + yRadius);
		// line to mid-far-right corner
		shape.LineTo(farRight, lowerMid - yRadius);
		// get to mid-mid-right corner
		if (farRight - endX < xRadius * 2) {
			// farRight and endX are too close together for the normal
			// method; draw mid-far-right corner without a connecting line
			shape.VerticalCornerTo((endX + farRight) / 2, lowerMid);
			}
		else {
			// mid-far-right corner
			shape.VerticalCornerTo(farRight - xRadius, lowerMid);
			// line to mid-mid-right corner
			shape.LineTo(endX + xRadius, lowerMid);
			}
		// mid-mid-right corner
		shape.HorizontalCornerTo(endX, lowerMid + yRadius);
		// line to bottom-right corner
		shape.LineTo(endX, bottom - yRadius);
		// bottom-right corner
		shape.VerticalCornerTo(endX - xRadius, bottom);
		// line to bottom-left corner
		shape.LineTo(farLeft + xRadius, bottom);
		// bottom-left corner
		shape.HorizontalCornerTo(farLeft, bottom - yRadius);
		// get to the top-left corner
		if (startX == farLeft) {
			// no wiggling to get there--just draw a line to top-left corner
			shape.LineTo(farLeft, top + yRadius);
			}
		else {
			// line to mid-far-left corner
			shape.LineTo(farLeft, upperMid + yRadius);
			// get to mid-mid-left corner
			if (startX - farLeft < xRadius * 2) {
				// startX is too close to farLeft, so draw the corners without
				// a connecting line
				shape.VerticalCornerTo((startX + farLeft) / 2, upperMid);
				}
			else {
				// mid-far-left corner
				shape.VerticalCornerTo(farLeft + xRadius, upperMid);
				// line to mid-mid-left corner
				shape.LineTo(startX - xRadius, upperMid);
				}
			// mid-mid-left corner
			shape.HorizontalCornerTo(startX, upperMid - yRadius);
			// line back to start at top-left corner
			shape.LineTo(startX, top + yRadius);
			}
		// draw
		shape.Close();
		view->MovePenTo(origin);
		view->StrokeShape(&shape);
		}

	// clean up drawing
	view->PopState();
}


Rectangle GlyphsHilite::Bounds()
{
	BlockableDisplayNode* leafBlock = firstGlyph.GetTextNode()->EnclosingLeafBlock();
	Font* lastFont = lastGlyph.GetTextNode()->CurFont();

	// all on one line
	if (firstGlyph.GetGlyph()->GetLine() == lastGlyph.GetGlyph()->GetLine()) {
		float endX = lastGlyph.GetGlyph()->GetX() + lastGlyph.GetGlyph()->Width(lastFont);
		Rectangle boundsRect(firstGlyph.GetGlyph()->GetX() - lineSpill,
		                     firstGlyph.GetGlyph()->GetY() - lineSpill,
		                     endX + lineSpill,
		                     lastGlyph.GetGlyph()->GetY() + lastFont->Ascent() + lastFont->Descent() + lineSpill);
		boundsRect.InsetBy(-xOutset, -yOutset);
		boundsRect.OffsetBy(leafBlock->LeftSpace(), leafBlock->Top());
		return boundsRect;
		}

	// more than one line
	else {
		Rectangle boundsRect(0, firstGlyph.GetGlyph()->GetY(),
		                    leafBlock->Width(),
		                    lastGlyph.GetGlyph()->GetY() + lastFont->Ascent() + lastFont->Descent());
		boundsRect.InsetBy(-(xOutset + lineSpill), -(yOutset + lineSpill));
		boundsRect.OffsetBy(leafBlock->LeftSpace(), leafBlock->Top());
		return boundsRect;
		}
}


bool GlyphsHilite::IsVisible(Rectangle pageRect)
{
	int yOrigin = firstGlyph.GetTextNode()->EnclosingLeafBlock()->Top();
	Font* firstFont = firstGlyph.GetTextNode()->CurFont();
	int firstY = firstGlyph.GetGlyph()->GetY() + firstFont->Ascent() + yOrigin;
	int lastY = lastGlyph.GetGlyph()->GetY() + yOrigin;
	return (lastY >= pageRect.top && firstY <= pageRect.bottom);
}


bool GlyphsHilite::ContainsPoint(CoordPoint point)
{
	int startX, endX;

	BlockableDisplayNode* leafBlock = firstGlyph.GetTextNode()->EnclosingLeafBlock();
	Font* firstFont = firstGlyph.GetTextNode()->CurFont();
	Font* lastFont = lastGlyph.GetTextNode()->CurFont();
	point.x -= leafBlock->LeftSpace();	// convert to paragraph coords
	point.y -= leafBlock->Top();

	// check if it's at all within range
	if (point.y < firstGlyph.GetGlyph()->GetY())
		return false;
	if (point.y > lastGlyph.GetGlyph()->GetY() + lastFont->Ascent() + lastFont->Descent())
		return false;

	// all on one line
	int firstLine = firstGlyph.GetGlyph()->GetLine();
	int lastLine = lastGlyph.GetGlyph()->GetLine();
	if (firstLine == lastLine) {
		startX = firstGlyph.GetGlyph()->GetX() - xOutset;
		endX = lastGlyph.GetGlyph()->GetX() + lastGlyph.GetGlyph()->Width(lastFont) + xOutset;
		return (point.x >= startX && point.x < endX);
		}

	// check if it's in the first line
	else if (point.y < firstGlyph.GetGlyph()->GetY() + firstFont->LineHeight()) {
		startX = firstGlyph.GetGlyph()->GetX() - xOutset;
		return (point.x >= startX);
		}

	// check if it's in the last line
	else if (point.y > lastGlyph.GetGlyph()->GetY()) {
		endX = lastGlyph.GetGlyph()->GetX() + lastGlyph.GetGlyph()->Width(lastFont) + xOutset;
		return (point.x < endX);
		}

	// otherwise, it's somewhere within the selection
	return true;
}


CoordPoint GlyphsHilite::DragStartPoint(CoordPoint destPoint)
{
	BlockableDisplayNode* leafBlock = firstGlyph.GetTextNode()->EnclosingLeafBlock();
	Font* lastFont = lastGlyph.GetTextNode()->CurFont();
	CoordPoint blockOrigin(leafBlock->LeftSpace(), leafBlock->Top());
	int top = firstGlyph.GetGlyph()->GetY();
	bool oneLine = (firstGlyph.GetGlyph()->GetLine() == lastGlyph.GetGlyph()->GetLine());
	if (destPoint.y < top + blockOrigin.y) {
		// above the selection
		int lastX =
			oneLine ?
			lastGlyph.GetGlyph()->GetX() + lastGlyph.GetGlyph()->Width(lastFont) :
			leafBlock->Width();
		return CoordPoint((firstGlyph.GetGlyph()->GetX() + lastX) / 2, top - yOutset) + blockOrigin;
		}
	else {
		// below the selection (or on the same line)
		int firstX = (oneLine ? firstGlyph.GetGlyph()->GetX() : 0);
		int lastX = lastGlyph.GetGlyph()->GetX() + lastGlyph.GetGlyph()->Width(lastFont);
		return CoordPoint((firstX + lastX) / 2,
		                  lastGlyph.GetGlyph()->GetY() + lastFont->Ascent() + lastFont->Descent())
		       	+ blockOrigin;
		}
}


Rectangle GlyphsHilite::TagSpecRect()
{
	Rectangle specRect;

	int firstLine = firstGlyph.glyph->GetLine();
	int lastLine = lastGlyph.glyph->GetLine();
	if (firstLine == lastLine) {
		Rectangle bounds = Bounds();
		specRect.Set(bounds.left + tagXInset, 0,
		             bounds.right - tagXInset, bounds.top + tagYInset);
		}
	else {
		BlockableDisplayNode* leafBlock = firstGlyph.textNode->EnclosingLeafBlock();
		int left = firstGlyph.glyph->GetX() - xOutset - lineSpill + leafBlock->LeftSpace();
		int top = (int) (firstGlyph.glyph->GetY() - yOutset - lineSpill + leafBlock->Top());
		int right = leafBlock->Width();
		specRect.Set(left + tagXInset, 0, right - tagXInset, top + tagYInset);
		}

	return specRect;
}


