// ScreenReaderParametersDlg.cpp
// Purpose:  
// Author:  T A Burgess
// Raised Bar Ltd.
// http://www.raisedbar.net

#define _HAS_STD_BYTE 0

#include "ScreenReaderParametersDlg.h"


ScreenReaderParametersDlg::ScreenReaderParametersDlg(const wxString & title, std::shared_ptr <std::vector<JAWSParameter> > pSpeechParameters)
       : wxDialog(NULL, -1, title, wxDefaultPosition, wxSize(250, 230)),
	   pMySpeechParameters( new JawsParametersType ())
{
pMySpeechParameters = pSpeechParameters;
myPanel = new wxPanel(this, -1);
	
	// Parameter controls
vBoxParameters = new wxBoxSizer( wxVERTICAL);

for (unsigned int i = 0; i < pMySpeechParameters->size(); i++)
{
	std::wstring wstrPrompt = pMySpeechParameters->at(i).Name;
	wstrPrompt.append( wstrSpacedColon);
AddParameter( wstrPrompt);
}

// Buttons 
		  wxBoxSizer * vBoxButtons = new wxBoxSizer( wxVERTICAL);
		  btnOK = new wxButton(myPanel, wxID_OK, wstrOKButtonName, wxDefaultPosition, wxSize( wxDefaultSize));
		  btnOK->SetDefault();
		  btnCancel = new wxButton(myPanel, wxID_CANCEL, wstrCancelButtonName, wxDefaultPosition, wxSize( wxDefaultSize));
vBoxButtons->Add( btnOK, 0, wxEXPAND);
vBoxButtons->Add( btnCancel, 0, wxEXPAND);
	
	wxBoxSizer * hBox1 = new wxBoxSizer( wxHORIZONTAL);
	hBox1->Add( vBoxParameters);		
	hBox1->Add( vBoxButtons);

	wxBoxSizer * vBox1 = new wxBoxSizer( wxVERTICAL);
	vBox1->Add( hBox1);

SetSizerAndFit(vBox1);
}


ScreenReaderParametersDlg::~ScreenReaderParametersDlg()
{}


std::wstring ScreenReaderParametersDlg::GetParameterString()
{
return wstrArguments;
}


void ScreenReaderParametersDlg::AddParameter( std::wstring wstrPrompt)
{
	std::pair <wxStaticText *, wxTextCtrl *> myControlPair;
wxStaticText * lblParameterPrompt = new wxStaticText( myPanel, wxID_ANY, wstrPrompt,  wxPoint( -1, -1), wxSize( -1, -1));
wxTextCtrl * txtParameter= new wxTextCtrl( myPanel, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize);
myControlPair = std::make_pair( lblParameterPrompt, txtParameter);

	std::pair <std::wstring, std::pair <wxStaticText *, wxTextCtrl *> > myParameterPair;
	myParameterPair = std::make_pair( wstrPrompt, myControlPair);
myParameters.insert( myParameterPair);

// Add the new display to the correct sizer
vBoxParameters->Add( myParameterPair.second.first, wxEXPAND);
vBoxParameters->Add( myParameterPair.second.second, wxEXPAND);
}


void ScreenReaderParametersDlg::SetParameter( std::wstring wstrLabel, std::wstring wstrValue)
{
std::map <std::wstring, std::pair < wxStaticText *, wxTextCtrl *> >::iterator it;
it = myParameters.find( wstrLabel);

if (it != myParameters.end())
{
	it->second.second->SetValue( wstrValue);
}
}


// Event handlers

void ScreenReaderParametersDlg::OnOK(wxCommandEvent& event)
{
EndModal( wxID_OK);
}


	void ScreenReaderParametersDlg::OnCancel(wxCommandEvent& event)
	{
	EndModal( wxID_CANCEL);
}


		// We need to implement an event table in which the events received by an ActionPicker are routed to their respective handler functions 

	BEGIN_EVENT_TABLE(ScreenReaderParametersDlg, wxDialog)
// Buttons
EVT_BUTTON(wxID_OK,  ScreenReaderParametersDlg::OnOK)
	    EVT_BUTTON( wxID_CANCEL,                 ScreenReaderParametersDlg::OnCancel)
END_EVENT_TABLE()
	
		