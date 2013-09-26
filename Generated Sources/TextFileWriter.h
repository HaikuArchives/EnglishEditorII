/* TextFileWriter.h */

#ifndef _H_TextFileWriter_
#define _H_TextFileWriter_

#include "DOMString.h"
#include "string_slice.h"
#include <stdio.h>

class TextFileWriter {
public:
	TextFileWriter(string_slice filePathIn);
	~TextFileWriter();
	void	WriteText(DOMString text);
	void	WriteRawText(DOMString text);
	void	NewLine();
	void	SetLinePrefix(DOMString newPrefix);
	bool	AtLineStart();

protected:
	FILE*                    	file;
	DOMString                	curLine;
	bool                     	whitespaceAtEndOfLine;
	DOMString                	linePrefix;
	static const unsigned int	lineLength;
};


#endif
