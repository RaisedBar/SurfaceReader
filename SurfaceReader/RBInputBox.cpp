// RBInputBox.cpp
// Author:  T A Burgess
// Raised Bar Ltd.
// http://www.raisedbar.net

#include "RBInputBox.h"


RBInputBox::RBInputBox( const wxString & title, const wxString &Prompt)
       : wxDialog(NULL, -1, title, wxDefaultPosition, wxSize(250, 230))
{
RBPanel = new wxPanel(this, -1);
	
// Prompt and text box
wxBoxSizer * hInputSizer = new wxBoxSizer( wxHORIZONTAL);
lblPrompt = new wxStaticText(RBPanel, wxID_ANY, Prompt, wxDefaultPosition, wxSize( wxDefaultSize));
txtUserInput = new wxTextCtrl( RBPanel, ID_INPUT_BOX_TEXT, wxEmptyString, wxDefaultPosition, wxSize(wxDefaultSize), wxTE_PROCESS_ENTER);
hInputSizer->Add( lblPrompt, 0, wxEXPAND);
hInputSizer->Add( txtUserInput, 0, wxEXPAND);

// Dialog buttons
wxBoxSizer * hButtonSizer = new wxBoxSizer( wxHORIZONTAL);
OKButton = new wxButton(RBPanel, wxID_OK, wstrOKButtonName, wxDefaultPosition, wxSize( wxDefaultSize));
CancelButton = new wxButton(RBPanel, wxID_CANCEL, wstrCancelButtonName, wxDefaultPosition, wxSize( wxDefaultSize));

hButtonSizer->Add( OKButton, 0, wxEXPAND);
hButtonSizer->Add( CancelButton, 0, wxEXPAND);

wxBoxSizer * vBox = new wxBoxSizer( wxVERTICAL); 
vBox->Add( hInputSizer, 0, wxEXPAND);
vBox->Add( hButtonSizer, 0, wxEXPAND);
txtUserInput->SetFocus();

RBPanel->SetSizer( vBox);
vBox->SetSizeHints( this );
RBPanel->Fit();
vBox->Fit( RBPanel);
Centre();

OKButton->SetDefault();
}


RBInputBox::RBInputBox( const wxString& title, const wxString &Prompt, wxString wxstrDefault)
: wxDialog(NULL, -1, title, wxDefaultPosition, wxSize(250, 230))
{
SetAffirmativeId( wxID_OK);
RBPanel = new wxPanel(this, -1);
	
// Prompt and text box
wxBoxSizer * hInputSizer = new wxBoxSizer( wxHORIZONTAL);
lblPrompt = new wxStaticText(RBPanel, wxID_ANY, Prompt, wxDefaultPosition, wxSize( wxDefaultSize));
txtUserInput = new wxTextCtrl( RBPanel, ID_INPUT_BOX_TEXT, wxEmptyString, wxDefaultPosition, wxSize(wxDefaultSize), wxTE_PROCESS_ENTER);
hInputSizer->Add( lblPrompt, 0, wxEXPAND);
hInputSizer->Add( txtUserInput, 0, wxEXPAND);
// Populate the text box with a default string
txtUserInput->SetValue( wxstrDefault);

// Dialog buttons
wxBoxSizer * hButtonSizer = new wxBoxSizer( wxHORIZONTAL);
OKButton = new wxButton(RBPanel, wxID_OK, wstrOKButtonName, wxDefaultPosition, wxSize( wxDefaultSize));
CancelButton = new wxButton(RBPanel, wxID_CANCEL, wstrCancelButtonName, wxDefaultPosition, wxSize( wxDefaultSize));

hButtonSizer->Add( OKButton, 0, wxEXPAND);
hButtonSizer->Add( CancelButton, 0, wxEXPAND);

wxBoxSizer * vBox = new wxBoxSizer( wxVERTICAL); 
vBox->Add( hInputSizer, 0, wxEXPAND);
vBox->Add( hButtonSizer, 0, wxEXPAND);
RBPanel->SetSizer( vBox);
vBox->SetSizeHints( this );
RBPanel->Fit();
vBox->Fit( RBPanel);
Centre();
}


RBInputBox:: ~RBInputBox()
{}


// Event handlers

				void RBInputBox::OnCancel(wxCommandEvent& event)
	{
					EndModal( wxID_CANCEL);
	}


void RBInputBox::OnOK( wxCommandEvent& event)
	{
					EndModal( wxID_OK);
	}


std::wstring RBInputBox::GetText() const
{
	return txtUserInput->GetValue().ToStdWstring();
}


// We need to implement an event table in which the events received by a wxNewSurfaceDialog are routed to their respective handler functions 

	BEGIN_EVENT_TABLE(RBInputBox, wxDialog)
        EVT_BUTTON(wxID_CANCEL, RBInputBox::OnCancel)
EVT_BUTTON(wxID_OK, RBInputBox::OnOK)
// EVT_COMMAND_TEXT_ENTER( ID_INPUT_BOX_TEXT, RBInputBox::OnOK) 
		END_EVENT_TABLE()
	
