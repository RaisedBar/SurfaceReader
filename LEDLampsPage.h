// LEDLampsPage.h
// Author:  T A Burgess
// Raised Bar Ltd.
// http://www.raisedbar.net


#ifndef LEDLampsPage_H
#define LEDLampsPage_H

#pragma once

#include <map>

#include "DefineLEDLampDlg.h"
#include "RBEnums.h"
#include "RBWizardPage.h"
#include "RBInputBox.h"
#include "Protocol.h"
#include "DisplayStatesDialog.h"
#include "DefineMessageDialog.h"

#include <boost/shared_ptr.hpp>

#include <wx/wizard.h>
#include <wx/checkbox.h>
#include <wx/sizer.h>
#include <wx/StatText.h>
#include <wx/TextCtrl.h>
#include <wx/listbox.h>
#include <wx/button.h>
#include <wx/msgdlg.h> 
#include <wx/msw/winundef.h> 


class LEDLampsPage: public wxWizardPageSimple
{
public:
LEDLampsPage( wxWizard * wizParent, const wxString& title, boost::shared_ptr<SurfaceProtocol> pProtocol);
// ~LEDLampsPage();

bool IsValidWizardPage();

virtual bool TransferDataFromWindow()
{
return IsValidWizardPage();
}

private:
void ListDefinedLamps();

// Event handlers:
void OnLEDSelect( wxCommandEvent& event);
void OnAddDefinition( wxCommandEvent& event);
void OnEditDefinition( wxCommandEvent& event);
void OnDeleteDefinition( wxCommandEvent& event);
void OnRenameLamp( wxCommandEvent& event);
void OnDefineLampStates( wxCommandEvent& event);
void OnToggleGlobalStates(wxCommandEvent& event);    
void OnWizardCancel(wxWizardEvent& event);
    
// Internal storage
// List box of defined lamp names
wxListBox * lbxLampNames; 
// Buttons to manipulate the list box contents
wxButton * btnAdd, * btnEdit, * btnDelete, * btnRename, * btnStates;
wxCheckBox * wxchkUseGlobalStates;

// Pointer to the protocol we're modifying
boost::shared_ptr<SurfaceProtocol> pMyProtocol;

DECLARE_EVENT_TABLE()
};

#endif       // LEDLampsPage_H