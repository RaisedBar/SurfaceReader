// SpeechDialog.cpp
// Implementation of the SpeechDialog class to provide a custom dialog allowing the user to select speech parameters on the Mac
// Author:  T A Burgess
// Raised Bar Ltd.
// http://www.raisedbar.net

#ifdef __WXOSX_COCOA__   

#include "SpeechDialog.h"
#include <wx/msw/winundef.h>


SpeechDialog::SpeechDialog(const wxString & title, boost::shared_ptr <RBSpeech> pSpeech)
       : wxDialog(NULL, -1, title, wxDefaultPosition, wxSize( SPEECH_DIALOG_HEIGHT, SPEECH_DIALOG_WIDTH)),
pMySpeech( new RBSpeech())
{
pMySpeech = pSpeech;
	
	// Get initial values
	wstrOldVoice = pMySpeech->GetVoice();
	fOldRate = pMySpeech->GetRate();
	fOldVolume = pMySpeech->GetVolume();
	
	wxPanel * myPanel = new wxPanel(this, -1);

    // List of available voices
wxBoxSizer *hBoxVoiceNames = new wxBoxSizer(wxHORIZONTAL);
  wxStaticText *lblVoiceNamePrompt = new wxStaticText(myPanel, wxID_ANY, wstrVoiceNamePrompt);
lbxVoiceNames = new wxListBox( myPanel, ID_VOICE_NAMES_LIST_BOX);
	ListVoiceNames();
	hBoxVoiceNames->Add( lblVoiceNamePrompt, 0, wxEXPAND);
	hBoxVoiceNames->Add( lbxVoiceNames, 0, wxEXPAND);

	wxBoxSizer *hBoxFields = new wxBoxSizer(wxHORIZONTAL);
wxStaticText * lblVoiceRatePrompt = new wxStaticText(myPanel, wxID_ANY, wstrVoiceRatePrompt);
txtVoiceRate = new wxTextCtrl( myPanel, wxID_ANY);
	wxStaticText *lblVoiceVolumePrompt = new wxStaticText(myPanel, wxID_ANY, wstrVoiceVolumePrompt);
	txtVoiceVolume = new wxTextCtrl( myPanel, wxID_ANY);
	hBoxFields->Add( lblVoiceRatePrompt, 0, wxEXPAND);
	hBoxFields->Add( txtVoiceRate, 0, wxEXPAND);
	hBoxFields->AddSpacer( 25);
	hBoxFields->Add( lblVoiceVolumePrompt, 0, wxEXPAND);
	hBoxFields->Add( txtVoiceVolume, 0, wxEXPAND);

	txtVoiceRate->SetValue( wxString::Format( "%f", fOldRate));
 	txtVoiceVolume->SetValue( wxString::Format( "%f", fOldVolume));
	
	wxBoxSizer *hBoxButtons = new wxBoxSizer(wxHORIZONTAL);
	wxButton * TestButton = new wxButton(myPanel, ID_TEST, wstrTestButtonName);
wxButton * okButton = new wxButton(myPanel, wxID_OK, wstrOKButtonName);
okButton->SetDefault();
	  wxButton *CancelButton = new wxButton(myPanel, wxID_CANCEL, wstrCancelButtonName);
	hBoxButtons->Add( TestButton, 0, wxEXPAND);
	hBoxButtons->Add( okButton, 0, wxEXPAND);
  hBoxButtons->Add( CancelButton, 0, wxEXPAND);

wxBoxSizer *vBox = new wxBoxSizer(wxVERTICAL);
  vBox->Add( hBoxVoiceNames);
vBox->AddSpacer( 25);
	vBox->Add( hBoxFields);
	vBox->AddSpacer( 25);
	  vBox->Add(hBoxButtons);
myPanel->SetSizer(vBox);
myPanel->Fit();
	vBox->Fit( myPanel);
	Centre();
}


SpeechDialog::~SpeechDialog()
{}


void SpeechDialog::ListVoiceNames()
{
	for (unsigned int i = 0; i < pMySpeech->GetVoiceCount(); i++)
	{
		lbxVoiceNames->Append( pMySpeech->GetVoice( i).AfterLast( wxucFullStop));
	}
		// Find the currently-selected voice and its index
	int nSelectedVoice = pMySpeech->GetVoiceIndex( wstrOldVoice);

if (nSelectedVoice > -1)
{
	// Set initial selection
	lbxVoiceNames->SetSelection( nSelectedVoice);
}
}


void SpeechDialog::OnTest(wxCommandEvent& event)
{
	double dRate = fOldRate;
	double dVolume = fOldVolume;
	
	// Set new parameter values
 int nSelection = lbxVoiceNames->GetSelection();
	if (nSelection > -1)
{
	pMySpeech->SetVoice( pMySpeech->GetVoice( nSelection));
}
else
{
	pMySpeech->SetVoice( wstrDefaultVoiceName);
	}
	if (txtVoiceRate->GetValue().ToDouble( &dRate))
	{
		pMySpeech->SetRate( dRate);
	}
	
	if (txtVoiceVolume->GetValue().ToDouble( &dVolume))
	{
		pMySpeech->SetVolume( dVolume);
	}
	
		// Announce test string
	ActiveProduct myActiveProduct;
	DolphinProduct dpProduct;
	DolphinReturnCode dpReturn;

	pMySpeech->GetActiveProduct( myActiveProduct, dpProduct);
pMySpeech->Speak( wstrVoiceTestText, true, dpReturn);
}


void SpeechDialog::OnOK(wxCommandEvent& event)
{
pMySpeech->SaveSpeechParameters();
	EndModal( wxID_OK);
}


void SpeechDialog::OnCancel(wxCommandEvent& event)
{
	// Restore old  parameter values
	pMySpeech->SetVoice( wstrOldVoice);
	pMySpeech->SetRate( fOldRate);
	pMySpeech->SetVolume( fOldVolume);
	
	EndModal( wxID_CANCEL);
}


BEGIN_EVENT_TABLE( SpeechDialog, wxDialog)
EVT_BUTTON(ID_TEST, SpeechDialog::OnTest)
EVT_BUTTON(wxID_OK,  SpeechDialog::OnOK)
EVT_BUTTON( wxID_CANCEL,                 SpeechDialog::OnCancel)
END_EVENT_TABLE()
#endif

