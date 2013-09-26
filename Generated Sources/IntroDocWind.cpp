/* IntroDocWind.cpp */

#include "IntroDocWind.h"
#include "DocumentSource.h"
#include "WindowDirector.h"
#include "DocumentDisplayNode.h"
#include "Document.h"
#include <Screen.h>


IntroDocWind::IntroDocWind(DocumentSource* docSource)
{
	Init(docSource);
	director->KillControls();

	// resize to the width & height of the document
	int windWidth = (int) Bounds().Width();
	int windHeight = (int) Bounds().Height();
	DocumentDisplayNode* docDisplayNode =
		dynamic_cast<DocumentDisplayNode*>(director->DisplayNodeFor(docSource->GetDocument()));
	if (docDisplayNode) {
		int docWidth = docDisplayNode->MaxLineWidth();
		int docHeight = director->GetDocumentHeight();
		windWidth = docWidth + 2 * director->GetHMargin();
		windHeight = docHeight + 2 * director->GetVMargin();
		ResizeTo(windWidth, windHeight);
		docDisplayNode->BlockLayout(true);
		}

	// center on the screen
	BRect screenRect = BScreen(this).Frame();
	MoveTo((screenRect.Width() - windWidth) / 2,
	       (screenRect.Height() - windHeight) / 2);

	SetLook(B_MODAL_WINDOW_LOOK);
	Show();
}


void IntroDocWind::FrameChanged()
{
	// do nothing--we don't want to save the size/position
}


