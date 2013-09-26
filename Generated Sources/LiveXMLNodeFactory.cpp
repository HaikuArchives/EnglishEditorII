/* LiveXMLNodeFactory.cpp */

#include "LiveXMLNodeFactory.h"
#include "BasicLiveDocument.h"
#include "BasicLiveElement.h"
#include "BasicLiveAttr.h"
#include "BasicLiveText.h"
#include "BasicLiveCDATASection.h"


Document* LiveXMLNodeFactory::MakeDocument()
{
	return new BasicLiveDocument(this);
}


Element* LiveXMLNodeFactory::MakeElement(DOMString name)
{
	return new BasicLiveElement(name);
}


Attr* LiveXMLNodeFactory::MakeAttr(DOMString name)
{
	return new BasicLiveAttr(name);
}


Text* LiveXMLNodeFactory::MakeText(DOMString value)
{
	return new BasicLiveText(value);
}


CDATASection* LiveXMLNodeFactory::MakeCDATASection(DOMString value)
{
	return new BasicLiveCDATASection(value);
}


Text* LiveXMLNodeFactory::MakeIgnorableWhitespace(DOMString value)
{
	return new BasicLiveText(value, true);
}


