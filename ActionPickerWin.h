// ActionPickerWin.h
// Definition for the ActionPicker class 
// Author:  T A Burgess
// Raised Bar Ltd.
// http://www.raisedbar.net

// #define _SCL_SECURE_NO_WARNINGS

#ifndef ACTION_PICKER_WIN_H
#define ACTION_PICKER_WIN_H

#pragma once


#include "AppConfig.h"
#include "DisplayParamsDlg.h"
#include "MIDIWidget.h"
#include "RBEnums.h"
#include "RBInputBox.h"
#include "RBSpeech.h"
#include "SRConstants.h"
#include "ScreenReaderFunctionsDlg.h"

#include <boost/shared_ptr.hpp>
#include <boost/foreach.hpp>
#include <boost/lexical_cast.hpp>
using boost::lexical_cast;
using boost::bad_lexical_cast;

#include <wx/sizer.h>
#include <wx/statbox.h>
#include <wx/TreeCtrl.h>
#include <wx/button.h>
#include <wx/dialog.h>
#include <wx/msw/winundef.h> 


const int SELECTED_CONTROL_LEVEL = 0;
const int SURFACE_ACTION_TYPE_LEVEL = 1;
const int SURFACE_ACTION_MODE_LEVEL = 2;


class ActionPicker: public wxDialog
	{
	public:
		ActionPicker(const wxString & title, const wxString &ControlName, boost::shared_ptr<AppConfig> pAppConfig, boost::shared_ptr<RBSpeech> pSpeech, boost::shared_ptr<JawsCacheType> pJawsCache);
~ActionPicker();

bool IsValidSurfaceAction();
SurfaceAction GetSurfaceAction();
SurfaceActionType GetDefinedSurfaceActionType();

	DECLARE_EVENT_TABLE()

	private:
void TogglePropertiesButton( wxTreeItemId wxtIDCurrentNode);
void ListSurfaceActionTypes();
void ListScreenReaderActions( SurfaceActionType mySAType, wxTreeItemId wxtIDCurrentNode);
void ListScreenReaderParameters();
void ListJawsActions();
		void ListSAActions();
void ListNVDAActions();
		void ListDolphinActions();

		void GetDisplayParameters();
void GetScreenReaderFunction();
bool GetMessageString();

int GetLevel( wxTreeItemId wxtIDCurrentNode);
int GetChildNumber( wxTreeItemId wxtIDCurrentNode);
	
SurfaceActionType GetSurfaceActionTypeFromNode( wxTreeItemId myCurrentNode);
Mode GetActionMode( wxTreeItemId myCurrentNode);

		// Event handlers
void OnTreeSelect( wxTreeEvent& event);
void OnActionProperties(wxCommandEvent& event);
void OnOK(wxCommandEvent& event);
	void OnCancel(wxCommandEvent& event);


// Internal storage

// A tree of possible actions
wxTreeCtrl * wxtActionPicker;
// ID of the tree's root node
wxTreeItemId wxtIDRootNode; 
// Required for tree navigation:
wxTreeItemIdValue cookie;

// Buttons 
wxButton *btnActionProperties, * btnOK, * btnCancel;

// Storage for the defined action
SurfaceAction myNewSurfaceAction;

ActiveProduct myActiveProduct;
ActiveProduct myOldActiveProduct;
DolphinProduct myDolphinProduct;

boost::shared_ptr<AppConfig> pMyAppConfig;
boost::shared_ptr<RBSpeech> pMySpeech;
boost::shared_ptr<JawsCacheType> pMyJawsCache;
};
#endif  // define guard
