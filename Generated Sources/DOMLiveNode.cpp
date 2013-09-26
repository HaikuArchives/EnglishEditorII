/* DOMLiveNode.cpp */

#include "DOMLiveNode.h"
#include "DOMChangeClient.h"



DOMLiveNode::DOMLiveNode()
	: changeClient(NULL)
{
}


DOMLiveNode::~DOMLiveNode()
{
	if (changeClient)
		changeClient->NodeDying();
}


void DOMLiveNode::AddChangeClient(DOMChangeClient* newChangeClient)
{
	/*** someday support multiple clients through DOMChangeSplitter ***/
	changeClient = newChangeClient;
}


void DOMLiveNode::RemoveChangeClient(DOMChangeClient* oldChangeClient)
{
	/*** someday support multiple clients through DOMChangeSplitter ***/
	if (changeClient == oldChangeClient)
		changeClient = NULL;
}


DOMChangeClient* DOMLiveNode::GetChangeClient()
{
	return changeClient;
}


