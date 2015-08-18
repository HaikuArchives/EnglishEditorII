/* BlockElementSelection.cpp */

#include "BlockElementSelection.h"
#include "ElementDisplayNode.h"
#include "Element.h"
#include "Text.h"
#include "CompositeAction.h"
#include "RemoveNodesAction.h"
#include "MoveNodesAction.h"
#include "NewTextAction.h"
#include "AddCharsAction.h"
#include "ChangeElementTagNameAction.h"
#include "SelectBetweenBlocksAction.h"
#include "RestoreSelectionAction.h"
#include "RestoreBlockElementSelectionAction.h"
#include "BlocksSelection.h"
#include "TagEditor.h"
#include "XMLStringWriter.h"
#include "Shape.h"
#include "View.h"
#include "Keys.h"
#include "InternalException.h"

const int BlockElementSelection::lineWidth = 3;
const int BlockElementSelection::xOutset = 4;
const int BlockElementSelection::yOutset = 0;
const int BlockElementSelection::cornerRadius = 8;
const int BlockElementSelection::tagXInset = 8;
const int BlockElementSelection::tagYInset = 10;
const int BlockElementSelection::lineSpill = lineWidth - 1;


BlockElementSelection::BlockElementSelection(ElementDisplayNode* displayNodeIn)
	: displayNode(displayNodeIn)
{
}


ElementDisplayNode* BlockElementSelection::GetDisplayNode()
{
	return displayNode;
}


void BlockElementSelection::Draw(View* view, CoordPoint origin)
{
	// create the tagEditor if there isn't one yet
	// we'd do this in the ctor, except stupid C++ can't call derived virtual
	//		functions from the ctor
	if (tagEditor == NULL)
		ShowTag(displayNode->GetDisplayDirector());

	// set up drawing
	view->PushState();
	view->SetPenSize(lineWidth);
	view->SetHighColor(Selection::selectionColor);
	view->SetDrawingMode(B_OP_ALPHA);

	// clip out the tagEditor
	if (tagEditor) {
		Rectangle allRect(0, 0, 32767, 32767);
		Region clipRegion;
		clipRegion.Set(allRect);
		Rectangle tagBounds = tagEditor->Bounds();
		tagBounds.OffsetBy(origin);
		clipRegion.Exclude(tagBounds);
		view->ConstrainClippingRegion(&clipRegion);
		}

	// draw
	Rectangle rect = HiliteBounds();
	rect.InsetBy(lineSpill, lineSpill);
	Shape shape;
	shape.MoveTo(rect.left, rect.top + cornerRadius);
	shape.VerticalCornerTo(rect.left + cornerRadius, rect.top);
	shape.LineTo(rect.right - cornerRadius, rect.top);
	shape.HorizontalCornerTo(rect.right, rect.top + cornerRadius);
	shape.LineTo(rect.right, rect.bottom - cornerRadius);
	shape.VerticalCornerTo(rect.right - cornerRadius, rect.bottom);
	shape.LineTo(rect.left + cornerRadius, rect.bottom);
	shape.HorizontalCornerTo(rect.left, rect.bottom - cornerRadius);
	shape.Close();
	view->MovePenTo(origin);
	view->StrokeShape(&shape);

	// clean up drawing
	view->PopState();

	// draw the tagEditor
	if (tagEditor)
		tagEditor->Draw(view->Bounds());
}


void BlockElementSelection::AcceptKey(string_slice key, DisplayDirector* director)
{
	CompositeAction* action;
	Element* element;

	// handle keys for the tagEditor
	if ((tagEditor && tagEditor->IsSelected()) || key == "\t")
		TagSelection::AcceptKey(key, director);

	// deletion
	else if (key == "\b" || key == Keys::DelKey) {
		// delete the blocks
		action = new CompositeAction();
		action->AddAction(new RestoreSelectionAction());
		element = displayNode->GetElement();
		action->AddAction(new RemoveNodesAction(element, element));
		// select
		ElementDisplayNode* parentElementNode =
			dynamic_cast<ElementDisplayNode*>(displayNode->Parent());
		Element* parentElement = (parentElementNode ? parentElementNode->GetElement() : NULL);
		ElementDisplayNode* nextElementNode =
			dynamic_cast<ElementDisplayNode*>(displayNode->NextSibling());
		Element* nextElement = (nextElementNode ? nextElementNode->GetElement() : NULL);
		if (parentElement)
			action->AddAction(new SelectBetweenBlocksAction(parentElement, nextElement));
		director->DoAction(action);
		}

	else if (key == Keys::LeftArrowKey) {
		director->SetSelection(displayNode->StartSelection());
		director->ScrollToSelection();
		}
	else if (key == Keys::RightArrowKey) {
		director->SetSelection(displayNode->EndSelection());
		director->ScrollToSelection();
		}

	else {
		// replace the contents with typed text
		ReplaceContents(key, director);
		}
}


void BlockElementSelection::Promote(DisplayDirector* director)
{
	// select the nearest selectable ancestor
	for (DisplayNode* node = displayNode->Parent(); node; node = node->Parent()) {
		Selection* newSelection = node->GetSelection();
		if (newSelection) {
			director->SetSelection(newSelection);
			return;
			}
		}
}


Selection* BlockElementSelection::ExtendTo(CoordPoint point)
{
	DisplayNode* parent = displayNode->Parent();
	if (parent == NULL)
		return this;

	// before current selection
	DisplayNode* node;
	BlockableDisplayNode* block;
	if (point.y < displayNode->Top()) {
		for (node = parent->FirstChild(); ; node = node->NextSibling()) {
			block = dynamic_cast<BlockableDisplayNode*>(node);
			if (block == NULL)
				throw InternalException("BlockElementSelection::ExtendTo: some sibling is not a block!");

			// if we've reached "displayNode", we've got the current selection
			if (block == displayNode)
				return this;
			// if the point is after the current DisplayNode, keep looking
			if (point.y > block->Top() + block->Height())
				continue;

			// the new selection starts with this DisplayNode
			return new BlocksSelection(block, displayNode);
			}
		}

	// after current selection
	else if (point.y > displayNode->Top() + displayNode->Height()) {
		for (node = parent->LastChild(); ; node = node->PreviousSibling()) {
			block = dynamic_cast<BlockableDisplayNode*>(node);
			if (block == NULL)
				throw InternalException("BlockElementSelection::ExtendTo: some sibling is not a block!");

			// if we've reached "displayNode", we've got the current selection
			if (block == displayNode)
				return this;
			// if the point is before the current DisplayNode, keep looking
			if (point.y < block->Top())
				continue;

			// the new selection ends with this DisplayNode
			return new BlocksSelection(displayNode, block);
			}
		}

	// in current selection
	else
		return this;
}


Action* BlockElementSelection::GetRestoreAction()
{
	return new RestoreBlockElementSelectionAction(displayNode->GetElement());
}


Rectangle BlockElementSelection::Bounds()
{
	Rectangle bounds = HiliteBounds();
	if (tagEditor)
		bounds = bounds | tagEditor->Bounds();

	return bounds;
}


bool BlockElementSelection::IsVisible(Rectangle pageRect)
{
	return pageRect.Intersects(Bounds());
}


bool BlockElementSelection::NeedsClip()
{
	return true;
}


CoordPoint BlockElementSelection::DragStartPoint(CoordPoint destPoint)
{
	Rectangle bounds = Bounds();
	return CoordPoint(bounds.left, (bounds.top + bounds.bottom) / 2);
}


bool BlockElementSelection::CanCopy()
{
	return true;
}


DOMString BlockElementSelection::GetXMLCopy()
{
	XMLStringWriter writer(displayNode->GetElement());
	writer.Write();
	return writer.GetString();
}


bool BlockElementSelection::CanPaste()
{
	return true;
}


void BlockElementSelection::Paste(String pasteText, DisplayDirector* director)
{
	ReplaceContents(pasteText, director);
}


DOMString BlockElementSelection::TagName()
{
	return displayNode->GetElement()->TagName();
}


Rectangle BlockElementSelection::TagSpecRect()
{
	Rectangle bounds = HiliteBounds();
	return Rectangle(bounds.left + tagXInset, 0, bounds.right - tagXInset,
	                 bounds.top + tagYInset);
}


void BlockElementSelection::TagNameChanged(DOMString newTagName, DisplayDirector* director)
{
	HideTag(director);	// so it won't cause trouble when the selection changes
	Element* element = displayNode->GetElement();
	if (element->TagName() == newTagName || newTagName.empty())
		return;
	ChangeElementTagNameAction* changeTagAction =
		new ChangeElementTagNameAction(element, newTagName);
	director->DoAction(new RestoreSelectionAction(changeTagAction));
}


Rectangle BlockElementSelection::HiliteBounds()
{
	Rectangle bounds;
	bounds.top = displayNode->Top();
	bounds.bottom = bounds.top + displayNode->Height();
	bounds.left = displayNode->DisplayLeft();
	bounds.right = bounds.left + displayNode->DisplayWidth();
	bounds.InsetBy(-(xOutset + lineSpill), -(yOutset + lineSpill));
	return bounds;
}


void BlockElementSelection::ReplaceContents(String newContents, DisplayDirector* director)
{
	CompositeAction* action = new CompositeAction();
	action->AddAction(new RestoreSelectionAction());

	// get rid of the contents of the element
	Element* element = displayNode->GetElement();
	if (element->HasChildNodes())
		action->AddAction(new RemoveNodesAction(element->FirstChild(), element->LastChild()));

	// add a new Text
	NewTextAction* newTextAction = new NewTextAction(element->OwnerDocument());
	action->AddAction(newTextAction);
	Text* newText = newTextAction->GetText();
	action->AddAction(new MoveNodesAction(newText, newText, element, NULL));
	action->AddAction(new AddCharsAction(newContents, newText, 0));

	director->DoAction(action);
}


