/* CharacterData.cpp */

#include "CharacterData.h"
#include "DOMChangeClient.h"
#include "DOMException.h"



CharacterData::CharacterData(DOMString dataIn)
	: data(dataIn)
{
}


DOMString CharacterData::Data()
{
	return data;
}


void CharacterData::SetData(DOMString arg)
{
	data = arg;

	if (changeClient)
		changeClient->DataSet(arg);
}


unsigned long CharacterData::Length()
{
	return data.length();
}


DOMString CharacterData::SubstringData(unsigned long offset, unsigned long count)
{
	return data.substr(offset, count);
}


void CharacterData::AppendData(DOMString arg)
{
	unsigned long position = data.length();

	data += arg;

	if (changeClient)
		changeClient->DataInserted(position, arg);
}


void CharacterData::InsertData(unsigned long offset, DOMString arg)
{
	data.insert(offset, arg);

	if (changeClient)
		changeClient->DataInserted(offset, arg);
}


void CharacterData::DeleteData(unsigned long offset, unsigned long count)
{
	data.erase(offset, count);

	if (changeClient)
		changeClient->DataDeleted(offset, count);
}


void CharacterData::ReplaceData(unsigned long offset, unsigned long count, DOMString arg)
{
	DeleteData(offset, count);
	InsertData(offset, arg);
}


Node* CharacterData::InsertBefore(Node* newChild, Node* refChild)
{
	throw DOMException(HIERARCHY_REQUEST_ERR);
}


Node* CharacterData::ReplaceChild(Node* newChild, Node* oldChild)
{
	throw DOMException(HIERARCHY_REQUEST_ERR);
}


Node* CharacterData::RemoveChild(Node* oldChild)
{
	throw DOMException(NOT_FOUND_ERR);
}


Node* CharacterData::AppendChild(Node* newChild)
{
	throw DOMException(HIERARCHY_REQUEST_ERR);
}


