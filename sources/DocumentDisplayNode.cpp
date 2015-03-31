/* DocumentDisplayNode.cpp */

#include "DocumentDisplayNode.h"
#include "Document.h"
#include "Element.h"
#include "DisplayDirector.h"
#include "EditStylesheet.h"
#include "Font.h"
#include "InternalException.h"



DocumentDisplayNode::DocumentDisplayNode(Document* documentIn, DisplayDirector* directorIn)
	: CompositeDisplayNode(NULL), document(documentIn), displayDirector(directorIn),
	  font(NULL)
{
	document->AddChangeClient(this);
}


DocumentDisplayNode::~DocumentDisplayNode()
{
	document->RemoveChangeClient(this);
	delete font;
}


DisplayType DocumentDisplayNode::DeclaredDisplayType()
{
	return Block;
}


bool DocumentDisplayNode::IsDisposable()
{
	return false;
}


void DocumentDisplayNode::Load(DisplayDirector* director)
{
	// remove children
	ClearChildren();

	// add the documentElement
	DisplayNode* docElementNode =
		director->DisplayNodeFor(document->DocumentElement());
	AppendChildren(docElementNode, docElementNode);

	LoadChildren(director);
	CheckIfBlock();

	CompositeDisplayNode::Load(director);
}


void DocumentDisplayNode::UpdateProperties()
{
	// this is its own method to get better profiling results

	// ...plus it has to clear the cached font
	delete font;
	font = NULL;

	CompositeDisplayNode::UpdateProperties();
}


DOMString DocumentDisplayNode::GetProperty(string_slice propertyName)
{
	// try "#default"
	EditStylesheet* stylesheet = displayDirector->GetStylesheet();
	if (stylesheet == NULL)
		throw InternalException("DocumentDisplayNode::GetProperty: DisplayDirector has no stylesheet.");
	return stylesheet->PropertyForElement(propertyName, "#default");
}


Font* DocumentDisplayNode::CurFont()
{
	if (font == NULL)
		font = GetFont();
	return font;
}


int DocumentDisplayNode::Width()
{
	return displayDirector->DisplayWidth();
}


int DocumentDisplayNode::LeftSpace()
{
	return 0;
}


void DocumentDisplayNode::BlockLayout(bool force)
{
	// this is its own method to get better profile results
	CompositeDisplayNode::BlockLayout(force);
}


Node* DocumentDisplayNode::FirstNode()
{
	return document;
}


Node* DocumentDisplayNode::LastNode()
{
	return document;
}


bool DocumentDisplayNode::CanSelectAcross()
{
	// not really meaningful, since this is never an inline node
	return false;
}


void DocumentDisplayNode::ChildReplaced(Node* newChild, Node* oldChild)
{
	if (newChild == document->DocumentElement())
		displayDirector->DocTypeChanged();	// this'll take care of everything...
	else
		CompositeDisplayNode::ChildReplaced(newChild, oldChild);
}


DisplayDirector* DocumentDisplayNode::Director()
{
	return displayDirector;
}


