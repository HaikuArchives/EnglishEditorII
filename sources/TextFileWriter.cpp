/* TextFileWriter.cpp */

#include "TextFileWriter.h"
#include "OSException.h"
#include <errno.h>

const unsigned int TextFileWriter::lineLength = 72;


TextFileWriter::TextFileWriter(string_slice filePathIn)
	: whitespaceAtEndOfLine(false)
{
	// open the file
	qstring filePath(filePathIn);
	file = fopen(filePath.c_str(), "w");
	if (file == NULL)
		throw OSException("Couldn't open file.", errno);
}


TextFileWriter::~TextFileWriter()
{
	if (!curLine.empty())
		fwrite(curLine.begin(), 1, curLine.length(), file);
	fclose(file);
}


void TextFileWriter::WriteText(String text)
{
	char c;

	// write the text, wrapping lines
	const char* p = text.begin();
	const char* stopper = text.end();
	while (true) {
		// skip whitespace
		while (p < stopper) {
			c = *p;
			if (c != ' ' && c != '\t' && c != '\n' && c != '\r')
				break;
			whitespaceAtEndOfLine = true;
			++p;
			}
		if (p >= stopper)
			break;

		// get the next character run
		const char* charsStart = p;
		while (p < stopper) {
			c = *p;
			if (c == ' ' || c == '\t' || c == '\n' || c == '\r')
				break;
			p++;
			}
		string_slice chars(charsStart, p);

		// check if we need a newline or a space
		if (!AtLineStart()) {
			unsigned int neededChars = chars.length() + (whitespaceAtEndOfLine ? 1 : 0);
			if (curLine.length() + neededChars > lineLength)
				NewLine();
			else if (whitespaceAtEndOfLine)
				curLine += " ";
			}

		// write the character run
		curLine += chars;
		whitespaceAtEndOfLine = false;
		}
}


void TextFileWriter::WriteRawText(String text)
{
	// finish any pending text
	if (whitespaceAtEndOfLine) {
		curLine += " ";
		whitespaceAtEndOfLine = false;
		}
	if (!curLine.empty()) {
		fwrite(curLine.begin(), 1, curLine.length(), file);
		curLine = linePrefix;
		}

	// write the new text
	fwrite(text.begin(), 1, text.length(), file);
}


void TextFileWriter::NewLine()
{
	if (!curLine.empty())
		fwrite(curLine.begin(), curLine.length(), 1, file);
	fputs("\n", file);
	curLine = linePrefix;
	whitespaceAtEndOfLine = false;
}


void TextFileWriter::SetLinePrefix(String newPrefix)
{
	bool lineWasEmpty = (curLine == linePrefix || curLine.empty());
	linePrefix = newPrefix;
	if (lineWasEmpty)
		curLine = linePrefix;
}


bool TextFileWriter::AtLineStart()
{
	return (curLine == linePrefix);
}


