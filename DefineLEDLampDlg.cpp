// DefineLEDLampDialog.cpp
// Author:  T A Burgess
// Raised Bar Ltd.
// http://www.raisedbar.net

#define _HAS_STD_BYTE 0

#include "DefineLEDLampDlg.h"


DefineLEDLampDialog::DefineLEDLampDialog( const wxString & title, std::shared_ptr<SurfaceProtocol> pProtocol)
       : wxDialog(NULL, -1, title, wxDefaultPosition, wxSize(250, 230)),
	   pMyProtocol( new SurfaceProtocol())
{
pMyProtocol = pProtocol;
	
RBPanel = new wxPanel(this, -1);
	
// Prompt and text box pairs
wxBoxSizer * hInputSizer1 = new wxBoxSizer( wxHORIZONTAL);
lblLampNamePrompt = new wxStaticText(RBPanel, wxID_ANY, wstrLEDLampNamePrompt, wxDefaultPosition, wxSize( wxDefaultSize));
txtLampName = new wxTextCtrl( RBPanel, ID_DISPLAY_NAME, wxEmptyString, wxDefaultPosition, wxSize(wxDefaultSize));

hInputSizer1->Add( lblLampNamePrompt, 0, wxEXPAND);
hInputSizer1->Add( txtLampName, 0, wxEXPAND);

wxBoxSizer * hButtonSizer = new wxBoxSizer( wxHORIZONTAL);
MIDIButton = new wxButton(RBPanel, ID_DEFINE_MIDI, wstrMIDIMessageButtonName, wxDefaultPosition, wxSize( wxDefaultSize));
OKButton = new wxButton(RBPanel, wxID_OK, wstrOKButtonName, wxDefaultPosition, wxSize( wxDefaultSize));
OKButton->SetDefault();
CancelButton = new wxButton(RBPanel, wxID_CANCEL, wstrCancelButtonName, wxDefaultPosition, wxSize( wxDefaultSize));
hButtonSizer->Add( MIDIButton, 0, wxEXPAND);
hButtonSizer->Add( OKButton, 0, wxEXPAND);
hButtonSizer->Add( CancelButton, 0, wxEXPAND);

wxBoxSizer * vBox = new wxBoxSizer( wxVERTICAL); 
vBox->Add( hInputSizer1, 0, wxEXPAND);
vBox->Add( hButtonSizer, 0, wxEXPAND);

RBPanel->SetSizer( vBox);
vBox->SetSizeHints( this );
RBPanel->Fit();
vBox->Fit( RBPanel);
Centre();
this->Maximize();
}


DefineLEDLampDialog:: ~DefineLEDLampDialog()
{}


bool DefineLEDLampDialog::IsValidLEDLamp() const
{
long lLineCount = 0;
long lLineLength = 0;
long lCursorOffset = 0;
long lStripCount = 0;

return (( txtLampName->GetValue().IsEmpty() == false)
&& (lLineCount == 0)
&& (lLineLength == 0)
&& (lCursorOffset == 0)
&& (lStripCount == 0)
&& (strHash.empty() == false)); 
}


std::wstring DefineLEDLampDialog::GetLEDLampName() const
{
return txtLampName->GetValue().ToStdWstring();
}


std::string DefineLEDLampDialog::GetDisplayHash() const
{
return strHash;
}


std::vector <unsigned char> DefineLEDLampDialog::GetSysExAddressBytes()
{
	return vSysExAddressBytes;
}


// Event handlers

void DefineLEDLampDialog::OnDefineMIDI( wxCommandEvent& event)				
{
	DefineMessageDialog * myDefinitionDialog = new DefineMessageDialog( wstrLampSetupTitle, txtLampName->GetValue(), pMyProtocol->GetSysExHeader());
	
if (myDefinitionDialog->ShowModal() == wxID_OK)
{
	strHash = myDefinitionDialog->GetMessageHash();
	}

// Tidy up
myDefinitionDialog->Destroy();
}


void DefineLEDLampDialog::OnCancel(wxCommandEvent& event)
	{
					EndModal( wxID_CANCEL);
	}


void DefineLEDLampDialog::OnOK( wxCommandEvent& event)
	{
					EndModal( wxID_OK);
	}


// We need to implement an event table in which the events received by a wxNewSurfaceDialog are routed to their respective handler functions 

	BEGIN_EVENT_TABLE(DefineLEDLampDialog, wxDialog)
EVT_BUTTON(ID_DEFINE_MIDI, DefineLEDLampDialog::OnDefineMIDI)        
		EVT_BUTTON(wxID_CANCEL, DefineLEDLampDialog::OnCancel)
EVT_BUTTON(wxID_OK, DefineLEDLampDialog::OnOK)
END_EVENT_TABLE()
	

