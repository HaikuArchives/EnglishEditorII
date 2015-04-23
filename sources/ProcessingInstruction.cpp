/* ProcessingInstruction.cpp */

#include "ProcessingInstruction.h"
#include "DOMException.h"



ProcessingInstruction::ProcessingInstruction(DOMString targetIn, DOMString dataIn)
	: target(targetIn), data(dataIn)
{
}


ProcessingInstruction::ProcessingInstruction(const ProcessingInstruction& other)
	: target(other.target), data(other.data)
{
}


DOMString ProcessingInstruction::NodeName()
{
	return Target();
}


DOMString ProcessingInstruction::NodeValue()
{
	return Data();
}


unsigned short ProcessingInstruction::NodeType()
{
	return PROCESSING_INSTRUCTION_NODE;
}


Node* ProcessingInstruction::InsertBefore(Node* newChild, Node* refChild)
{
	throw DOMException(HIERARCHY_REQUEST_ERR);
}


Node* ProcessingInstruction::ReplaceChild(Node* newChild, Node* oldChild)
{
	throw DOMException(HIERARCHY_REQUEST_ERR);
}


Node* ProcessingInstruction::RemoveChild(Node* oldChild)
{
	throw DOMException(NOT_FOUND_ERR);
}


Node* ProcessingInstruction::AppendChild(Node* newChild)
{
	throw DOMException(HIERARCHY_REQUEST_ERR);
}


Node* ProcessingInstruction::CloneNode(bool deep)
{
	return new ProcessingInstruction(*this);
}


DOMString ProcessingInstruction::Target()
{
	return target;
}


DOMString ProcessingInstruction::Data()
{
	return data;
}


