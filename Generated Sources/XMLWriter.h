/* XMLWriter.h */

#ifndef _H_XMLWriter_
#define _H_XMLWriter_

#include "string_slice.h"

class Document;
class Node;
class Element;
class Attr;
class Text;
class CDATASection;
class EntityReference;
class ProcessingInstruction;
class Comment;

class XMLWriter {
public:
	XMLWriter(Node* rootNodeIn);
	virtual~XMLWriter();
	virtual void	Write();
	virtual void	Write(const string_slice str) = 0;
	void        	WriteNode(Node* node);
	void        	WriteChildren(Node* node);
	void        	WriteElement(Element* element);
	void        	WriteText(Text* text);
	void        	WriteCDATASection(CDATASection* cdata);
	void        	WriteEntityReference(EntityReference* entityRef);
	void        	WriteProcessingInstruction(ProcessingInstruction* pi);
	void        	WriteComment(Comment* comment);

protected:
	Node*	rootNode;
};


#endif
