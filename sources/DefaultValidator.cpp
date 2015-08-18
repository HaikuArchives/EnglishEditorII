/* DefaultValidator.cpp */

#include "DefaultValidator.h"
#include "DisplayDirector.h"
#include "EditStylesheet.h"
#include "Element.h"
#include "ElementDisplayNode.h"
#include "InternalException.h"



DefaultValidator::DefaultValidator(DisplayDirector* directorIn)
	: director(directorIn)
{
}


bool DefaultValidator::AllowsContents(Element* parentNode, NodeTypeList nodeTypes)
{
	// allow blocks within non-leaf-blocks; inlines within inlines and leaf-blocks

	// set up
	ElementDisplayNode* displayNode =
		dynamic_cast<ElementDisplayNode*>(director->DisplayNodeFor(parentNode));
	if (displayNode == NULL)
		throw InternalException("DefaultValidator::AllowsContents: parent has no DisplayNode.");
	bool allowInline = (displayNode->GetDisplayType() == Inline ||
	                    displayNode->IsLeafBlock());
	bool allowBlock = !allowInline;
	EditStylesheet* stylesheet = director->GetStylesheet();

	// check
	for (nodeTypes.Rewind(); nodeTypes.IteratorValid(); nodeTypes.Step()) {
		String curNodeType = nodeTypes.CurNodeType();
		String curDisplayType;
		if (curNodeType == "#text")
			curDisplayType = "inline";
		else
			curDisplayType = stylesheet->PropertyForElement("display", curNodeType);
		if (curDisplayType == "inline") {
			if (!allowInline)
				return false;
			}
		else if (curDisplayType == "block") {
			if (!allowBlock)
				return false;
			}
		else
			return false;
		}

	// all passed; go ahead
	return true;
}


