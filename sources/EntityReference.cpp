/* EntityReference.cpp */

#include "EntityReference.h"
#include "DOMException.h"



EntityReference::EntityReference(String nameIn)
	: name(nameIn)
{
}


EntityReference::EntityReference(const EntityReference& other)
	: name(other.name)
{
}


String EntityReference::NodeName()
{
	return name;
}


unsigned short EntityReference::NodeType()
{
	return ENTITY_REFERENCE_NODE;
}


Node* EntityReference::CloneNode(bool deep)
{
	return new EntityReference(*this);
}


Node* EntityReference::InsertBefore(Node* newChild, Node* refChild)
{
	// since we don't yet support Entity nodes, we don't allow this
	throw DOMException(NOT_SUPPORTED_ERR);
}


Node* EntityReference::ReplaceChild(Node* newChild, Node* oldChild)
{
	// since we don't yet support Entity nodes, we don't allow this
	throw DOMException(NOT_SUPPORTED_ERR);
}


Node* EntityReference::RemoveChild(Node* oldChild)
{
	// since we don't yet support Entity nodes, we don't allow this
	throw DOMException(NOT_SUPPORTED_ERR);
}


Node* EntityReference::AppendChild(Node* newChild)
{
	// since we don't yet support Entity nodes, we don't allow this
	throw DOMException(NOT_SUPPORTED_ERR);
}


