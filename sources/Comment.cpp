/* Comment.cpp */

#include "Comment.h"
#include "DOMException.h"
#include "DOMChangeClient.h"



Comment::Comment(String commentIn)
	: CharacterData(commentIn)
{
}


Comment::Comment(const Comment& other)
	: CharacterData(other.comment)
{
}


String Comment::NodeName()
{
	return "#comment";
}


String Comment::NodeValue()
{
	return comment;
}


unsigned short Comment::NodeType()
{
	return COMMENT_NODE;
}


Node* Comment::InsertBefore(Node* newChild, Node* refChild)
{
	throw DOMException(HIERARCHY_REQUEST_ERR);
}


Node* Comment::ReplaceChild(Node* newChild, Node* oldChild)
{
	throw DOMException(HIERARCHY_REQUEST_ERR);
}


Node* Comment::RemoveChild(Node* oldChild)
{
	throw DOMException(NOT_FOUND_ERR);
}


Node* Comment::AppendChild(Node* newChild)
{
	throw DOMException(HIERARCHY_REQUEST_ERR);
}


Node* Comment::CloneNode(bool deep)
{
	return new Comment(*this);
}


String Comment::Data()
{
	return comment;
}


void Comment::SetData(String arg)
{
	comment = arg;

	if (changeClient)
		changeClient->DataSet(arg);
}


unsigned long Comment::Length()
{
	return comment.length();
}


String Comment::SubstringData(unsigned long offset, unsigned long count)
{
	return comment.substr(offset, count);
}


void Comment::AppendData(String arg)
{
	unsigned long position = Length();

	comment += arg;

	if (changeClient)
		changeClient->DataInserted(position, arg);
}


void Comment::InsertData(unsigned long offset, String arg)
{
	comment.insert(offset, arg);

	if (changeClient)
		changeClient->DataInserted(offset, arg);
}


void Comment::DeleteData(unsigned long offset, unsigned long count)
{
	comment.erase(offset, count);

	if (changeClient)
		changeClient->DataDeleted(offset, count);
}


void Comment::ReplaceData(unsigned long offset, unsigned long count, String arg)
{
	DeleteData(offset, count);
	InsertData(offset, arg);
}


