/* DisplayNode.h */

#ifndef _H_DisplayNode_
#define _H_DisplayNode_

#include "Constants.h"
#include "String.h"
#include "string_slice.h"

class Font;
class DisplayDirector;
class BlockableDisplayNode;
class Selection;
class Hotspot;

class DisplayNode {
public:
	DisplayNode(DisplayNode* parentIn = NULL);
	virtual~DisplayNode();
	virtual DisplayType          	GetDisplayType() = 0;
	virtual DisplayType          	DeclaredDisplayType() = 0;
	virtual bool                 	IsLeafBlock() = 0;
	virtual bool                 	IsDisposable() = 0;
	virtual void                 	Load(DisplayDirector* director);
	virtual void                 	UpdateProperties() = 0;
	virtual String            	GetProperty(string_slice propertyName);
	String                    	GetInheritedProperty(string_slice propertyName);
	virtual int                  	GetPropertyInPixels(string_slice propertyName, bool inherited);
	virtual BlockableDisplayNode*	AsBlockableDisplayNode() = 0;
	virtual Selection*           	StartSelection() = 0;
	virtual Selection*           	EndSelection() = 0;
	virtual DisplayNode*         	FirstChild() = 0;
	virtual DisplayNode*         	LastChild() = 0;
	virtual Selection*           	GetSelection();
	virtual Hotspot*             	GetHotspot(DisplayDirector* director);
	virtual Font*                	CurFont() = 0;
	DisplayNode*                 	Parent();
	DisplayNode*                 	PreviousSibling();
	DisplayNode*                 	NextSibling();
	bool                         	DescendsFrom(DisplayNode* otherNode);
	DisplayNode*                 	PrevNodeInDoc();
	DisplayNode*                 	NextNodeInDoc();
	DisplayDirector*             	GetDisplayDirector();
	Font*                        	GetFont();
	bool                         	NeedsLoad();
	bool                         	NeedsLayout();
	void                         	SetPrevSibling(DisplayNode* newPrevSibling);
	void                         	SetNextSibling(DisplayNode* newNextSibling);
	void                         	SetParent(DisplayNode* newParent);

protected:
	DisplayNode*	parent;
	DisplayNode*	prevSibling;
	DisplayNode*	nextSibling;
	bool        	needsLoad;
	bool        	needsLayout;
};


#endif
