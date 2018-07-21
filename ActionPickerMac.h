// ActionPickerMac.hpp
// Definition for the ActionPicker class 
// Author:  T A Burgess
// Raised Bar Ltd.
// http://www.raisedbar.net

#ifndef ACTION_PICKER_H
#define ACTION_PICKER_H

#pragma once

#include <wx/platform.h>
#ifdef __WXOSX_COCOA__   

#include <boost/foreach.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/make_shared.hpp>
#include <boost/lexical_cast.hpp>
using boost::lexical_cast;
using boost::bad_lexical_cast;

#include "DisplayParamsDlg.h"
#include "RBInputBox.h"
#include "ScreenReaderFunctionsDlg.h"
#include "ActionPickerModels.h"
#include "AppConfig.h"
#include "MIDISurface.h"
#include "Protocol.h"
#include "RBEnums.h"
#include "RBSpeech.h"
#include "RBStringFuncs.h"
#include "SRConstants.h"

#include <wx/button.h>
#include <wx/dataview.h>
#include <wx/dialog.h>
#include <wx/sizer.h>
#include <wx/statbox.h>
#include <wx/msw/winundef.h>


class ActionPicker: public wxDialog
	{
	public:
		ActionPicker(const wxString & title, const std::wstring &wstrControlName, boost::shared_ptr <AppConfig> pAppConfig, boost::shared_ptr <SurfaceProtocol> pProtocol, boost::shared_ptr <RBSpeech> pSpeech);
~ActionPicker();

bool IsValidSurfaceAction();
SurfaceAction GetSurfaceAction();
SurfaceActionType GetDefinedSurfaceActionType();

	DECLARE_EVENT_TABLE()

	private:
void BuildDataView( wxPanel* parent, unsigned long style = 0);
bool GetMessageString();
void TogglePropertiesButton( wxDataViewItem wxtIDCurrentNode);
void ListScreenReaderParameters();
		void ListScreenReaderActions( SurfaceActionType mySAType, wxDataViewItem wxtIDCurrentNode);
		void ListJawsActions();
		void ListSAActions();
	void ListWindowEyesActions();
void ListNVDAActions();
		void ListDolphinActions();

		void GetDisplayParameters();
void GetScreenReaderFunction();
int GetChildNumber( wxDataViewItem wxtIDCurrentNode);
SurfaceActionType GetSurfaceActionTypeFromNode( wxDataViewItem myCurrentNode);

		// Event handlers
void OnDataViewChar(wxKeyEvent& event);
void OnSelectionChanged( wxDataViewEvent &event );

	void OnProperties(wxCommandEvent& event);
void OnOK(wxCommandEvent& event);
	void OnCancel(wxCommandEvent& event);

// Internal storage
		std::wstring  wstrMyControl;

// A tree of possible actions
wxDataViewCtrl * myDataView;
// Models for the data view control
    wxObjectDataPtr<ActionPickerModel> m_actions_model;

// Buttons 
wxButton *btnActionProperties, * btnOK, * btnCancel;

// Storage for the defined action
SurfaceAction myNewSurfaceAction;

boost::shared_ptr <AppConfig> pMyAppConfig;
boost::shared_ptr <SurfaceProtocol> pMyProtocol;

ActiveProduct myActiveProduct;
ActiveProduct myOldActiveProduct;
DolphinProduct myDolphinProduct;
boost::shared_ptr <RBSpeech> pMySpeech;
};
#endif   // if Mac
#endif  // define guard
