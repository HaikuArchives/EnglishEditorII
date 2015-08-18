/* XMLStringWriter.cpp */

#include "XMLStringWriter.h"



XMLStringWriter::XMLStringWriter(Node* rootNodeIn)
	: XMLWriter(rootNodeIn)
{
}


String XMLStringWriter::GetString()
{
	return string;
}


void XMLStringWriter::AppendNode(Node* extraNode)
{
	WriteNode(extraNode);
}


void XMLStringWriter::AppendString(string_slice str)
{
	Write(str);
}


void XMLStringWriter::Write(const string_slice str)
{
	string += str;
}


void XMLStringWriter::Write()
{
	string = "";
	XMLWriter::Write();
}


