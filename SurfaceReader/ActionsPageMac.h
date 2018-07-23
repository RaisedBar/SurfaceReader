 // ActionsPageMac.hpp
// Definition for the Apple Mac version of the ActionsPage class, part of the AppConfigWizard 
// Author:  T A Burgess
// Raised Bar Ltd.
// http://www.raisedbar.net

#ifndef ACTIONS_PAGE_MAC_H
#define ACTIONS_PAGE_MAC_H

#pragma once

#include <wx/platform.h>
#ifdef __WXOSX_COCOA__   

#include <boost/shared_ptr.hpp>

#include "ActionsPageModels.h"
#include "ActionPickerMac.h"
#include "AppConfig.h"
#include "DisplayParamsDlg.h"
#include "MessageDefinition.h"
#include "Protocol.h"
#include "RBEnums.h"
#include "RBInputBox.h"
#include "RBSpeech.h"
#include "RBStringFuncs.h"
#include "ScreenReaderFunctionsDlg.h"
#include "SRConstants.h"
#include "RBException.h"

#include <map>

#include <wx/msgdlg.h>
#include <wx/dataview.h>
#include <wx/button.h>
#include <wx/dialog.h>
#include <wx/sizer.h>
#include <wx/statbox.h>
#include <wx/wizard.h>
#include <wx/msw/winundef.h>


class ActionsPage: public wxWizardPageSimple
	{
	public:
		ActionsPage(wxWizard * wizParent, const wxString & title, boost::shared_ptr <AppConfig> pAppConfig, boost::shared_ptr <SurfaceProtocol> pProtocol, boost::shared_ptr <RBSpeech> pSpeech);
~ActionsPage();

std::map <std::string, MessageDefinition> GetActions();
		std::wstring GetActionParameterDescription( SurfaceAction mySA);

	DECLARE_EVENT_TABLE()

	private:
void BuildDataView( wxPanel* parent, unsigned long style = 0);
bool IsLiveModeNode( wxDataViewItem wxtIDCurrentItem);
bool IsScreenReaderModeNode( wxDataViewItem wxtIDCurrentItem);
SurfaceAction GetSurfaceAction();
SurfaceActionType GetSurfaceActionTypeFromNode( wxDataViewItem wxtIDCurrentItem);
wxString GetActiveProductName( ActiveProduct CurrentActiveProduct);
ActiveProduct GetActiveProduct();
ActiveProduct GetProductFromNode( wxDataViewItem wxtIDCurrentItem);
		ActiveProduct GetProductFromName( std::wstring wstrDescription);
bool IsActiveProductBranch();
bool OtherScreenReadersContainActions( wxDataViewItem wxtIDCurrentNode);

	void ToggleButtons();
void GetDisplayParameters();
void GetScreenReaderFunction();

std::string GetControlName( std::wstring wstrName);
int GetChildNumber( wxDataViewItem wxtIDCurrentItem);
wxDataViewItem GetFirstChild( wxDataViewItem wxtIDCurrentNode);
wxDataViewItem GetPrevSibling( wxDataViewItem wxtIDCurrentNode);
wxDataViewItem GetNextSibling( wxDataViewItem wxtIDCurrentNode);

std::string GetControlHash( wxDataViewItem wxtIDCurrentItem);
		std::wstring GetMode( wxDataViewItem wxtIDCurrentItem);
std::vector <std::wstring> ReplicateActions( wxDataViewItem wxtiCurrentNode, ActiveProduct myProduct);

// Event handlers
    void OnDataViewChar(wxKeyEvent& event);
void OnSelectionChanged( wxDataViewEvent &event );
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

// A structure of possible actions
wxDataViewCtrl * wxDVActions;

// Buttons to manipulate the tree contents
wxButton * btnAdd, * btnRemove, * btnReplicate, * btnUp, * btnDown, * btnRename, *btnActionProperties;

// Active access technology product
DolphinProduct myDolphinProduct;
ActiveProduct myActiveProduct; //apply action to what product?
ActiveProduct myOldActiveProduct;
boost::shared_ptr <AppConfig> pMyAppConfig;
boost::shared_ptr <SurfaceProtocol> pMyProtocol;
std::map <std::string, MessageDefinition> myControls;

wxDataViewCtrl* myDataView;

// Models for the data view control
    wxObjectDataPtr<ActionsTreeModel> m_actions_model;

boost::shared_ptr <RBSpeech> pMySpeech;
};
#endif    //  if Mac
#endif   // define guard
