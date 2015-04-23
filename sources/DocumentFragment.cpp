/* DocumentFragment.cpp */

#include "DocumentFragment.h"


DocumentFragment::DocumentFragment()
{
}


DocumentFragment::DocumentFragment(const DocumentFragment& other, bool deep)
{
	if (deep)
		CloneChildrenFrom(&other);
}


DOMString DocumentFragment::NodeName()
{
	return "#document-fragment";
}


DOMString DocumentFragment::NodeValue()
{
	return "";
}


unsigned short DocumentFragment::NodeType()
{
	return DOCUMENT_FRAGMENT_NODE;
}


Node* DocumentFragment::CloneNode(bool deep)
{
	return new DocumentFragment(*this);
}


