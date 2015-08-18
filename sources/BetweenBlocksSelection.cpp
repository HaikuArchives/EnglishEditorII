/* BetweenBlocksSelection.cpp */

#include "BetweenBlocksSelection.h"
#include "BlockableDisplayNode.h"
#include "ElementDisplayNode.h"
#include "GeneratedLeafBlock.h"
#include "Element.h"
#include "Text.h"
#include "CompositeAction.h"
#include "NewElementAction.h"
#include "NewTextAction.h"
#include "MoveNodesAction.h"
#include "AddCharsAction.h"
#include "RestoreSelectionAction.h"
#include "RestoreBetweenBlocksSelectionAction.h"
#include "TagEditor.h"
#include "DisplayDirector.h"
#include "View.h"
#include "Keys.h"
#include <typeinfo>

const int BetweenBlocksSelection::caretWidth = 6;
const int BetweenBlocksSelection::caretHeight = 4;
const int BetweenBlocksSelection::caretLineWidth = 2;
const int BetweenBlocksSelection::tagXOffset = 8;
const int BetweenBlocksSelection::tagYOffset = 8;


BetweenBlocksSelection::BetweenBlocksSelection(BlockableDisplayNode* parentNodeIn, BlockableDisplayNode* beforeNodeIn)
	: parentNode(parentNodeIn), beforeNode(beforeNodeIn)
{
}


void BetweenBlocksSelection::Draw(View* view, BPoint origin)
{
	view->PushState();
	view->SetPenSize(caretLineWidth);
	view->SetHighColor(selectionColor);

	BPoint point = Point() + origin;
	view->StrokeLine(point, BPoint(point.x - caretWidth, point.y - caretHeight));
	view->StrokeLine(point, BPoint(point.x - caretWidth, point.y + caretHeight));

	if (tagEditor) {
		BRect tagBounds = tagEditor->Bounds();
		view->SetPenSize(1);
		view->StrokeLine(point, BPoint(tagBounds.left + origin.x, point.y));
		tagEditor->Draw(view->Bounds());
		}

	view->PopState();
}


void BetweenBlocksSelection::AcceptKey(string_slice key, DisplayDirector* director)
{
	if (TagIsSelected() || key == "\t")
		TagSelection::AcceptKey(key, director);
	else if (key == "\b" || key == Keys::DelKey || key == "\n") {
		// do nothing
		}
	else if (key == Keys::LeftArrowKey) {
		SelectBackward(director);
		}
	else if (key == Keys::RightArrowKey) {
		SelectForward(director);
		}
	else {
		// create a new Element
		InsertText(key, director);
		}
}


BRect BetweenBlocksSelection::Bounds()
{
	BPoint point = Point();
	const int lineSpill = caretLineWidth - 1;
	BRect bounds(point.x - caretWidth - lineSpill, point.y - caretHeight - lineSpill,
	                 point.x + lineSpill, point.y + caretHeight + lineSpill);
	if (tagEditor)
		bounds = bounds | tagEditor->Bounds();
	return bounds;
}


bool BetweenBlocksSelection::IsVisible(BRect pageRect)
{
	return pageRect.Contains(Point());
}


bool BetweenBlocksSelection::NeedsClip()
{
	return false;
}


void BetweenBlocksSelection::Promote(DisplayDirector* director)
{
	director->SetSelection(parentNode->GetSelection());
}


Selection* BetweenBlocksSelection::ExtendTo(BPoint point)
{
	/***/
	return this;
}


Action* BetweenBlocksSelection::GetRestoreAction()
{
	return new RestoreBetweenBlocksSelectionAction(DocParentNode(), DocBeforeNode());
}


bool BetweenBlocksSelection::CanPaste()
{
	return true;
}


void BetweenBlocksSelection::Paste(String pasteText, DisplayDirector* director)
{
	InsertText(pasteText, director);
}


String BetweenBlocksSelection::TagName()
{
	return "";
}


BRect BetweenBlocksSelection::TagSpecRect()
{
	BPoint point = Point();
	int tagX = (int) (point.x + tagXOffset);
	return BRect(tagX, 0, tagX, point.y + tagYOffset);
}


void BetweenBlocksSelection::TagNameChanged(String newTagName, DisplayDirector* director)
{
	CompositeAction* action = new CompositeAction();
	action->AddAction(new RestoreSelectionAction());
	action->AddAction(new NewElementAction(newTagName, DocParentNode(), DocBeforeNode()));
	director->DoAction(action);
}


void BetweenBlocksSelection::TagEditDone(DisplayDirector* director)
{
	HideTag(director);
}


BPoint BetweenBlocksSelection::Point()
{
	BlockableDisplayNode* afterNode = dynamic_cast<BlockableDisplayNode*>(
			beforeNode ? beforeNode->PreviousSibling() : parentNode->LastChild()
		);
	int x, y;
	if (afterNode && beforeNode) {
		// x is min of the two blocks' left edges
		x = afterNode->DisplayLeft();
		int beforeX = beforeNode->DisplayLeft();
		if (beforeX < x)
			x = beforeX;
		// y is between them
		y = (afterNode->Bottom() + beforeNode->Top()) / 2;
		}
	else if (afterNode) {
		x = afterNode->DisplayLeft();
		y = afterNode->Bottom();
		}
	else if (beforeNode) {
		x = beforeNode->DisplayLeft();
		y = beforeNode->Top();
		}
	else {
		// probably never happens; may need to be changed if it does
		x = parentNode->DisplayLeft();
		y = parentNode->Top();
		}
	return BPoint(x, y);
}


Node* BetweenBlocksSelection::DocParentNode()
{
	Node* parent = parentNode->FirstNode();
	if (typeid(*parentNode) == typeid(GeneratedLeafBlock))
		parent = parent->ParentNode();
	return parent;
}


Node* BetweenBlocksSelection::DocBeforeNode()
{
	return (beforeNode ? beforeNode->FirstNode() : NULL);
}


void BetweenBlocksSelection::SelectForward(DisplayDirector* director)
{
	Selection* selection = NULL;

	// find the selection
	DisplayNode* node = (beforeNode ? beforeNode : parentNode->NextNodeInDoc());
	while (node) {
		selection = node->StartSelection();
		if (selection)
			break;
		node = node->NextNodeInDoc();
		}

	if (selection)
		director->SetSelection(selection);
	director->ScrollToSelection();
}


void BetweenBlocksSelection::SelectBackward(DisplayDirector* director)
{
	Selection* selection = NULL;

	// find the selection
	DisplayNode* node = (beforeNode ? beforeNode->PrevNodeInDoc() : parentNode->LastChild());
	while (node) {
		selection = node->EndSelection();
		if (selection)
			break;
		node = node->PrevNodeInDoc();
		}

	if (selection)
		director->SetSelection(selection);
	director->ScrollToSelection();
}


void BetweenBlocksSelection::InsertText(String newContents, DisplayDirector* director)
{
	// figure out what kind of element to insert
	DisplayNode* protoNode = NULL;
	if (beforeNode) {
		DisplayNode* afterNode = beforeNode->PreviousSibling();
		if (afterNode)
			protoNode = afterNode;
		}
	else
		protoNode = parentNode->LastChild();
	ElementDisplayNode* protoElementNode = dynamic_cast<ElementDisplayNode*>(protoNode);

	// other info
	ElementDisplayNode* parentElementNode =
		dynamic_cast<ElementDisplayNode*>(parentNode);
	ElementDisplayNode* beforeElementNode =
		dynamic_cast<ElementDisplayNode*>(beforeNode);

	// do it
	if (protoElementNode && parentElementNode && (beforeElementNode || beforeNode == NULL)) {
		Element* parentElement = parentElementNode->GetElement();
		CompositeAction* action = new CompositeAction();
		action->AddAction(new RestoreSelectionAction());
		NewElementAction* newElementAction =
			new NewElementAction(protoElementNode->GetElement()->TagName(),
			                     parentElement,
			                     beforeElementNode->GetElement());
		action->AddAction(newElementAction);
		NewTextAction* newTextAction = new NewTextAction(parentElement->OwnerDocument());
		action->AddAction(newTextAction);
		Text* newText = newTextAction->GetText();
		action->AddAction(new MoveNodesAction(newText, newText,
		                                      newElementAction->GetNewElement(),
		                                      NULL));
		action->AddAction(new AddCharsAction(newContents, newText, 0));
		director->DoAction(action);
		}
}


