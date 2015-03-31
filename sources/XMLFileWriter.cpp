/* XMLFileWriter.cpp */

#include "XMLFileWriter.h"
#include "Document.h"



XMLFileWriter::XMLFileWriter(Document* documentIn, FILE* fileIn)
	: XMLWriter(documentIn), file(fileIn)
{
}


void XMLFileWriter::Write(const string_slice str)
{
	fwrite(str.begin(), 1, str.length(), file);
}


void XMLFileWriter::Write()
{
	rewind(file);
	XMLWriter::Write();
}


