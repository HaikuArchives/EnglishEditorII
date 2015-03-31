/* TagSelection.cpp */

#include "TagSelection.h"
#include "TagEditor.h"
#include "WindowDirector.h"
#include "Keys.h"



TagSelection::TagSelection()
	: tagEditor(NULL)
{
}


TagSelection::~TagSelection()
{
	delete tagEditor;
}


void TagSelection::ShowTag(DisplayDirector* director)
{
	if (tagEditor)
		return;

	WindowDirector* windowDirector = dynamic_cast<WindowDirector*>(director);
	if (windowDirector) {
		tagEditor = new TagEditor(TagName(), this, windowDirector);
		director->RefreshDocRect(Bounds());
		}
}


void TagSelection::HideTag(DisplayDirector* director)
{
	if (tagEditor == NULL)
		return;

	Rectangle biggerBounds = Bounds();	// before the tag goes away

	delete tagEditor;
	tagEditor = NULL;

	director->RefreshDocRect(biggerBounds);
}


void TagSelection::SelectTag(DisplayDirector* director)
{
	ShowTag(director);
	tagEditor->Select();
	director->RefreshDocRect(Bounds());
}


void TagSelection::DeselectTag(DisplayDirector* director)
{
	if (tagEditor) {
		tagEditor->Deselect();
		director->RefreshDocRect(Bounds());
		}
}


bool TagSelection::TagIsSelected()
{
	return (tagEditor && tagEditor->IsSelected());
}


void TagSelection::RestoreTag(DisplayDirector* director)
{
	if (tagEditor) {
		director->StartRefreshCycle();
		director->RefreshDocRect(Bounds());
		tagEditor->SetSelection(NULL);
		tagEditor->SetTagName(TagName());
		director->RefreshDocRect(Bounds());
		director->FinishRefreshCycle();
		}
}


void TagSelection::TagEditDone(DisplayDirector* director)
{
	// default: deselect, but don't hide
	DeselectTag(director);
}


void TagSelection::AcceptKey(string_slice key, DisplayDirector* director)
{
	// handle keys for the tagEditor
	if (tagEditor && tagEditor->IsSelected()) {
		if (key == "\n" || key == "\t") {
			DOMString newTagName = tagEditor->TagName();
			if (newTagName.empty()) {
				RestoreTag(director);
				TagEditDone(director);
				}
			else if (newTagName != TagName()) {
				// in this case, we *don't* call TagEditDone, since the selection
				// (and the tag) will have gone away.
				TagNameChanged(newTagName, director);
				}
			else
				TagEditDone(director);
			}
		else
			tagEditor->KeyDown(key);
		}

	// tag key opens/selects the tagEditor
	else if (key == "\t") {
		SelectTag(director);
		}
}


bool TagSelection::HandleSpecialKey(string_slice key, DisplayDirector* director)
{
	if (key == Keys::EscKey && TagIsSelected()) {
		director->StartRefreshCycle();
		RestoreTag(director);
		TagEditDone(director);
		director->FinishRefreshCycle();
		return true;
		}

	return false;
}


