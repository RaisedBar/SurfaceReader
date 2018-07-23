// DefineCharDialog.cpp
// Author:  T A Burgess
// Raised Bar Ltd.
// http://www.raisedbar.net


#include "DefineCharDialog.h"


DefineCharDialog::DefineCharDialog( const wxString& title)
       : wxDialog(NULL, -1, title, wxDefaultPosition, wxSize(250, 230))
{  
wxPanel * myPanel = new wxPanel(this, -1);
	
	wxBoxSizer * hBoxValues = new wxBoxSizer( wxHORIZONTAL);
lblIndexPrompt = new wxStaticText(myPanel, wxID_ANY, wstrCharIndexPrompt);
txtCharIndex = new wxTextCtrl( myPanel, wxID_ANY);
lblLabelPrompt = new wxStaticText(myPanel, wxID_ANY, wstrCharLabelPrompt);
txtCharLabel = new wxTextCtrl( myPanel, wxID_ANY);

hBoxValues ->Add( lblIndexPrompt, 0, wxEXPAND);
hBoxValues ->Add( txtCharIndex, 0, wxEXPAND);
hBoxValues ->Add( lblLabelPrompt, 0, wxEXPAND);
hBoxValues ->Add( txtCharLabel, 0, wxEXPAND);

// Dialog buttons
wxBoxSizer * hBoxButtons= new wxBoxSizer( wxHORIZONTAL);
wxButton * OKButton = new wxButton(myPanel, wxID_OK, wstrOKButtonName, wxDefaultPosition, wxSize( wxDefaultSize));
OKButton->SetDefault();
wxButton * CancelButton = new wxButton(myPanel, wxID_CANCEL, wstrCancelButtonName, wxDefaultPosition, wxSize( wxDefaultSize));
hBoxButtons->Add( OKButton, 0, wxEXPAND);
hBoxButtons->Add( CancelButton, 0, wxEXPAND);

wxBoxSizer * vBox1 = new wxBoxSizer( wxVERTICAL);
vBox1->Add( hBoxValues);
vBox1->Add( hBoxButtons);	

myPanel->SetSizer( vBox1);
vBox1->SetSizeHints( this );
myPanel->Fit();
vBox1->Fit( myPanel);
Centre();
this->Maximize();
}


DefineCharDialog::DefineCharDialog( const wxString& title, int nIndex, std::wstring wstrLabel)
       : wxDialog(NULL, -1, title, wxDefaultPosition, wxSize(250, 230))
{  
	wxPanel * myPanel = new wxPanel(this, -1);
	
	wxBoxSizer * hBoxValues = new wxBoxSizer( wxHORIZONTAL);
lblIndexPrompt = new wxStaticText(myPanel, wxID_ANY, wstrCharIndexPrompt);
txtCharIndex = new wxTextCtrl( myPanel, wxID_ANY);
txtCharIndex->SetValue( ByteToHex( nIndex));

lblLabelPrompt = new wxStaticText(myPanel, wxID_ANY, wstrCharLabelPrompt);
txtCharLabel = new wxTextCtrl( myPanel, wxID_ANY);
txtCharLabel->SetValue( wstrLabel);
hBoxValues ->Add( lblIndexPrompt, 0, wxEXPAND);
hBoxValues ->Add( txtCharIndex, 0, wxEXPAND);
hBoxValues ->Add( lblLabelPrompt, 0, wxEXPAND);
hBoxValues ->Add( txtCharLabel, 0, wxEXPAND);

// Dialog buttons
wxBoxSizer * hBoxButtons= new wxBoxSizer( wxHORIZONTAL);
wxButton * OKButton = new wxButton(myPanel, wxID_OK, wstrOKButtonName, wxDefaultPosition, wxSize( wxDefaultSize));
wxButton * CancelButton = new wxButton(myPanel, wxID_CANCEL, wstrCancelButtonName, wxDefaultPosition, wxSize( wxDefaultSize));
hBoxButtons->Add( OKButton, 0, wxEXPAND);
hBoxButtons->Add( CancelButton, 0, wxEXPAND);

wxBoxSizer * vBox1 = new wxBoxSizer( wxVERTICAL);
vBox1->Add( hBoxValues);
vBox1->Add( hBoxButtons);	

myPanel->SetSizer( vBox1);
vBox1->SetSizeHints( this );
myPanel->Fit();
vBox1->Fit( myPanel);
Centre();
this->Maximize();
}


DefineCharDialog::~DefineCharDialog()
{}


bool DefineCharDialog::IsValidCharDefinition()
{
if ((IsValidCharLabel())
	&& (IsValidCharIndex()))
	{
	return true;
}
else
{
return false;
}
}


std::wstring DefineCharDialog::GetCharLabel()
{
	return txtCharLabel->GetValue().ToStdWstring();
}


int DefineCharDialog::GetCharIndex()
{
long lTemp;

if (txtCharIndex->GetValue().ToLong( &lTemp, 16))
{
	return (int) lTemp;
}
else
{
return -1;
}
}


bool DefineCharDialog::IsValidCharLabel()
{
	if (txtCharLabel->IsEmpty())
	{
		return false;
	}
	else
	{
		return true;
	}
}


bool DefineCharDialog::IsValidCharIndex()
{
long lTemp;
bool blnResult = false;

if (txtCharIndex->GetValue().ToLong( &lTemp, 16))
{
if (lTemp > -1)
{
	blnResult = true;
}
}

	return blnResult;
}


// Event handlers:

void DefineCharDialog::OnOK( wxCommandEvent& event)
		{
	EndModal( wxID_OK);
		}

		
		void DefineCharDialog::OnCancel( wxCommandEvent& event)
		{
		EndModal( wxID_CANCEL);
		}

		
		// We need to implement an event table in which the events received by a wxNewSurfaceDialog are routed to their respective handler functions 

	BEGIN_EVENT_TABLE(DefineCharDialog, wxDialog)
		EVT_BUTTON( wxID_OK, DefineCharDialog::OnOK)
	    EVT_BUTTON( wxID_CANCEL, DefineCharDialog::OnCancel)
END_EVENT_TABLE()
