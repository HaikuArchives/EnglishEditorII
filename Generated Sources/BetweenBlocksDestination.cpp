/* BetweenBlocksDestination.cpp */

#include "BetweenBlocksDestination.h"
#include "ElementDisplayNode.h"
#include "BlocksSelection.h"
#include "BlockElementSelection.h"
#include "Element.h"
#include "CompositeAction.h"
#include "RestoreSelectionAction.h"
#include "MoveNodesAction.h"
#include "CopyNodesAction.h"
#include "SelectBlockElementsAction.h"
#include "View.h"
#include "InternalException.h"

const int BetweenBlocksDestination::arrowheadWidth = 6;
const int BetweenBlocksDestination::arrowheadHeight = 8;
const int BetweenBlocksDestination::arrowLoopWidth = 18;
const int BetweenBlocksDestination::arrowStem = 6;
const int BetweenBlocksDestination::lineSlop = (int) (arrowLineWidth / 2);


BetweenBlocksDestination::BetweenBlocksDestination(Selection* selectionIn, BlockableDisplayNode* parentIn, BlockableDisplayNode* afterNodeIn)
	: Destination(selectionIn), parent(parentIn), afterNode(afterNodeIn)
{
	// calculate the destination point
	toPoint.x = parent->LeftSpace();
	if (afterNode) {
		int prevBottom = afterNode->Bottom();
		BlockableDisplayNode* nextNode =
			dynamic_cast<BlockableDisplayNode*>(afterNode->NextSibling());
		if (nextNode)
			toPoint.y = (prevBottom + nextNode->Top()) / 2;
		else
			toPoint.y = prevBottom;
		}
	else
		toPoint.y = parent->Top();

	fromPoint = selection->DragStartPoint(toPoint);
}


Rectangle BetweenBlocksDestination::ArrowBounds()
{
	Rectangle bounds;

	bounds.left = fromPoint.x - arrowLoopWidth - lineSlop;
	float toLeft = toPoint.x - arrowStem - arrowLoopWidth - lineSlop;
	if (toLeft < bounds.left)
		bounds.left = toLeft;

	bounds.right = fromPoint.x + lineSlop;
	float toRight = toPoint.x + lineSlop;
	if (toRight > bounds.right)
		bounds.right = toRight;

	bounds.top = fromPoint.y - lineSlop;
	float toTop = toPoint.y - arrowheadHeight / 2 - lineSlop;
	if (toTop < bounds.top)
		bounds.top = toTop;

	bounds.bottom = fromPoint.y + lineSlop;
	float toBottom = toPoint.y + arrowheadHeight / 2 + lineSlop;
	if (toBottom > bounds.bottom)
		bounds.bottom = toBottom;

	return bounds;
}


void BetweenBlocksDestination::DrawArrow(View* view, CoordPoint origin)
{
	view->PushState();
	view->SetPenSize(arrowLineWidth);
	view->SetHighColor(arrowColor);

	// line
	const int loopWidth = arrowLoopWidth - arrowStem;
	CoordPoint loopTo(toPoint.x - arrowStem, toPoint.y);
	CoordPoint midPoint((fromPoint.x + loopTo.x) / 2 - loopWidth,
	                    (fromPoint.y + loopTo.y) / 2);
	CoordPoint points[4];
	points[0] = fromPoint + origin;
	points[1] = CoordPoint(fromPoint.x - loopWidth, fromPoint.y) + origin;
	points[2] = points[3] = midPoint + origin;
	view->StrokeBezier(points);
	points[0] = points[1] = midPoint + origin;
	points[2] = CoordPoint(loopTo.x - loopWidth, loopTo.y) + origin;
	points[3] = loopTo + origin;
	view->StrokeBezier(points);
	view->StrokeLine(loopTo + origin, toPoint + origin);

	// arrowhead
	int arrowheadLeft = (int) (toPoint.x - arrowheadWidth);
	view->StrokeLine(toPoint + origin,
	                 CoordPoint(arrowheadLeft, toPoint.y - arrowheadHeight / 2) + origin);
	view->StrokeLine(toPoint + origin,
	                 CoordPoint(arrowheadLeft, toPoint.y + arrowheadHeight / 2) + origin);

	view->PopState();
}


Action* BetweenBlocksDestination::GetMoveAction()
{
	return MakeMoveAction(false);
}


Action* BetweenBlocksDestination::GetCopyAction()
{
	return MakeMoveAction(true);
}


Action* BetweenBlocksDestination::MakeMoveAction(bool copying)
{
	// get the source info
	Element* firstElement = NULL;
	Element* lastElement = NULL;
	BlocksSelection* blocksSource = dynamic_cast<BlocksSelection*>(selection);
	if (blocksSource) {
		ElementDisplayNode* firstElementNode =
			dynamic_cast<ElementDisplayNode*>(blocksSource->FirstNode());
		if (useAssertions && firstElementNode == NULL)
			throw InternalException("BetweenBlocksDestination: selection start is not an Element.");
		firstElement = firstElementNode->GetElement();
		ElementDisplayNode* lastElementNode =
			dynamic_cast<ElementDisplayNode*>(blocksSource->LastNode());
		if (useAssertions && lastElementNode == NULL)
			throw InternalException("BetweenBlocksDestination: selection end is not an Element.");
		lastElement = lastElementNode->GetElement();
		}
	else {
		BlockElementSelection* elementSource = dynamic_cast<BlockElementSelection*>(selection);
		ElementDisplayNode* displayNode =
			dynamic_cast<ElementDisplayNode*>(elementSource->GetDisplayNode());
		if (useAssertions && displayNode == NULL)
			throw InternalException("BetweenBlocksDestination: BlockElementSelection has no ElementDisplayNode.");
		firstElement = lastElement = displayNode->GetElement();
		}
	if (useAssertions && firstElement == NULL)
		throw InternalException("BetweenBlocksDestination: unhandled selection type.");

	// get the dest info
	ElementDisplayNode* parentElementNode = dynamic_cast<ElementDisplayNode*>(parent);
	if (parentElementNode == NULL)
		throw InternalException("BetweenBlocksDestination: parent is not an Element");
	Element* parentElement = parentElementNode->GetElement();
	ElementDisplayNode* afterElementNode = dynamic_cast<ElementDisplayNode*>(afterNode);
	if (afterElementNode == NULL && afterNode != NULL)
		throw InternalException("BetweenBlocksDestination: afterNode is not an Element.");
	Node* beforeNode;
	if (afterElementNode)
		beforeNode = afterElementNode->GetElement()->NextSibling();
	else
		beforeNode = parentElement->FirstChild();

	CompositeAction* action = new CompositeAction();
	action->AddAction(new RestoreSelectionAction());
	Action* moveAction;
	Element* firstSelectionElement;
	Element* lastSelectionElement;
	if (copying) {
		CopyNodesAction* copyAction = new CopyNodesAction(firstElement, lastElement,
		                                                  parentElement, beforeNode);
		moveAction = copyAction;
		firstSelectionElement = dynamic_cast<Element*>(copyAction->FirstNode());
		lastSelectionElement = dynamic_cast<Element*>(copyAction->LastNode());
		}
	else {
		moveAction = new MoveNodesAction(firstElement, lastElement,
		                                 parentElement, beforeNode);
		firstSelectionElement = firstElement;
		lastSelectionElement = lastElement;
		}
	action->AddAction(moveAction);
	action->AddAction(new SelectBlockElementsAction(firstSelectionElement, lastSelectionElement));
	return action;
}


