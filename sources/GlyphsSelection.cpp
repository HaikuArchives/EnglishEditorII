/* GlyphsSelection.cpp */

#include "GlyphsSelection.h"
#include "GlyphsHilite.h"
#include "TagEditor.h"
#include "TextDisplayNode.h"
#include "ElementDisplayNode.h"
#include "BlockableDisplayNode.h"
#include "GlyphLoc.h"
#include "Glyph.h"
#include "Text.h"
#include "Element.h"
#include "CompositeAction.h"
#include "AddCharsAction.h"
#include "DeleteCharsAction.h"
#include "NewTextAction.h"
#include "RemoveNodesAction.h"
#include "SplitTextAction.h"
#include "EncloseTextAction.h"
#include "SelectPointAction.h"
#include "RestoreSelectionAction.h"
#include "RestoreGlyphsSelectionAction.h"
#include "DisplayDirector.h"
#include "XMLStringWriter.h"
#include "View.h"
#include "Keys.h"
#include "Error.h"



GlyphsSelection::GlyphsSelection(GlyphRef firstGlyphIn, GlyphRef lastGlyphIn)
	: firstGlyph(firstGlyphIn), lastGlyph(lastGlyphIn)
{
	hilite = new GlyphsHilite(firstGlyph, lastGlyph);
}


GlyphsSelection::~GlyphsSelection()
{
	delete hilite;
}


bool GlyphsSelection::HasEmbeddedElement()
{
	return (firstGlyph.GetTextNode() != lastGlyph.GetTextNode());
}


GlyphRef GlyphsSelection::FirstGlyph()
{
	return firstGlyph;
}


GlyphRef GlyphsSelection::LastGlyph()
{
	return lastGlyph;
}


Selection* GlyphsSelection::SelectionFromTo(GlyphRef firstGlyph, GlyphRef lastGlyph)
{
	// see if it's selecting an entire element
	InlineableDisplayNode* ancestor =
		dynamic_cast<InlineableDisplayNode*>(firstGlyph.CommonAncestorWith(lastGlyph));
	if (ancestor && firstGlyph == ancestor->FirstSelectableGlyph() && lastGlyph == ancestor->LastSelectableGlyph()) {
		ElementDisplayNode* elementNode;
		// if the ancestor is a text node, and the only child of an element, check
		// that element
		TextDisplayNode* textNode = dynamic_cast<TextDisplayNode*>(ancestor);
		if (textNode) {
			elementNode = dynamic_cast<ElementDisplayNode*>(textNode->Parent());
			if (elementNode && elementNode->FirstChild() == textNode && elementNode->LastChild() == textNode)
				ancestor = elementNode;
			}
		// if the ancestor is an element, select it
		elementNode = dynamic_cast<ElementDisplayNode*>(ancestor);
		if (elementNode)
			return elementNode->GetSelection();
		}

	// otherwise, just use a GlyphsSelection
	return new GlyphsSelection(firstGlyph, lastGlyph);
}


void GlyphsSelection::Draw(View* view, BPoint origin)
{
	if (tagEditor) {
		tagEditor->Draw(view->Bounds());
		hilite->Draw(view, origin, tagEditor->Bounds());
		}
	else
		hilite->Draw(view, origin);
}


void GlyphsSelection::AcceptKey(string_slice key, DisplayDirector* director)
{
	int offset;

	if (tagEditor || key == "\t")
		TagSelection::AcceptKey(key, director);
	else if (key == "\n") {
		// not supported yet
		/***/
		}
	else if (key == Keys::LeftArrowKey) {
		offset = firstGlyph.textNode->OffsetOfGlyph(firstGlyph.glyph);
		director->SetSelection(firstGlyph.textNode->GetSelectionAtOffset(offset));
		director->ScrollToSelection();
		}
	else if (key == Keys::RightArrowKey) {
		offset = lastGlyph.textNode->OffsetOfGlyph(lastGlyph.glyph) +
			lastGlyph.glyph->NumCharsInText();
		director->SetSelection(lastGlyph.textNode->GetSelectionAtOffset(offset));
		director->ScrollToSelection();
		}
	else {
		String newText = key;
		if (key == "\b" || key == Keys::DelKey)
			newText = "";
		ReplaceText(newText, director);
#ifdef NOT_ANYMORE
		// set up
		DisplayNode* ancestor = firstGlyph.CommonAncestorWith(lastGlyph);
		if (ancestor == NULL)
			throw InternalException("GlyphsSelection::AcceptKey: No common ancestor.");
		bool hasFirstText = (firstGlyph.textNode->Parent() == ancestor);
		bool hasLastText = (lastGlyph.textNode->Parent() == ancestor);
		bool oneText = (firstGlyph.textNode == lastGlyph.textNode);
		bool hasNodes =
			(!oneText &&
				(!hasFirstText || !hasLastText ||
				 firstGlyph.textNode->NextSibling() != firstGlyph.textNode));
		CompositeAction* action = new CompositeAction();
		action->AddAction(new RestoreGlyphsSelectionAction());
		// clear out anything that's there
		if (oneText) {
			unsigned long startOffset = firstGlyph.StartOffset();
			action->AddAction(new DeleteCharsAction(firstGlyph.textNode->GetText(),
			                                        startOffset,
			                                        lastGlyph.EndOffset() - startOffset));
			}
		else {
			if (hasFirstText) {
				Text* firstText = firstGlyph.textNode->GetText();
				unsigned long offset = firstGlyph.StartOffset();
				action->AddAction(new DeleteCharsAction(firstText, offset,
				                                        firstText->Length() - offset));
				}
			if (hasLastText) {
				action->AddAction(new DeleteCharsAction(lastGlyph.textNode->GetText(),
				                                        0, lastGlyph.EndOffset()));
				}
			if (hasNodes) {
				ElementDisplayNode* ancestorDisplayNode = dynamic_cast<ElementDisplayNode*>(ancestor);
				if (ancestorDisplayNode == NULL)
					throw InternalException("GlyphsSelection::AcceptKey: ancestor isn't an element.");
				Element* ancestorElement = ancestorDisplayNode->GetElement();
				Node* firstDeleteNode = firstGlyph.textNode->GetText();
				while (firstDeleteNode->ParentNode() != ancestorElement)
					firstDeleteNode = firstDeleteNode->ParentNode();
				if (hasFirstText)
					firstDeleteNode = firstDeleteNode->NextSibling();
				Node* lastDeleteNode = lastGlyph.textNode->GetText();
				while (lastDeleteNode->ParentNode() != ancestorElement)
					lastDeleteNode = lastDeleteNode->ParentNode();
				if (hasLastText)
					lastDeleteNode = lastDeleteNode->PreviousSibling();
				action->AddAction(new RemoveNodesAction(firstDeleteNode, lastDeleteNode));
				}
			}
		// add in the key
		if (key == "\b" || key == Keys::DelKey) {
			// just select
			Text* selectText = NULL;
			unsigned long selectOffset = 0;
			if (oneText || hasFirstText) {
				selectText = firstGlyph.textNode->GetText();
				selectOffset = firstGlyph.StartOffset();
				}
			else if (hasLastText) {
				selectText = lastGlyph.textNode->GetText();
				selectOffset = 0;
				}
			else {
				// we don't handle this yet (point selection between two Elements)
				/***/
				}
			if (selectText)
				action->AddAction(new SelectPointAction(selectText, selectOffset));
			}
		else {
			Text* destText;
			unsigned long destOffset;
			if (oneText || hasFirstText) {
				destText = firstGlyph.textNode->GetText();
				destOffset = firstGlyph.StartOffset();
				}
			else if (hasLastText) {
				destText = lastGlyph.textNode->GetText();
				destOffset = 0;
				}
			else {
				NewTextAction* newTextAction =
					new NewTextAction(firstGlyph.textNode->GetText()->OwnerDocument());
				action->AddAction(newTextAction);
				destText = newTextAction->GetText();
				destOffset = 0;
				}
			action->AddAction(new AddCharsAction(key, destText, destOffset));
				// this will do the selection also
			}
		// go!
		director->DoAction(action);
#endif
		}
}


void GlyphsSelection::Promote(DisplayDirector* director)
{
	DisplayNode* node = firstGlyph.CommonAncestorWith(lastGlyph);

	// get out of the selection
	while (node) {
		InlineableDisplayNode* inlineNode = dynamic_cast<InlineableDisplayNode*>(node);
		if (inlineNode == NULL || inlineNode->GetDisplayType() != Inline)
			break;
		if (firstGlyph != inlineNode->FirstSelectableGlyph() ||
				lastGlyph != inlineNode->LastSelectableGlyph())
			break;
		node = node->Parent();
		}

	// get the nearest enclosing selection
	Selection* newSelection = NULL;
	for (; node; node = node->Parent()) {
		newSelection = node->GetSelection();
		if (newSelection)
			break;
		}

	// select
	if (newSelection)
		director->SetSelection(newSelection);
}


Selection* GlyphsSelection::ExtendTo(BPoint point)
{
	if (hilite->ContainsPoint(point))
		return this;

	// convert point to leaf-block coords
	BlockableDisplayNode* leafBlock = firstGlyph.GetTextNode()->EnclosingLeafBlock();
	point.y -= leafBlock->Top();
	point.x -= leafBlock->LeftSpace();

	DisplayNode* rawNode;

	// before current selection
	if (GlyphLoc(firstGlyph) > point) {
		// work backwards until we find it
		GlyphRef newFirstGlyph = firstGlyph;
		for (rawNode = firstGlyph.GetTextNode(); rawNode; rawNode = rawNode->PreviousSibling()) {
			InlineableDisplayNode* node = dynamic_cast<InlineableDisplayNode*>(rawNode);
			if (node == NULL)
				throw InternalException("GlyphsSelection::ExtendTo: Sibling is not inline.");

			// figure out whether to look in this node
			if (!node->CanSelectAcross())
				break;
			GlyphRef nodesFirstGlyph = node->FirstSelectableGlyph();
			if (!nodesFirstGlyph.IsValid())
				continue;
			if (GlyphLoc(node->LastSelectableGlyph()) < point)
				break;
			if (GlyphLoc(nodesFirstGlyph) >= point) {
				newFirstGlyph = nodesFirstGlyph;
				continue;
				}

			// we've now found the node to look into; check if it's a TextDisplayNode
			TextDisplayNode* textNode = dynamic_cast<TextDisplayNode*>(node);
			if (textNode == NULL) {
				// probly an element; select from its beginning
				newFirstGlyph = nodesFirstGlyph;
				break;
				}

			// it *is* a TextDisplayNode; find out which glyph to use
			Glyph* foundGlyph = textNode->GlyphAtPoint(point);
			if (foundGlyph)
				newFirstGlyph.Set(textNode, foundGlyph);
			break;
			}

		// return the new selection
		return SelectionFromTo(newFirstGlyph, lastGlyph);
		}

	// after current selection
	else {
		// work forwards until we find it
		GlyphRef newLastGlyph = lastGlyph;
		for (rawNode = lastGlyph.GetTextNode(); rawNode; rawNode = rawNode->NextSibling()) {
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
			TextDisplayNode* textNode = dynamic_cast<TextDisplayNode*>(node);
			if (textNode == NULL) {
				// probly an element; select to its end
				newLastGlyph = nodesLastGlyph;
				break;
				}

			// it *is* a TextDisplayNode; find out which glyph to use
			Glyph* foundGlyph = textNode->GlyphAtPoint(point);
			if (foundGlyph)
				newLastGlyph.Set(textNode, foundGlyph);
			break;
			}

		// return the new selection
		return SelectionFromTo(firstGlyph, newLastGlyph);
		}

	return this;
}


Action* GlyphsSelection::GetRestoreAction()
{
	return new RestoreGlyphsSelectionAction();
}


BRect GlyphsSelection::Bounds()
{
	return hilite->Bounds();
}


bool GlyphsSelection::IsVisible(BRect pageRect)
{
	return hilite->IsVisible(pageRect);
}


bool GlyphsSelection::NeedsClip()
{
	return true;
}


bool GlyphsSelection::ContainsPoint(BPoint point)
{
	return hilite->ContainsPoint(point);
}


BPoint GlyphsSelection::DragStartPoint(BPoint destPoint)
{
	return hilite->DragStartPoint(destPoint);
}


bool GlyphsSelection::CanCopy()
{
	return true;
}


String GlyphsSelection::GetXMLCopy()
{
	// handle single-text case
	bool singleText = (firstGlyph.textNode == lastGlyph.textNode);
	if (singleText) {
		int startOffset = firstGlyph.StartOffset();
		int endOffset = lastGlyph.EndOffset();
		return firstGlyph.textNode->GetText()->SubstringData(startOffset, endOffset - startOffset);
		}

	// get info about the structure
	DisplayNode* ancestor = firstGlyph.CommonAncestorWith(lastGlyph);
	bool startsWithText = (firstGlyph.textNode->Parent() == ancestor);
	bool endsWithText = (lastGlyph.textNode->Parent() == ancestor);

	// find out where the first and last embedded elements are
	// first
	ElementDisplayNode* firstElementDisplayNode = NULL;
	if (!startsWithText) {
		// it's the element the first glyph is in
		DisplayNode* firstElementNode = firstGlyph.textNode;
		while (firstElementNode) {
			if (firstElementNode->Parent() == ancestor)
				break;
			firstElementNode = firstElementNode->Parent();
			}
		firstElementDisplayNode = dynamic_cast<ElementDisplayNode*>(firstElementNode);
		}
	else {
		firstElementDisplayNode =
			dynamic_cast<ElementDisplayNode*>(firstGlyph.textNode->NextSibling());
		}
	if (firstElementDisplayNode == NULL)
		throw InternalException("GlyphsSelection::GetXMLCopy: Couldn't find firstElementNode.");
	Node* firstElement = firstElementDisplayNode->GetElement();
	// last
	ElementDisplayNode* lastElementDisplayNode = NULL;
	if (!endsWithText) {
		// it's the element the last glyph is in
		DisplayNode* lastElementNode = lastGlyph.textNode;
		while (lastElementNode) {
			if (lastElementNode->Parent() == ancestor)
				break;
			lastElementNode = lastElementNode->Parent();
			}
		lastElementDisplayNode = dynamic_cast<ElementDisplayNode*>(lastElementNode);
		}
	else {
		lastElementDisplayNode =
			dynamic_cast<ElementDisplayNode*>(lastGlyph.textNode->PreviousSibling());
		}
	if (lastElementDisplayNode == NULL)
		throw InternalException("GlyphsSelection::GetXMLCopy: Couldn't find lastElementNode.");
	Node* lastElement = lastElementDisplayNode->GetElement();

	// write the selection
	XMLStringWriter writer(NULL);
	if (startsWithText) {
		Text* startText = firstGlyph.textNode->GetText();
		int startOffset = firstGlyph.StartOffset();
		String startData =
			startText->SubstringData(startOffset, startText->Length() - startOffset);
		writer.AppendString(startData);
		}
	for (Node* node = firstElement; ; node = node->NextSibling()) {
		writer.AppendNode(node);
		if (node == lastElement)
			break;
		}
	if (endsWithText) {
		Text* endText = lastGlyph.textNode->GetText();
		int endOffset = lastGlyph.EndOffset();
		String endData = endText->SubstringData(0, endOffset);
		writer.AppendString(endData);
		}
	return writer.GetString();
}


bool GlyphsSelection::CanPaste()
{
	return true;
}


void GlyphsSelection::Paste(String pasteText, DisplayDirector* director)
{
	ReplaceText(pasteText, director);
}


String GlyphsSelection::TagName()
{
	return "";
}


BRect GlyphsSelection::TagSpecRect()
{
	return hilite->TagSpecRect();
}


void GlyphsSelection::TagNameChanged(String newTagName, DisplayDirector* director)
{
	CompositeAction* action = new CompositeAction();
	action->AddAction(new RestoreSelectionAction());

	if (HasEmbeddedElement()) {
		Error::ShowError("Making a new element from a GlyphsSelection with an embedded element is Coming Soon!");
		delete action;
		return;
		}

	else {
		// split the text up
		// we have to do this in a specific order so the SplitTextAction ctor is
		//	always passed a Text that is currently attached to a Document
		Text* initialText = firstGlyph.textNode->GetText();
		action->AddAction(new SplitTextAction(initialText, lastGlyph.EndOffset()));
		SplitTextAction* frontSplitAction =
			new SplitTextAction(initialText, firstGlyph.StartOffset());
		Text* selectedText = frontSplitAction->GetNewText();
		action->AddAction(frontSplitAction);
/***
		int firstSplitOffset = firstGlyph.StartOffset();
		SplitTextAction* firstSplitAction =
			new SplitTextAction(initialText, firstSplitOffset);
		Text* selectedText = firstSplitAction->GetNewText();
		action->AddAction(firstSplitAction);
		int secondSplitOffset = lastGlyph.EndOffset() - firstSplitOffset;
		action->AddAction(new SplitTextAction(selectedText, secondSplitOffset));
***/

		// enclose the selected text in a new tag
		action->AddAction(new EncloseTextAction(selectedText, newTagName));
		}

	director->DoAction(action);
}


void GlyphsSelection::TagEditDone(DisplayDirector* director)
{
	HideTag(director);
}


void GlyphsSelection::ReplaceText(String newText, DisplayDirector* director)
{
	// set up
	DisplayNode* ancestor = firstGlyph.CommonAncestorWith(lastGlyph);
	if (ancestor == NULL)
		throw InternalException("GlyphsSelection::AcceptKey: No common ancestor.");
	bool hasFirstText = (firstGlyph.textNode->Parent() == ancestor);
	bool hasLastText = (lastGlyph.textNode->Parent() == ancestor);
	bool oneText = (firstGlyph.textNode == lastGlyph.textNode);
	bool hasNodes =
		(!oneText &&
			(!hasFirstText || !hasLastText ||
			 firstGlyph.textNode->NextSibling() != firstGlyph.textNode));
	CompositeAction* action = new CompositeAction();
	action->AddAction(new RestoreGlyphsSelectionAction());

	// clear out anything that's there
	if (oneText) {
		unsigned long startOffset = firstGlyph.StartOffset();
		action->AddAction(new DeleteCharsAction(firstGlyph.textNode->GetText(),
		                                        startOffset,
		                                        lastGlyph.EndOffset() - startOffset));
		}
	else {
		if (hasFirstText) {
			Text* firstText = firstGlyph.textNode->GetText();
			unsigned long offset = firstGlyph.StartOffset();
			action->AddAction(new DeleteCharsAction(firstText, offset,
			                                        firstText->Length() - offset));
			}
		if (hasLastText) {
			action->AddAction(new DeleteCharsAction(lastGlyph.textNode->GetText(),
			                                        0, lastGlyph.EndOffset()));
			}
		if (hasNodes) {
			ElementDisplayNode* ancestorDisplayNode = dynamic_cast<ElementDisplayNode*>(ancestor);
			if (ancestorDisplayNode == NULL)
				throw InternalException("GlyphsSelection::AcceptKey: ancestor isn't an element.");
			Element* ancestorElement = ancestorDisplayNode->GetElement();
			Node* firstDeleteNode = firstGlyph.textNode->GetText();
			while (firstDeleteNode->ParentNode() != ancestorElement)
				firstDeleteNode = firstDeleteNode->ParentNode();
			if (hasFirstText)
				firstDeleteNode = firstDeleteNode->NextSibling();
			Node* lastDeleteNode = lastGlyph.textNode->GetText();
			while (lastDeleteNode->ParentNode() != ancestorElement)
				lastDeleteNode = lastDeleteNode->ParentNode();
			if (hasLastText)
				lastDeleteNode = lastDeleteNode->PreviousSibling();
			action->AddAction(new RemoveNodesAction(firstDeleteNode, lastDeleteNode));
			}
		}

	// if we're replacing it with nothing, just select the point
	if (newText.empty()) {
		// just select
		Text* selectText = NULL;
		unsigned long selectOffset = 0;
		if (oneText || hasFirstText) {
			selectText = firstGlyph.textNode->GetText();
			selectOffset = firstGlyph.StartOffset();
			}
		else if (hasLastText) {
			selectText = lastGlyph.textNode->GetText();
			selectOffset = 0;
			}
		else {
			// we don't handle this yet (point selection between two Elements)
			/***/
			}
		if (selectText)
			action->AddAction(new SelectPointAction(selectText, selectOffset));
		}

	// add in the new text
	else {
		Text* destText;
		unsigned long destOffset;
		if (oneText || hasFirstText) {
			destText = firstGlyph.textNode->GetText();
			destOffset = firstGlyph.StartOffset();
			}
		else if (hasLastText) {
			destText = lastGlyph.textNode->GetText();
			destOffset = 0;
			}
		else {
			NewTextAction* newTextAction =
				new NewTextAction(firstGlyph.textNode->GetText()->OwnerDocument());
			action->AddAction(newTextAction);
			destText = newTextAction->GetText();
			destOffset = 0;
			}
		action->AddAction(new AddCharsAction(newText, destText, destOffset));
			// this will do the selection also
		}

	// do it
	director->DoAction(action);
}


