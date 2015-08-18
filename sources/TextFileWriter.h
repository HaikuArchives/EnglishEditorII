/* TextFileWriter.h */

#ifndef _H_TextFileWriter_
#define _H_TextFileWriter_

#include "String.h"
#include "string_slice.h"
#include <stdio.h>

class TextFileWriter {
public:
	TextFileWriter(string_slice filePathIn);
	~TextFileWriter();
	void	WriteText(String text);
	void	WriteRawText(String text);
	void	NewLine();
	void	SetLinePrefix(String newPrefix);
	bool	AtLineStart();

protected:
	FILE*                    	file;
	String                	curLine;
	bool                     	whitespaceAtEndOfLine;
	String                	linePrefix;
	static const unsigned int	lineLength;
};


#endif
