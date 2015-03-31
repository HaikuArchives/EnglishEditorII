/* DocumentSource.cpp */

#include "DocumentSource.h"
#include "XMLFileSource.h"
#include "System.h"
#include "String.h"


DocumentSource* DocumentSource::MakeMenuDocSource()
{
	String filePath = System::AppDirectory();
	filePath += "Aux-Docs/window-menu";
	return new XMLFileSource(filePath);
}


void DocumentSource::AttachedToDirector(DisplayDirector* director)
{
	// default: do nothing
}


bool DocumentSource::CanCloseOnSpacebar()
{
	// default: no
	return false;
}


