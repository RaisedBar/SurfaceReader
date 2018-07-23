// DisplayStatesDialog.h
// Custom dialog class definition for defining MIDI message state labels
// Author:  T A Burgess
// Raised Bar Ltd.
// http://www.raisedbar.net

// #define _SCL_SECURE_NO_WARNINGS

#ifndef CONTROL_STATES_DLG_H
#define CONTROL_STATES_DLG_H

#pragma once

#include <vector>

#include "SRConstants.h"
#include "RBEnums.h"
#include "RBInputBox.h"
#include "DefineStateDialog.h"
#include "MIDI.h"
#include "DisplayDefinition.h"
#include "RBTextCtrl.h"

#include <wx/StatText.h>
#include <wx/listbox.h>
#include <wx/TextCtrl.h>
#include <wx/sizer.h>
#include <wx/msgdlg.h> 
#include <wx/button.h>
#include <wx/msw/winundef.h> 


class DisplayStatesDialog: public wxDialog
{
public:
DisplayStatesDialog( const wxString& title, std::string strHash, DisplayDefinition * pDisplay, std::vector <unsigned char> vSysEx);
~DisplayStatesDialog();

private:
void CheckSelection();
void ListDefinedStates();

// Event handlers:
void OnListSelect(wxCommandEvent& event);
void OnAddState( wxCommandEvent& event);
void OnEditState( wxCommandEvent& event);
void OnDeleteState( wxCommandEvent& event);
void OnOK( wxCommandEvent& event);
void OnCancel( wxCommandEvent& event);

  wxStaticText * lblDefinedStatesPrompt;
wxListBox * lbxStates;
wxButton * AddStateButton, * DeleteStateButton, * EditStateButton, * OKButton, * CancelButton;

// Internal storage
std::string strMyHash;
std::vector <unsigned char> vMySysEx;
std::vector <unsigned char> vBytes;
DisplayDefinition * pMyDisplay;

DECLARE_EVENT_TABLE()
};
#endif       // CONTROL_STATES_DLG_H
