/* ProcessingInstruction.cpp */

#include "ProcessingInstruction.h"
#include "DOMException.h"



ProcessingInstruction::ProcessingInstruction(String targetIn, String dataIn)
	: target(targetIn), data(dataIn)
{
}


ProcessingInstruction::ProcessingInstruction(const ProcessingInstruction& other)
	: target(other.target), data(other.data)
{
}


String ProcessingInstruction::NodeName()
{
	return Target();
}


String ProcessingInstruction::NodeValue()
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


String ProcessingInstruction::Target()
{
	return target;
}


String ProcessingInstruction::Data()
{
	return data;
}


