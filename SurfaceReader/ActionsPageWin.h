// ActionsPageWin.h
// Definition for the ActionsPage class, part of the AppConfigWizard 
// Author:  T A Burgess
// Raised Bar Ltd.
// http://www.raisedbar.net

// #define _SCL_SECURE_NO_WARNINGS

#ifndef ACTIONS_PAGE_WIN_H
#define ACTIONS_PAGE_WIN_H

#pragma once

#include <map>
#include <string>

#include "ActionPickerWin.h"
#include "AppConfig.h"
#include "DisplayParamsDlg.h"
#include "MessageDefinition.h"
#include "Protocol.h"
#include "RBEnums.h"
#include "RBInputBox.h"
#include "RBSpeech.h"
#include "SRConstants.h"
#include "ScreenReaderFunctionsDlg.h"
#include "RBException.h"

#include <boost/shared_ptr.hpp>

#include <wx/platform.h>
#include <wx/msgdlg.h>
#include <wx/TreeCtrl.h>
#include <wx/button.h>
#include <wx/dialog.h>
#include <wx/sizer.h>
#include <wx/statbox.h>
#include <wx/wizard.h>
#include <wx/msw/winundef.h> 


const int PROTOCOL_LEVEL = 0;
const int CONTROL_LEVEL = 1;
const int SPEECH_LEVEL = 2;
const int MODE_LEVEL = 3;
const int ACTION_LEVEL = 4;
const int PROPERTIES_LEVEL = 5;


class ActionsPage: public wxWizardPageSimple
	{
	public:
			ActionsPage(wxWizard * wizParent, const wxString & title, boost::shared_ptr<AppConfig> pAppConfig, boost::shared_ptr<SurfaceProtocol> pProtocol, boost::shared_ptr<RBSpeech> pSpeech, boost::shared_ptr<JawsCacheType> pJawsCache);
~ActionsPage();

std::map <std::string, MessageDefinition> GetActions();
std::wstring GetActionParameterDescription( SurfaceAction mySA);

	DECLARE_EVENT_TABLE()

	private:
bool IsLiveModeNode( wxTreeItemId wxtIDCurrentNode);
bool IsScreenReaderModeNode( wxTreeItemId wxtIDCurrentNode);
SurfaceAction GetSurfaceAction();
SurfaceActionType GetSurfaceActionTypeFromNode( wxTreeItemId myCurrentNode);
std::wstring GetActiveProductName( ActiveProduct CurrentActiveProduct);
ActiveProduct GetActiveProduct();
ActiveProduct GetProductFromNode( wxTreeItemId wxtIDCurrentNode);
ActiveProduct GetProductFromName( std::wstring wstrDescription);
bool IsActiveProductBranch();

void ListControls( wxTreeItemId wxtIDCurrentNode);
void ListScreenReaders( wxTreeItemId wxtIDCurrentNode);
void ListModes( wxTreeItemId wxtIDCurrentNode);
void ListActions( wxTreeItemId wxtIDCurrentNode);
void ToggleButtons();
void GetDisplayParameters();
void GetScreenReaderFunction();

std::string GetControlName( std::wstring wstrName);
int GetLevel( wxTreeItemId wxtIDCurrentNode);
int GetChildNumber( wxTreeItemId wxtIDCurrentNode);
std::string GetControlHash( wxTreeItemId myCurrentNode);
std::wstring GetMode( wxTreeItemId myCurrentNode);
bool OtherScreenReadersContainActions( wxTreeItemId wxtIDCurrentNode);
std::vector <std::wstring> ReplicateActions( wxTreeItemId wxtiCurrentNode, ActiveProduct myProduct);

// Event handlers
void OnTreeSelect( wxTreeEvent& event);
void OnAdd(wxCommandEvent& event);
void OnRemove(wxCommandEvent& event);
void OnProperties(wxCommandEvent& event);
	void OnRename(wxCommandEvent& event);
	void OnReplicate(wxCommandEvent& event);
void OnUp(wxCommandEvent& event);
void OnDown(wxCommandEvent& event);
void OnWizardPageChanging(wxWizardEvent& event);
void OnWizardCancel(wxWizardEvent& event);

	// Internal storage

// A tree of possible actions
wxTreeCtrl * wxActionsTree;
// ID of the tree's root node
wxTreeItemId wxtIDRootNode; 
// Required for tree navigation:
wxTreeItemIdValue cookie;

// Buttons to manipulate the tree contents
wxButton * btnAdd, * btnRemove, * btnUp, * btnDown, * btnRename, * btnReplicate, *btnActionProperties;

// Active access technology product
DolphinProduct myDolphinProduct;
ActiveProduct myActiveProduct; //apply action to what product?
ActiveProduct myOldActiveProduct;
boost::shared_ptr<AppConfig> pMyAppConfig;
boost::shared_ptr<SurfaceProtocol> pMyProtocol;
boost::shared_ptr<RBSpeech> pMySpeech;

std::map <std::string, MessageDefinition> myControls;
boost::shared_ptr<JawsCacheType> pMyJawsCache;
};
#endif  // define guard

