// DefineStateDialog.cpp
// Author:  T A Burgess
// Raised Bar Ltd.
// http://www.raisedbar.net


#include "DefineStateDialog.h"


DefineStateDialog::DefineStateDialog( const wxString& title, std::string strHash, std::vector <unsigned char> vSysExHeader)
       : 
wxDialog(NULL, -1, title, wxDefaultPosition, wxSize(250, 230))
{  
strMyHash = strHash;
vMySysExHeader = vSysExHeader;
vBytes.clear();

wxPanel * myPanel = new wxPanel(this, -1);

	wxBoxSizer * hBoxValues = new wxBoxSizer( wxHORIZONTAL);
lblStateLabelPrompt = new wxStaticText(myPanel, wxID_ANY, wstrStateLabelPrompt);
txtStateLabel = new wxTextCtrl( myPanel, wxID_ANY);
lblMSBPrompt = new wxStaticText(myPanel, wxID_ANY, wstrMSBPrompt);
txtMSBByte = new wxTextCtrl( myPanel, wxID_ANY);
lblLSBPrompt = new wxStaticText(myPanel, wxID_ANY, wstrLSBPrompt);
txtLSBByte = new wxTextCtrl( myPanel, wxID_ANY);
lblSysExBytesPrompt = new wxStaticText(myPanel, wxID_ANY, wstrSysExPrompt);
rbtxtSysExBytes = new RBTextCtrl( myPanel, wxID_ANY);
rbtxtSysExBytes->SetValue( BytesToHex( vBytes));

hBoxValues ->Add( lblStateLabelPrompt, 0, wxEXPAND);
hBoxValues ->Add( txtStateLabel, 0, wxEXPAND);
hBoxValues ->Add( lblMSBPrompt, 0, wxEXPAND);
hBoxValues ->Add( txtMSBByte, 0, wxEXPAND);
hBoxValues ->Add( lblLSBPrompt, 0, wxEXPAND);
hBoxValues ->Add( txtLSBByte, 0, wxEXPAND);
hBoxValues ->Add( lblSysExBytesPrompt, 0, wxEXPAND);
hBoxValues ->Add( rbtxtSysExBytes, 0, wxEXPAND);

// Dialog buttons
wxBoxSizer * hBoxButtons= new wxBoxSizer( wxHORIZONTAL);
wxButton * AddByteButton = new wxButton(myPanel, ID_ADD, wstrAddByteButtonName, wxDefaultPosition, wxSize( wxDefaultSize));
wxButton * RemoveByteButton = new wxButton(myPanel, ID_DELETE, wstrRemoveByteButtonName, wxDefaultPosition, wxSize( wxDefaultSize));
wxButton * OKButton = new wxButton(myPanel, wxID_OK, wstrOKButtonName, wxDefaultPosition, wxSize( wxDefaultSize));
OKButton->SetDefault();
wxButton * CancelButton = new wxButton(myPanel, wxID_CANCEL, wstrCancelButtonName, wxDefaultPosition, wxSize( wxDefaultSize));

hBoxButtons->Add( AddByteButton, 0, wxEXPAND);
hBoxButtons->Add( RemoveByteButton, 0, wxEXPAND);
hBoxButtons->Add( OKButton, 0, wxEXPAND);
hBoxButtons->Add( CancelButton, 0, wxEXPAND);

wxBoxSizer * vBox1 = new wxBoxSizer( wxVERTICAL);
vBox1->Add( hBoxValues);
vBox1->Add( hBoxButtons);	

// Modify the layout according to the message type being manipulated
if (strHash.substr( 0,1).compare( strSysExHashPrefix) == 0)
			{  // SysEx message, so variable number of data bytes
								lblMSBPrompt->Disable();
				lblLSBPrompt->Disable();
				txtMSBByte->Disable();
				txtLSBByte->Disable();
				}
else  // Short message, may be 1 or 2 data bytes
{
	AddByteButton->Disable();
	RemoveByteButton->Disable();
	rbtxtSysExBytes->Disable();	
	
	if (strHash.substr( 0,1).compare( strDoubleHashPrefix) != 0)    // Single data byte
{
	lblMSBPrompt->SetLabel( wstrValuePrompt);	
	lblLSBPrompt->Disable();
	txtLSBByte->Disable();
}  // end if double
}				  // end if SysEx

myPanel->SetSizer( vBox1);
vBox1->SetSizeHints( this );
myPanel->Fit();
vBox1->Fit( myPanel);
Centre();
}


// Constructor to edit an existing state
DefineStateDialog::DefineStateDialog( const wxString& title, std::string strHash, std::vector <unsigned char> vSysExHeader, wxString wxstrLabel, std::vector <unsigned char> vKeyBytes)
: wxDialog(NULL, -1, title, wxDefaultPosition, wxSize(250, 230))
{  
strMyHash = strHash;
vMySysExHeader = vSysExHeader;
vBytes = vKeyBytes;

	wxPanel * myPanel = new wxPanel(this, -1);
	
	wxBoxSizer * hBoxValues = new wxBoxSizer( wxHORIZONTAL);
lblStateLabelPrompt = new wxStaticText(myPanel, wxID_ANY, wstrStateLabelPrompt);
txtStateLabel = new wxTextCtrl( myPanel, wxID_ANY);
txtStateLabel->SetValue( wxstrLabel);
lblMSBPrompt = new wxStaticText(myPanel, wxID_ANY, wstrMSBPrompt);
txtMSBByte = new wxTextCtrl( myPanel, wxID_ANY);
lblLSBPrompt = new wxStaticText(myPanel, wxID_ANY, wstrLSBPrompt);
txtLSBByte = new wxTextCtrl( myPanel, wxID_ANY);
lblSysExBytesPrompt = new wxStaticText(myPanel, wxID_ANY, wstrSysExPrompt);
rbtxtSysExBytes = new RBTextCtrl( myPanel, wxID_ANY);
rbtxtSysExBytes->SetValue( BytesToHex( vBytes)); 

hBoxValues ->Add( lblStateLabelPrompt, 0, wxEXPAND);
hBoxValues ->Add( txtStateLabel, 0, wxEXPAND);
hBoxValues ->Add( lblMSBPrompt, 0, wxEXPAND);
hBoxValues ->Add( txtMSBByte, 0, wxEXPAND);
hBoxValues ->Add( lblLSBPrompt, 0, wxEXPAND);
hBoxValues ->Add( txtLSBByte, 0, wxEXPAND);
hBoxValues ->Add( lblSysExBytesPrompt, 0, wxEXPAND);
hBoxValues ->Add( rbtxtSysExBytes, 0, wxEXPAND);

// Dialog buttons
wxBoxSizer * hBoxButtons= new wxBoxSizer( wxHORIZONTAL);
wxButton * AddByteButton = new wxButton(myPanel, ID_ADD, wstrAddByteButtonName, wxDefaultPosition, wxSize( wxDefaultSize));
wxButton * RemoveByteButton = new wxButton(myPanel, ID_DELETE, wstrRemoveByteButtonName, wxDefaultPosition, wxSize( wxDefaultSize));
wxButton * OKButton = new wxButton(myPanel, wxID_OK, wstrOKButtonName, wxDefaultPosition, wxSize( wxDefaultSize));
wxButton * CancelButton = new wxButton(myPanel, wxID_CANCEL, wstrCancelButtonName, wxDefaultPosition, wxSize( wxDefaultSize));

hBoxButtons->Add( AddByteButton, 0, wxEXPAND);
hBoxButtons->Add( RemoveByteButton, 0, wxEXPAND);
hBoxButtons->Add( OKButton, 0, wxEXPAND);
hBoxButtons->Add( CancelButton, 0, wxEXPAND);

wxBoxSizer * vBox1 = new wxBoxSizer( wxVERTICAL);
vBox1->Add( hBoxValues);
vBox1->Add( hBoxButtons);	

// Modify the layout according to the message type being manipulated
if (strHash.substr( 0,1).compare( strSysExHashPrefix) == 0)
			{  // SysEx message, so variable number of data bytes
				lblMSBPrompt->Disable();
				lblLSBPrompt->Disable();
				txtMSBByte->Disable();
				txtLSBByte->Disable();
}
else  // Short message, may be 1 or 2 data bytes
{
	AddByteButton->Disable();
	RemoveByteButton->Disable();
	rbtxtSysExBytes->Disable();	
	
	if (strHash.substr( 0,1).compare( strDoubleHashPrefix) != 0)    // Single data byte
{
	lblMSBPrompt->SetLabel( wstrValuePrompt);	
	lblLSBPrompt->Disable();
	txtLSBByte->Disable();
	txtMSBByte->SetValue( ByteToHex( vBytes.at( 0)));
}  // end if single
	else
{
	txtMSBByte->SetValue( ByteToHex( vBytes.at( 1)));
	txtLSBByte->SetValue( ByteToHex( vBytes.at( 0)));
	}  // end if double
}				  // end if SysEx

myPanel->SetSizer( vBox1);
vBox1->SetSizeHints( this );
myPanel->Fit();
vBox1->Fit( myPanel);
Centre();
}


DefineStateDialog::~DefineStateDialog()
{}


bool DefineStateDialog::IsValidStateDefinition()
{
if ((IsValidStateLabel())
	&& (IsValidStateValue()))
	{
	return true;
}
else
{
return false;
}
}


std::wstring DefineStateDialog::GetStateName()
{
	return txtStateLabel->GetValue().ToStdWstring();
}


std::vector <unsigned char> DefineStateDialog::GetStateValue()
{
if (strMyHash.substr( 0,1).compare( strSysExHashPrefix) == 0)
			{  // SysEx message, so variable number of data bytes
				return vBytes;
}

		// Short message, may be 1 or 2 data bytes
	vBytes.clear();

	long lMSB;
	std::wstring wstrMSB = txtMSBByte->GetValue().ToStdWstring();
		txtMSBByte->GetValue().ToLong( &lMSB, 16);
		vBytes.push_back( lMSB);
		
if (strMyHash.substr( 0,1).compare( strDoubleHashPrefix) == 0)    // double data byte
{
long lLSB;
txtLSBByte->GetValue().ToLong( &lLSB, 16);
vBytes.push_back( lLSB);
			}  // end if double
	
return vBytes;
}


bool DefineStateDialog::IsValidStateLabel()
{
	if (txtStateLabel->IsEmpty())
	{
		return false;
	}
	else
	{
		return true;
	}
}


bool DefineStateDialog::IsValidStateValue()
{
bool blnResult = false;

if (strMyHash.substr( 0,1).compare( strSysExHashPrefix) == 0)
			{  // SysEx message, so variable number of data bytes
				blnResult = (vBytes.size() > 0);
}

else  // Short message, may be 1 or 2 data bytes
{
	if (strMyHash.substr( 0,1).compare( strDoubleHashPrefix) == 0)    // double data byte
{
	if ((IsSingleHexByteString( txtMSBByte->GetValue().ToStdString()) == false)
		&& (IsSingleHexByteString( txtLSBByte->GetValue().ToStdString()) == false))
	{
		blnResult = true;
	}
	}  // end if double
	else  // single data byte
{
	if (IsSingleHexByteString( txtMSBByte->GetValue().ToStdString()))
{
blnResult = true;
}
	}  // end if single
}				  // end if SysEx

return blnResult;
}


bool DefineStateDialog::IsValidDataByte( long lValue)
{
if ((lValue >= 0) && (lValue <= MAX_MIDI_DATA_BYTE))
{
return true;
}
else
{
return false;
}
}


// Event handlers:

void DefineStateDialog::OnAddDataByte( wxCommandEvent& event)
{
RBInputBox * myInputBox = new RBInputBox( wstrDefineStates, wstrSysExBytePrompt);

if (myInputBox->ShowModal() == wxID_OK)
{
wxString wxstrNewByte = myInputBox->GetText();
		
if (IsSingleHexByteWString( wxstrNewByte.ToStdWstring()))
{
long lTemp;
wxstrNewByte.ToLong( &lTemp, 16);
vBytes.push_back( (unsigned char) lTemp);
rbtxtSysExBytes->SetValue( BytesToHex( vBytes)); 
}  // end if valid hex
else
{
wxMessageBox( wstrBadHexByteError,                wstrAppTitle, wxOK | wxICON_INFORMATION );
}
}  // end if OK

// Tidy up
myInputBox->Destroy(); 
}


void DefineStateDialog::OnRemoveDataByte( wxCommandEvent& event)
{
	if (vBytes.size() > 0)
		{
			vBytes.pop_back();
// Update the display
rbtxtSysExBytes->SetValue( BytesToHex( vBytes)); 
	}
}


void DefineStateDialog::OnOK( wxCommandEvent& event)
		{
EndModal( wxID_OK);
		}

		
		void DefineStateDialog::OnCancel( wxCommandEvent& event)
		{
		EndModal( wxID_CANCEL);
		}

		
		// We need to implement an event table in which the events received by a wxNewSurfaceDialog are routed to their respective handler functions 

	BEGIN_EVENT_TABLE(DefineStateDialog, wxDialog)
		EVT_BUTTON( ID_ADD, DefineStateDialog::OnAddDataByte)
		EVT_BUTTON( ID_DELETE, DefineStateDialog::OnRemoveDataByte)
EVT_BUTTON( wxID_OK, DefineStateDialog::OnOK)
	    EVT_BUTTON( wxID_CANCEL, DefineStateDialog::OnCancel)
END_EVENT_TABLE()
