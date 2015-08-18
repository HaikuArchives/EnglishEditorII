/* InlineElementSelection.cpp */

#include "InlineElementSelection.h"
#include "ElementDisplayNode.h"
#include "TextDisplayNode.h"
#include "GlyphsHilite.h"
#include "TagEditor.h"
#include "Element.h"
#include "Text.h"
#include "RestoreInlineElementSelectionAction.h"
#include "CompositeAction.h"
#include "RemoveNodesAction.h"
#include "NewTextAction.h"
#include "AddCharsAction.h"
#include "MoveNodesAction.h"
#include "ChangeElementTagNameAction.h"
#include "RestoreSelectionAction.h"
#include "DisplayDirector.h"
#include "WindowDirector.h"
#include "GlyphLoc.h"
#include "GlyphsSelection.h"
#include "XMLStringWriter.h"
#include "View.h"
#include "Keys.h"
#include "InternalException.h"



InlineElementSelection::InlineElementSelection(ElementDisplayNode* displayNodeIn)
	: displayNode(displayNodeIn)
{
	hilite = new GlyphsHilite(displayNode->FirstSelectableGlyph(),
	                          displayNode->LastSelectableGlyph());
}


InlineElementSelection::~InlineElementSelection()
{
	delete hilite;
}


Element* InlineElementSelection::GetElement()
{
	return displayNode->GetElement();
}


ElementDisplayNode* InlineElementSelection::GetDisplayNode()
{
	return displayNode;
}


void InlineElementSelection::Draw(View* view, BPoint origin)
{
	// create the tagEditor if there isn't one yet
	// we'd do this in the ctor, except stupid C++ can't call derived virtual
	//		functions from the ctor
	if (tagEditor == NULL)
		ShowTag(displayNode->GetDisplayDirector());

	if (tagEditor) {
		tagEditor->Draw(view->Bounds());
		hilite->Draw(view, origin, tagEditor->Bounds());
		}
	else
		hilite->Draw(view, origin);
}


void InlineElementSelection::AcceptKey(string_slice key, DisplayDirector* director)
{
	CompositeAction* action;
	Element* element;

	// handle keys for the tagEditor
	if ((tagEditor && tagEditor->IsSelected()) || key == "\t")
		TagSelection::AcceptKey(key, director);

	else if (key == "\n") {
		/***/
		}
	else if (key == "\b" || key == Keys::DelKey) {
		// delete the whole element
		element = displayNode->GetElement();
		action = new CompositeAction();
		action->AddAction(new RestoreSelectionAction());
		action->AddAction(new RemoveNodesAction(element, element));
		//*** need to select, but don't really have a selection for it yet...
		//*** kinda want a deleter, like GlyphsMover, to handle surrounding
		//*** space
		director->DoAction(action);
		}
	else if (key == Keys::RightArrowKey) {
		director->SetSelection(displayNode->EndSelection());
		director->ScrollToSelection();
		}
	else if (key == Keys::LeftArrowKey) {
		director->SetSelection(displayNode->StartSelection());
		director->ScrollToSelection();
		}
	else {
		// replace the contents of the element
		ReplaceContents(key, director);
		}
}


void InlineElementSelection::Promote(DisplayDirector* director)
{
	// get the nearest enclosing selection
	Selection* newSelection = NULL;
	for (DisplayNode* node = displayNode->Parent(); node; node = node->Parent()) {
		newSelection = node->GetSelection();
		if (newSelection)
			break;
		}

	// select
	if (newSelection)
		director->SetSelection(newSelection);
}


Selection* InlineElementSelection::ExtendTo(BPoint point)
{
	if (hilite->ContainsPoint(point))
		return this;

	// convert point to leaf-block coords
	BlockableDisplayNode* leafBlock = displayNode->EnclosingLeafBlock();
	point.y -= leafBlock->Top();
	point.x -= leafBlock->LeftSpace();

	DisplayNode* rawNode;
	TextDisplayNode* textNode;
	Glyph* foundGlyph;
	GlyphRef firstGlyph = displayNode->FirstSelectableGlyph();
	GlyphRef lastGlyph = displayNode->LastSelectableGlyph();
	GlyphRef newFirstGlyph = firstGlyph;
	GlyphRef newLastGlyph = lastGlyph;

	// before current selection
	if (GlyphLoc(firstGlyph) > point) {
		// work backwards until we find it
		for (rawNode = displayNode; rawNode; rawNode = rawNode->PreviousSibling()) {
			InlineableDisplayNode* node = dynamic_cast<InlineableDisplayNode*>(rawNode);
			if (node == NULL)
				throw InternalException("GlyphsSelection::ExtendTo: Sibling is not inline.");

			// figure out whether to look in this node
			if (!node->CanSelectAcross())
				break;
			GlyphRef nodesFirstGlyph = node->FirstSelectableGlyph();
			if (!nodesFirstGlyph.IsValid())
				continue;	// it doesn't show up on the display
			if (GlyphLoc(node->LastSelectableGlyph()) < point)
				break;
			if (GlyphLoc(nodesFirstGlyph) >= point) {
				newFirstGlyph = nodesFirstGlyph;
				continue;
				}

			// we've now found the node to look into; check if it's a TextDisplayNode
			textNode = dynamic_cast<TextDisplayNode*>(node);
			if (textNode == NULL) {
				// probly an element; select from its beginning
				newFirstGlyph = nodesFirstGlyph;
				break;
				}

			// it *is* a TextDisplayNode; find out which glyph to use
			foundGlyph = textNode->GlyphAtPoint(point);
			if (foundGlyph)
				newFirstGlyph.Set(textNode, foundGlyph);
			break;
			}
		}

	// after current selection
	else {
		// work forwards until we find it
		for (rawNode = displayNode; rawNode; rawNode = rawNode->NextSibling()) {
			InlineableDisplayNode* node = dynamic_cast<InlineableDisplayNode*>(rawNode);
			if (node == NULL)
				throw InternalException("GlyphsSelection::ExtendTo: Sibling is not inline.");

			// figure out whether to look in this node
			if (!node->CanSelectAcross())
				break;
			GlyphRef nodesLastGlyph = node->LastSelectableGlyph();
			if (!nodesLastGlyph.IsValid())
				continue;
			if (GlyphLoc(node->FirstSelectableGlyph()) > point)
				break;
			if (GlyphLoc(nodesLastGlyph) <= point) {
				newLastGlyph = nodesLastGlyph;
				continue;
				}

			// we've now found the node to look into; check if it's a TextDisplayNode
			textNode = dynamic_cast<TextDisplayNode*>(node);
			if (textNode == NULL) {
				// probly an element; select to its end
				newLastGlyph = nodesLastGlyph;
				break;
				}

			// it *is* a TextDisplayNode; find out which glyph to use
			foundGlyph = textNode->GlyphAtPoint(point);
			if (foundGlyph)
				newLastGlyph.Set(textNode, foundGlyph);
			break;
			}
		}

	// return the new selection
	return GlyphsSelection::SelectionFromTo(newFirstGlyph, newLastGlyph);
}


Action* InlineElementSelection::GetRestoreAction()
{
	return new RestoreInlineElementSelectionAction();
}


BRect InlineElementSelection::Bounds()
{
	return hilite->Bounds();
}


bool InlineElementSelection::IsVisible(BRect pageRect)
{
	return hilite->IsVisible(pageRect);
}


bool InlineElementSelection::NeedsClip()
{
	return true;
}


BPoint InlineElementSelection::DragStartPoint(BPoint destPoint)
{
	return hilite->DragStartPoint(destPoint);
}


bool InlineElementSelection::CanCopy()
{
	return true;
}


String InlineElementSelection::GetXMLCopy()
{
	XMLStringWriter writer(displayNode->GetElement());
	writer.Write();
	return writer.GetString();
}


bool InlineElementSelection::CanPaste()
{
	return true;
}


void InlineElementSelection::Paste(String pasteText, DisplayDirector* director)
{
	ReplaceContents(pasteText, director);
}


String InlineElementSelection::TagName()
{
	return displayNode->GetElement()->TagName();
}


BRect InlineElementSelection::TagSpecRect()
{
	return hilite->TagSpecRect();
}


void InlineElementSelection::TagNameChanged(String newTagName, DisplayDirector* director)
{
	HideTag(director);	// this is going away; prevent surprises in TagEditDone()
	Element* element = displayNode->GetElement();
	if (element->TagName() == newTagName || newTagName.empty())
		return;
	ChangeElementTagNameAction* changeTagAction =
		new ChangeElementTagNameAction(element, newTagName);
	director->DoAction(new RestoreSelectionAction(changeTagAction));
}


void InlineElementSelection::ReplaceContents(String newContents, DisplayDirector* director)
{
	CompositeAction* action = new CompositeAction();
	action->AddAction(new RestoreSelectionAction());

	// get rid of current contents
	Element* element = displayNode->GetElement();
	if (element->HasChildNodes())
		action->AddAction(new RemoveNodesAction(element->FirstChild(), element->LastChild()));

	// add the new contents
	NewTextAction* newTextAction = new NewTextAction(element->OwnerDocument());
	action->AddAction(newTextAction);
	Text* newText = newTextAction->GetText();
	action->AddAction(new MoveNodesAction(newText, newText, element, NULL));
	action->AddAction(new AddCharsAction(newContents, newText, 0));

	director->DoAction(action);
}


