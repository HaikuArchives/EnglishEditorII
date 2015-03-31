/* InlineElementHotspot.cpp */

#include "InlineElementHotspot.h"
#include "ElementDisplayNode.h"
#include "GlyphsHilite.h"



InlineElementHotspot::InlineElementHotspot(ElementDisplayNode* displayNodeIn)
	: ElementHotspot(displayNodeIn)
{
	hilite = new GlyphsHilite(displayNode->FirstSelectableGlyph(),
	                          displayNode->LastSelectableGlyph(),
	                          Hotspot::hotspotColor);
}


InlineElementHotspot::~InlineElementHotspot()
{
	delete hilite;
}


Rectangle InlineElementHotspot::Bounds()
{
	return hilite->Bounds();
}


bool InlineElementHotspot::ContainsPoint(CoordPoint point)
{
	return hilite->ContainsPoint(point);
}


void InlineElementHotspot::Draw(View* view, CoordPoint origin)
{
	// fade the rest of the document
	FadeDocument(view, origin);

	hilite->Draw(view, origin);
}


