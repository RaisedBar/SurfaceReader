// ControlStatesDialog.h
// Custom dialog class definition for defining MIDI message state labels
// Author:  T A Burgess
// Raised Bar Ltd.
// http://www.raisedbar.net


#ifndef CONTROL_STATES_DLG_H
#define CONTROL_STATES_DLG_H

#pragma once

#include <vector>

#include "DefineStateDialog.h"
#include "MessageDefinition.h"
#include "MIDI.h"
#include "RBEnums.h"
#include "RBInputBox.h"
#include "SRConstants.h"
#include "RBTextCtrl.h"

#include <wx/StatText.h>
#include <wx/listbox.h>
#include <wx/TextCtrl.h>
#include <wx/sizer.h>
#include <wx/msgdlg.h> 
#include <wx/button.h>
#include <wx/msw/winundef.h> 


class ControlStatesDialog: public wxDialog
{
public:
ControlStatesDialog( const wxString& title, std::wstring strHash, MessageDefinition * pMessage, std::vector <unsigned char> vSysExHeader);
~ControlStatesDialog();

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
std::wstring strMyHash;
std::vector <unsigned char> vBytes;
MessageDefinition * pMyMessage;
std::vector <unsigned char> vMySysExHeader;

DECLARE_EVENT_TABLE()
};
#endif       // CONTROL_STATES_DLG_H
