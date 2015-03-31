/* IntroDocWind.h */

#ifndef _H_IntroDocWind_
#define _H_IntroDocWind_

#include "EnglishEditorWind.h"

class IntroDocWind : public EnglishEditorWind {
public:
	IntroDocWind(DocumentSource* docSource);
	void	FrameChanged();
};


#endif
