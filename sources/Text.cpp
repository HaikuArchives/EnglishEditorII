/* Text.cpp */

#include "Text.h"
#include "Document.h"
#include "DOMException.h"


Text::Text(String textIn)
	: CharacterData(textIn)
{
}


Text::Text(const Text& other)
	: CharacterData(other.data)
{
}


String Text::NodeName()
{
	return "#text";
}


String Text::NodeValue()
{
	return data;
}


unsigned short Text::NodeType()
{
	return TEXT_NODE;
}


Node* Text::CloneNode(bool deep)
{
	return new Text(*this);
}


Text* Text::SplitText(unsigned long offset)
{
	if (offset >= data.length())
		throw DOMException(INDEX_SIZE_ERR);

	// split the node
	Text* newText = OwnerDocument()->CreateTextNode("");
	newText->SetData(data.substr(offset, 0));
	DeleteData(offset, data.length() - offset);

	// insert the new node
	ParentNode()->InsertBefore(newText, NextSibling());

	return newText;
}


bool Text::IsIgnorableWhitespace()
{
	const char* stopper = data.end();
	for (const char* p = data.begin(); p < stopper; ++p) {
		switch (*p) {
			case ' ':
			case '\t':
			case '\r':
			case '\n':
				break;

			default:
				return false;
			}
		}
	return true;
}


