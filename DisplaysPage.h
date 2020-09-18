// DisplaysPage.h
// Definition for the DisplaysPage class, part of the AppConfigWizard 
// Author:  T A Burgess
// Raised Bar Ltd.
// http://www.raisedbar.net

#ifndef DISPLAYS_PAGE_H
#define DISPLAYS_PAGE_H

#pragma once

#include <map>

#include "RBEnums.h"
#include "RBInputBox.h"
#include "SRConstants.h"
#include "AppConfig.h"
#include "Protocol.h"
#include "DisplayDefinition.h"
#include "DisplayParamsDlg.h"
#include "RBException.h"

#include <memory>

#include <wx/msgdlg.h>
#include <wx/TreeCtrl.h>
#include <wx/button.h>
#include <wx/dialog.h>
#include <wx/sizer.h>
#include <wx/statbox.h>
#include <wx/wizard.h>
#include <wx/msw/winundef.h> 


const int DISPLAY_LEVEL = 1;
const int STATE_LEVEL = 2;


class DisplaysPage: public wxWizardPageSimple
	{
	public:
		DisplaysPage(wxWizard * wizParent, const wxString & title, std::shared_ptr<AppConfig> pAppConfig, std::shared_ptr<SurfaceProtocol> pProtocol);
~DisplaysPage();

std::map <std::string, DisplayDefinition> GetDisplays();

		DECLARE_EVENT_TABLE()

private:
void ListDisplays( wxTreeItemId wxtIDCurrentNode);
void ListStates( wxTreeItemId wxtIDCurrentNode);
void ToggleButtons();

std::string GetDisplayName( std::wstring wstrName);
int GetLevel( wxTreeItemId wxtIDCurrentNode);
int GetChildNumber( wxTreeItemId wxtIDCurrentNode);
std::string GetDisplayHash( wxTreeItemId myCurrentNode);

// Event handlers
void OnTreeSelect( wxTreeEvent& event);
void OnStates(wxCommandEvent& event);
	void OnRename(wxCommandEvent& event);
void OnWizardCancel(wxWizardEvent& event);

	// Internal storage

// A tree of possible actions
wxTreeCtrl * wxDisplayTree;
// ID of the tree's root node
wxTreeItemId wxtIDRootNode; 
// Required for tree navigation:
wxTreeItemIdValue cookie;

// Buttons to manipulate the tree contents
wxButton * btnRename, *btnStates;
// Active access technology product
DolphinProduct myDolphinProduct;
ActiveProduct myActiveProduct; //apply action to what product?
ActiveProduct myOldActiveProduct;
// Storage for the defined actions
	std::map <std::string, MessageDefinition> myDefinitions;
std::shared_ptr<SurfaceProtocol> pMyProtocol;
std::shared_ptr<AppConfig> pMyAppConfig;
std::map <std::string, DisplayDefinition> myDisplays;
};
#endif