/* BasicLiveCDATASection.cpp */

#include "BasicLiveCDATASection.h"
#include "DOMChangeClient.h"


BasicLiveCDATASection::BasicLiveCDATASection(DOMString textIn)
	: BasicLiveText(textIn)
{
}


BasicLiveCDATASection::BasicLiveCDATASection(const BasicLiveCDATASection& other)
	: BasicLiveText(other)
{
}


Text* BasicLiveCDATASection::SplitText(unsigned long offset)
{
	return BasicLiveText::SplitText(offset);
}


bool BasicLiveCDATASection::IsIgnorableWhitespace()
{
	return false;
}


Node* BasicLiveCDATASection::CloneNode(bool deep)
{
	return new BasicLiveCDATASection(*this);
}


