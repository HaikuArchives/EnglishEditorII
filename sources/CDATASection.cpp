/* CDATASection.cpp */

#include "CDATASection.h"
#include "Document.h"
#include "DOMException.h"


CDATASection::CDATASection(String cdata)
	: Text(cdata)
{
}


CDATASection::CDATASection(const CDATASection& other)
	: Text(other.data)
{
}


String CDATASection::NodeName()
{
	return "#cdata-section";
}


unsigned short CDATASection::NodeType()
{
	return CDATA_SECTION_NODE;
}


Node* CDATASection::CloneNode(bool deep)
{
	return new CDATASection(*this);
}


Text* CDATASection::SplitText(unsigned long offset)
{
	if (offset >= data.length())
		throw DOMException(INDEX_SIZE_ERR);

	// split the node
	CDATASection* newCDATA = OwnerDocument()->CreateCDATASection("");
	newCDATA->SetData(data.substr(offset, 0));
	data.erase(offset, data.length());

	// insert the new node
	ParentNode()->InsertBefore(newCDATA, NextSibling());

	return newCDATA;
}


