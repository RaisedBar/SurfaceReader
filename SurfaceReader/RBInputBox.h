// RBInputBox.h
// Simple prompt/text box dialog
// Author:  T A Burgess
// Raised Bar Ltd.
// http://www.raisedbar.net


#ifndef RBINPUTBOX_H
#define RBINPUTBOX_H

#pragma once

#include "RBEnums.h"
#include "SRConstants.h"

#include <wx/dialog.h>
#include <wx/stattext.h>
#include <wx/textctrl.h>
#include <wx/button.h>
#include <wx/sizer.h>
#include <wx/msw/winundef.h> 


class RBInputBox: public wxDialog
{
public:
	RBInputBox( const wxString& title, const wxString &Prompt);
RBInputBox( const wxString& title, const wxString &Prompt, wxString wxstrDefault); 
	~RBInputBox();
std::wstring GetText() const;

private:
	void OnCancel( wxCommandEvent& event);
void OnOK( wxCommandEvent& event);

wxPanel * RBPanel;

// Prompt and edit box for new control name
wxStaticText * lblPrompt; 
wxTextCtrl * txtUserInput; 

// Dialog buttons
wxButton * OKButton;
wxButton * CancelButton;

DECLARE_EVENT_TABLE()
};


#endif       // RBINPUTBOX_H