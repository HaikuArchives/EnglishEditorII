/* BlocksSelection.cpp */

#include "BlocksSelection.h"
#include "BlockableDisplayNode.h"
#include "CompositeAction.h"
#include "RestoreSelectionAction.h"
#include "RemoveNodesAction.h"
#include "NewTextAction.h"
#include "AddCharsAction.h"
#include "MoveNodesAction.h"
#include "SelectBetweenBlocksAction.h"
#include "RestoreBlocksSelectionAction.h"
#include "ElementDisplayNode.h"
#include "Element.h"
#include "Text.h"
#include "DisplayDirector.h"
#include "XMLStringWriter.h"
#include "View.h"
#include "Shape.h"
#include "Keys.h"
#include "InternalException.h"

const int BlocksSelection::lineWidth = 3;
const int BlocksSelection::xOutset = 4;
const int BlocksSelection::yOutset = 0;
const int BlocksSelection::cornerRadius = 8;
const int BlocksSelection::lineSpill = lineWidth - 1;


BlocksSelection::BlocksSelection(BlockableDisplayNode* firstNodeIn, BlockableDisplayNode* lastNodeIn)
	: firstNode(firstNodeIn), lastNode(lastNodeIn)
{
}


void BlocksSelection::Draw(View* view, CoordPoint origin)
{
	// set up drawing
	view->PushState();
	view->SetPenSize(lineWidth);
	view->SetHighColor(Selection::selectionColor);
	view->SetDrawingMode(B_OP_ALPHA);

	// draw
	Rectangle rect = Bounds();
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
}


void BlocksSelection::AcceptKey(string_slice key, DisplayDirector* director)
{
	CompositeAction* action;

	ElementDisplayNode* firstElementNode = dynamic_cast<ElementDisplayNode*>(firstNode);
	if (firstNode == NULL)
		throw InternalException("BlocksSelection::AcceptKey: first node isn't an Element.");
	ElementDisplayNode* lastElementNode = dynamic_cast<ElementDisplayNode*>(lastNode);
	if (lastNode == NULL)
		throw InternalException("BlocksSelection::AcceptKey: last node isn't an Element.");

	if (key == "\t") {
		//*** don't do anything yet: we're not a TagSelection.
		}
	else if (key == "\b" || key == Keys::DelKey) {
		// delete the blocks
		action = new CompositeAction();
		action->AddAction(new RestoreSelectionAction());
		action->AddAction(new RemoveNodesAction(firstElementNode->GetElement(),
		                                        lastElementNode->GetElement()));
		// select
		ElementDisplayNode* parentElementNode =
			dynamic_cast<ElementDisplayNode*>(firstNode->Parent());
		Element* parentElement = (parentElementNode ? parentElementNode->GetElement() : NULL);
		ElementDisplayNode* nextElementNode =
			dynamic_cast<ElementDisplayNode*>(lastNode->NextSibling());
		Element* nextElement = (nextElementNode ? nextElementNode->GetElement() : NULL);
		if (parentElement)
			action->AddAction(new SelectBetweenBlocksAction(parentElement, nextElement));
		director->DoAction(action);
		}
	else if (key == Keys::LeftArrowKey) {
		director->SetSelection(firstNode->StartSelection());
		director->ScrollToSelection();
		}
	else if (key == Keys::RightArrowKey) {
		director->SetSelection(lastNode->EndSelection());
		director->ScrollToSelection();
		}
	else {
		// replace the first block with typed text
		action = new CompositeAction();
		action->AddAction(new RestoreSelectionAction());
		if (firstNode != lastNode) {
			// need to remove all but the first Element
			ElementDisplayNode* secondElementNode =
				dynamic_cast<ElementDisplayNode*>(firstNode->NextSibling());
			if (secondElementNode == NULL)
				throw InternalException("BlocksSelection::AcceptKey: second node isn't an Element.");
			action->AddAction(new RemoveNodesAction(secondElementNode->GetElement(),
			                                        lastElementNode->GetElement()));
			}
		// get rid of contents of the first element
		Element* element = firstElementNode->GetElement();
		action->AddAction(new RemoveNodesAction(element->FirstChild(), element->LastChild()));
		// add a new Text
		NewTextAction* newTextAction = new NewTextAction(element->OwnerDocument());
		action->AddAction(newTextAction);
		Text* newText = newTextAction->GetText();
		action->AddAction(new MoveNodesAction(newText, newText, element, NULL));
		action->AddAction(new AddCharsAction(key, newText, 0));
		director->DoAction(action);
		}
}


void BlocksSelection::Promote(DisplayDirector* director)
{
	// firstNode and lastNode must be siblings, so just select the nearest
	// selectable ancestor
	for (DisplayNode* node = firstNode->Parent(); node; node = node->Parent()) {
		Selection* newSelection = node->GetSelection();
		if (newSelection) {
			director->SetSelection(newSelection);
			return;
			}
		}
}


Selection* BlocksSelection::ExtendTo(CoordPoint point)
{
	DisplayNode* parent = firstNode->Parent();
	if (parent == NULL)
		return this;

	// before current selection
	DisplayNode* node;
	BlockableDisplayNode* block;
	if (point.y < firstNode->Top()) {
		for (node = parent->FirstChild(); ; node = node->NextSibling()) {
			block = dynamic_cast<BlockableDisplayNode*>(node);
			if (block == NULL)
				throw InternalException("BlocksSelection::ExtendTo: some sibling is not a block!");

			// if we've reached "firstNode", we've got the current selection
			if (block == firstNode)
				return this;
			// if the point is after the current DisplayNode, keep looking
			if (point.y > block->Top() + block->Height())
				continue;

			// the new selection starts with this DisplayNode
			return new BlocksSelection(block, lastNode);
			}
		}

	// after current selection
	else if (point.y > lastNode->Top() + lastNode->Height()) {
		for (node = parent->LastChild(); ; node = node->PreviousSibling()) {
			block = dynamic_cast<BlockableDisplayNode*>(node);
			if (block == NULL)
				throw InternalException("BlocksSelection::ExtendTo: some sibling is not a block!");

			// if we've reached "lastNode", we've got the current selection
			if (block == lastNode)
				return this;
			// if the point is before the current DisplayNode, keep looking
			if (point.y < block->Top())
				continue;

			// the new selection ends with this DisplayNode
			return new BlocksSelection(firstNode, block);
			}
		}

	// in current selection
	else
		return this;
}


Action* BlocksSelection::GetRestoreAction()
{
	ElementDisplayNode* firstElementNode =
		dynamic_cast<ElementDisplayNode*>(firstNode);
	if (firstElementNode == NULL)
		throw InternalException("BlocksSelection::GetRestoreAction: firstNode wasn't an ElementDisplayNode.");
	ElementDisplayNode* lastElementNode =
		dynamic_cast<ElementDisplayNode*>(lastNode);
	if (lastElementNode == NULL)
		throw InternalException("BlocksSelection::GetRestoreAction: lastNode wasn't an ElementDisplayNode.");

	return new RestoreBlocksSelectionAction(firstElementNode->GetElement(),
	                                        lastElementNode->GetElement());
}


Rectangle BlocksSelection::Bounds()
{
	Rectangle bounds;
	bounds.top = firstNode->Top();
	bounds.bottom = lastNode->Top() + lastNode->Height();

	bounds.left = firstNode->DisplayLeft();
	bounds.right = bounds.left + firstNode->DisplayWidth();
	if (firstNode != lastNode) {
		for (DisplayNode* node = firstNode->NextSibling(); node; node = node->NextSibling()) {
			BlockableDisplayNode* block = dynamic_cast<BlockableDisplayNode*>(node);
			if (block == NULL)
				throw InternalException("BlocksSelection::Bounds: some node wasn't a block!");

			int left = block->DisplayLeft();
			if (left < bounds.left)
				bounds.left = left;
			int right = left + block->DisplayWidth();
			if (right > bounds.right)
				bounds.right = right;

			if (block == lastNode)
				break;
			}
		}

	bounds.InsetBy(-(xOutset + lineSpill), -(yOutset + lineSpill));
	return bounds;
}


bool BlocksSelection::IsVisible(Rectangle pageRect)
{
	return pageRect.Intersects(Bounds());
}


bool BlocksSelection::NeedsClip()
{
	return true;
}


CoordPoint BlocksSelection::DragStartPoint(CoordPoint destPoint)
{
	Rectangle bounds = Bounds();
	return CoordPoint(bounds.left, (bounds.top + bounds.bottom) / 2);
}


bool BlocksSelection::CanCopy()
{
	return true;
}


DOMString BlocksSelection::GetXMLCopy()
{
	ElementDisplayNode* firstElementNode =
		dynamic_cast<ElementDisplayNode*>(firstNode);
	if (firstElementNode == NULL)
		throw InternalException("BlocksSelection::GetXMLCopy: firstNode wasn't an ElementDisplayNode.");
	ElementDisplayNode* lastElementNode =
		dynamic_cast<ElementDisplayNode*>(lastNode);
	if (lastElementNode == NULL)
		throw InternalException("BlocksSelection::GetXMLCopy: lastNode wasn't an ElementDisplayNode.");

	XMLStringWriter writer(NULL);
	Node* node = firstElementNode->GetElement();
	Node* lastNode = lastElementNode->GetElement();
	for (; node; node = node->NextSibling()) {
		writer.AppendNode(node);
		if (node == lastNode)
			break;
		}
	return writer.GetString();
}


BlockableDisplayNode* BlocksSelection::FirstNode()
{
	return firstNode;
}


BlockableDisplayNode* BlocksSelection::LastNode()
{
	return lastNode;
}


