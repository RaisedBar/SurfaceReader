// DefineStateDialog.h
// Custom dialog class definition for adding MIDI message state labels
// Author:  T A Burgess
// Raised Bar Ltd.
// http://www.raisedbar.net

#ifndef DEFINE_STATE_DLG_H
#define DEFINE_STATE_DLG_H

#pragma once

#include <vector>

#include "MIDI.h"
#include "RBTextCtrl.h"
#include "RBStringFuncs.h"
#include "RBEnums.h"
#include "RBInputBox.h"

#include <boost/lexical_cast.hpp>
using boost::lexical_cast;
using boost::bad_lexical_cast;

#include <wx/StatText.h>
#include <wx/listbox.h>
#include <wx/sizer.h>
#include <wx/msgdlg.h> 
#include <wx/button.h>
#include <wx/msw/winundef.h> 


class DefineStateDialog: public wxDialog
{
public:
DefineStateDialog( const wxString& title, std::string strHash, std::vector <unsigned char> vSysExHeader);
DefineStateDialog( const wxString& title, std::string strHash, std::vector <unsigned char> vSysExHeader, wxString wxstrLabel, std::vector <unsigned char> vKeyBytes);
~DefineStateDialog();

bool IsValidStateDefinition();
std::wstring GetStateName();
std::vector <unsigned char> GetStateValue();

private:
	bool IsValidStateLabel();
bool IsValidStateValue();
bool IsValidDataByte( long lValue);

// Event handlers:
void OnAddDataByte( wxCommandEvent& event);
void OnRemoveDataByte( wxCommandEvent& event);
void OnOK( wxCommandEvent& event);
void OnCancel( wxCommandEvent& event);

wxStaticText * lblStateLabelPrompt, * lblSysExBytesPrompt, * lblMSBPrompt, * lblLSBPrompt;
wxTextCtrl * txtStateLabel, * txtMSBByte, * txtLSBByte, * txtSysExByte;
RBTextCtrl * rbtxtSysExBytes;

  // Internal storage
std::string strMyHash;
std::vector <unsigned char> vMySysExHeader;
std::vector <unsigned char> vBytes;

DECLARE_EVENT_TABLE()
};
#endif       // DEFINE_STATE_DLG_H
