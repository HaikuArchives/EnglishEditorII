/* BasicLiveText.cpp */

#include "BasicLiveText.h"
#include "DOMChangeClient.h"


BasicLiveText::BasicLiveText(DOMString textIn, bool isIgnorableWhitespace)
	: BasicText(textIn, isIgnorableWhitespace)
{
}


BasicLiveText::BasicLiveText(const BasicLiveText& other)
	: BasicText(other)
{
}


void BasicLiveText::AppendData(DOMString arg)
{
	unsigned long position = Length();
	BasicText::AppendData(arg);
	if (changeClient)
		changeClient->DataInserted(position, arg);
}


void BasicLiveText::InsertData(unsigned long offset, DOMString arg)
{
	BasicText::InsertData(offset, arg);
	if (changeClient)
		changeClient->DataInserted(offset, arg);
}


void BasicLiveText::DeleteData(unsigned long offset, unsigned long count)
{
	BasicText::DeleteData(offset, count);
	if (changeClient)
		changeClient->DataDeleted(offset, count);
}


void BasicLiveText::SetData(DOMString arg)
{
	BasicText::SetData(arg);
	if (changeClient)
		changeClient->DataSet(arg);
}


Text* BasicLiveText::SplitText(unsigned long offset)
{
	return BasicText::SplitText(offset);
}


bool BasicLiveText::IsIgnorableWhitespace()
{
	return BasicText::IsIgnorableWhitespace();
}


Node* BasicLiveText::CloneNode(bool deep)
{
	return new BasicLiveText(*this);
}


