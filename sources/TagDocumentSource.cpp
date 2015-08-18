/* TagDocumentSource.cpp */

#include "TagDocumentSource.h"
#include "Document.h"
#include "Element.h"
#include "Text.h"



TagDocumentSource::TagDocumentSource(String tagName)
{
	InitStatics();

	// build the document
	document = new Document();
	Element* tagElement = document->CreateElement("tag-name");
	document->AppendChild(tagElement);
	tagText = document->CreateTextNode(tagName);
	tagElement->AppendChild(tagText);
}


TagDocumentSource::~TagDocumentSource()
{
	delete document;
}


Document* TagDocumentSource::GetDocument()
{
	return document;
}


string_slice TagDocumentSource::GetWindowTitle()
{
	return "#tag-name";
}


void TagDocumentSource::Save()
{
	// nothing to do
}


String TagDocumentSource::TagName()
{
	return tagText->Data();
}


void TagDocumentSource::SetTagName(String tagName)
{
	tagText->SetData(tagName);
}


void TagDocumentSource::InitStatics()
{
}


