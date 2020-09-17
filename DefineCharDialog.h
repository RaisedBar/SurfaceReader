// DefineCharDialog.h
// Custom dialog class definition for adding MIDI message Chara labels
// Author:  T A Burgess
// Raised Bar Ltd.
// http://www.raisedbar.net


#ifndef DEFINE_CHAR_DLG_H
#define DEFINE_CHAR_DLG_H

#pragma once


#include "RBEnums.h"
#include "RBInputBox.h"
#include "RBStringFuncs.h"

#include <wx/StatText.h>
#include <wx/textctrl.h>
#include <wx/sizer.h>
#include <wx/msgdlg.h> 
#include <wx/button.h>
#include <wx/msw/winundef.h> 


class DefineCharDialog: public wxDialog
{
public:
DefineCharDialog( const wxString& title);
	DefineCharDialog( const wxString& title, int nIndex, std::wstring wstrLabel);
~DefineCharDialog();

bool IsValidCharDefinition();
std::wstring GetCharLabel();
int GetCharIndex();

private:
	bool IsValidCharLabel();
bool IsValidCharIndex();

// Event handlers:
void OnOK( wxCommandEvent& event);
void OnCancel( wxCommandEvent& event);

wxStaticText * lblLabelPrompt, * lblIndexPrompt;
wxTextCtrl * txtCharLabel, * txtCharIndex;
  
DECLARE_EVENT_TABLE()
};
#endif       // DEFINE_CHAR_DLG_H
