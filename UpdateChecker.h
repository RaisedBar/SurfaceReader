// UpdateChecker.h
// Definition for the UpdateChecker class 
// Author:  T A Burgess
// Raised Bar Ltd.
// http://www.raisedbar.net

#ifndef UPDATE_CHECKER_H
#define UPDATE_CHECKER_H

#pragma once

#include "SRConstants.h"
#include "RBEnums.h"

#include <wx/sizer.h>
#include <wx/msgdlg.h>
#include <wx/TreeCtrl.h>
#include <wx/button.h>
#include <wx/dialog.h>
#include <wx/stattext.h>
#include <wx/TextCtrl.h>
#include <wx/msw/winundef.h> 

const int APP_LEVEL = 0;
const int UPDATE_TYPE_LEVEL = 1;
const int AVAILABLE_UPDATES_LEVEL = 2;



class UpdateChecker: public wxDialog
	{
	public:
		UpdateChecker(const wxString & title);
~UpdateChecker();


	DECLARE_EVENT_TABLE()

	private:
void ListAppUpdates();
void ListProtocolUpdates();
void ListAppConfigUpdates();
int Level( wxTreeItemId wxtIDCurrentNode);
int ChildNumber( wxTreeItemId wxtIDCurrentNode);

// Event handlers
void OnTreeSelect( wxTreeEvent& event);
void OnUpdate(wxCommandEvent& event);
void OnCancel(wxCommandEvent& event);


// Internal storage

// A tree of possible updates
wxTreeCtrl * wxUpdateChecker;
// ID of the tree's root node
wxTreeItemId wxtIDRootNode; 
// Required for tree navigation:
wxTreeItemIdValue cookie;
wxTreeItemId wxtIDProgramNode, wxtIDProtocolNode, wxtIDAppConfigNode;

wxStaticText * lblUpdateDescriptionPrompt;
wxTextCtrl * txtUpdateDescription;

wxButton * btnUpdate, * btnCancel;
};
#endif
