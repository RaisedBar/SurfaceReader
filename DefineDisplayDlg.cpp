// DefineDisplayDialog.cpp
// Author:  T A Burgess
// Raised Bar Ltd.
// http://www.raisedbar.net

#define _HAS_STD_BYTE 0

#include "DefineDisplayDlg.h"

// Constructor for new displays
DefineDisplayDialog::DefineDisplayDialog( const wxString & title, const bool blnIsLED, std::shared_ptr<SurfaceProtocol> pProtocol)
       : wxDialog(NULL, -1, title, wxDefaultPosition, wxSize(250, 230)),
	   pMyProtocol( new SurfaceProtocol ()),
	   blnCursorFromLeft( true),
	   blnUniCode( false)
{
pMyProtocol = pProtocol;

// Flag if we're going to define a text display or a LED lamp
	blnIsLEDLamp = blnIsLED;

RBPanel = new wxPanel(this, -1);
	
// Prompt and text box pairs
wxBoxSizer * hInputSizer1 = new wxBoxSizer( wxHORIZONTAL);
lblDisplayNamePrompt = new wxStaticText(RBPanel, wxID_ANY, wstrDisplayNamePrompt, wxDefaultPosition, wxSize( wxDefaultSize));
txtDisplayName = new wxTextCtrl( RBPanel, ID_DISPLAY_NAME, wxEmptyString, wxDefaultPosition, wxSize(wxDefaultSize));

hInputSizer1->Add( lblDisplayNamePrompt, 0, wxEXPAND);
hInputSizer1->Add( txtDisplayName, 0, wxEXPAND);

wxBoxSizer * hInputSizer2 = new wxBoxSizer( wxHORIZONTAL);
lblDisplayLinesPrompt = new wxStaticText(RBPanel, wxID_ANY, wstrDisplayLineCountPrompt, wxDefaultPosition, wxSize( wxDefaultSize));
	txtDisplayLines = new wxTextCtrl( RBPanel, ID_display_line_count, wxEmptyString, wxDefaultPosition, wxSize(wxDefaultSize));
	txtDisplayLines->SetValue( wstrDefaultLines);	
	lblDisplayLengthPrompt = new wxStaticText(RBPanel, wxID_ANY, wstrDisplayLengthPrompt, wxDefaultPosition, wxSize( wxDefaultSize));
		txtDisplayLength = new wxTextCtrl( RBPanel, ID_DISPLAY_LINE_LENGTH, wxEmptyString, wxDefaultPosition, wxSize(wxDefaultSize));
		txtDisplayLength->SetValue( wstrDefaultLineLength);	
		lblStripCountPrompt = new wxStaticText(RBPanel, wxID_ANY, wstrDisplayStripCountPrompt, wxDefaultPosition, wxSize( wxDefaultSize));
wxchkCursorFromLeft = new wxCheckBox( RBPanel, ID_CursorFromLeft, wstrCursorFromLeftPrompt);
wxchkCursorFromLeft->SetValue( blnCursorFromLeft);
txtStripCount = new wxTextCtrl( RBPanel, ID_DISPLAY_LINE_LENGTH, wxEmptyString, wxDefaultPosition, wxSize(wxDefaultSize));
		txtStripCount->SetValue( wstrDefaultStripCount);

if (blnIsLEDLamp == false)
{
		wxchkUniCode = new wxCheckBox( RBPanel, ID_UniCode, wstrUniCodePrompt);
		wxchkUniCode->SetValue( false);		
}				
		
hInputSizer2->Add( lblDisplayLinesPrompt, 0, wxEXPAND);
hInputSizer2->Add( txtDisplayLines, 0, wxEXPAND);
hInputSizer2->Add( 	lblDisplayLengthPrompt, 0, wxEXPAND); 
hInputSizer2->Add( txtDisplayLength, 0, wxEXPAND);
hInputSizer2->Add( wxchkCursorFromLeft, 0, wxEXPAND);
hInputSizer2->Add( lblStripCountPrompt, 0, wxEXPAND);
hInputSizer2->Add( txtStripCount, 0, wxEXPAND);

if (blnIsLEDLamp == false)
{
		hInputSizer2->Add( wxchkUniCode, 0, wxEXPAND);
}		

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
vBox->Add( hInputSizer2, 0, wxEXPAND);
vBox->Add( hButtonSizer, 0, wxEXPAND);

RBPanel->SetSizer( vBox);
vBox->SetSizeHints( this );
RBPanel->Fit();
vBox->Fit( RBPanel);
Centre();
}

// Constructor for editing an existing display
DefineDisplayDialog::DefineDisplayDialog( const wxString& title, const wxString &ControlLabel, const std::string &strDisplayHash, DisplayDefinition myDisplay, std::shared_ptr<SurfaceProtocol> pProtocol)
:
		wxDialog(NULL, -1, title, wxDefaultPosition, wxSize(250, 230)),
	   pMyProtocol( new SurfaceProtocol ()),
	   strHash( strEmpty),
	   blnCursorFromLeft( true),
	   blnUniCode( false)
	   {
// Set the window title to reflect the control being edited
std::wstring wstrTitle = title.ToStdWstring();
wstrTitle.append( wstrSpacedDash);
wstrTitle.append( ControlLabel);
	SetTitle( wstrTitle);

pMyProtocol = pProtocol;
strHash = strDisplayHash;
blnUniCode = myDisplay.IsUniCode();

// Flag if we're editing a text display or a LED lamp
	blnIsLEDLamp = myDisplay.IsLEDLamp();

RBPanel = new wxPanel(this, -1);
	
// Prompt and text box pairs
wxBoxSizer * hInputSizer1 = new wxBoxSizer( wxHORIZONTAL);
lblDisplayNamePrompt = new wxStaticText(RBPanel, wxID_ANY, wstrDisplayNamePrompt, wxDefaultPosition, wxSize( wxDefaultSize));
txtDisplayName = new wxTextCtrl( RBPanel, ID_DISPLAY_NAME, wxEmptyString, wxDefaultPosition, wxSize(wxDefaultSize));
txtDisplayName->SetValue( myDisplay.GetLabel());
txtDisplayName->Disable();

hInputSizer1->Add( lblDisplayNamePrompt, 0, wxEXPAND);
hInputSizer1->Add( txtDisplayName, 0, wxEXPAND);

wxBoxSizer * hInputSizer2 = new wxBoxSizer( wxHORIZONTAL);
lblDisplayLinesPrompt = new wxStaticText(RBPanel, wxID_ANY, wstrDisplayLineCountPrompt, wxDefaultPosition, wxSize( wxDefaultSize));
	txtDisplayLines = new wxTextCtrl( RBPanel, ID_display_line_count, wxEmptyString, wxDefaultPosition, wxSize(wxDefaultSize));

	std::wstring wstrLineCount = boost::lexical_cast <std::wstring> ( myDisplay.GetLineCount());
txtDisplayLines->SetValue( wstrLineCount);	
	
lblDisplayLengthPrompt = new wxStaticText(RBPanel, wxID_ANY, wstrDisplayLengthPrompt, wxDefaultPosition, wxSize( wxDefaultSize));
		txtDisplayLength = new wxTextCtrl( RBPanel, ID_DISPLAY_LINE_LENGTH, wxEmptyString, wxDefaultPosition, wxSize(wxDefaultSize));
		std::wstring wstrLineLength = boost::lexical_cast <std::wstring> ( myDisplay.GetDisplayLength());		
		txtDisplayLength->SetValue( wstrLineLength);	
		
wxchkCursorFromLeft = new wxCheckBox( RBPanel, ID_CursorFromLeft, wstrCursorFromLeftPrompt);
blnCursorFromLeft = myDisplay.IsCursorFromLeft();
wxchkCursorFromLeft->SetValue( blnCursorFromLeft);

		lblStripCountPrompt = new wxStaticText(RBPanel, wxID_ANY, wstrDisplayStripCountPrompt, wxDefaultPosition, wxSize( wxDefaultSize));
txtStripCount = new wxTextCtrl( RBPanel, ID_DISPLAY_LINE_LENGTH, wxEmptyString, wxDefaultPosition, wxSize(wxDefaultSize));
std::wstring wstrStripCount = boost::lexical_cast <std::wstring> ( myDisplay.GetStripCount());		
txtStripCount->SetValue( wstrStripCount);
				
		if (blnIsLEDLamp == false)
{
		wxchkUniCode = new wxCheckBox( RBPanel, ID_UniCode, wstrUniCodePrompt);
		wxchkUniCode->SetValue( blnUniCode);		
}				
		
hInputSizer2->Add( lblDisplayLinesPrompt, 0, wxEXPAND);
hInputSizer2->Add( txtDisplayLines, 0, wxEXPAND);
hInputSizer2->Add( 	lblDisplayLengthPrompt, 0, wxEXPAND); 
hInputSizer2->Add( txtDisplayLength, 0, wxEXPAND);
hInputSizer2->Add( wxchkCursorFromLeft, 0, wxEXPAND);
hInputSizer2->Add( lblStripCountPrompt, 0, wxEXPAND);
hInputSizer2->Add( txtStripCount, 0, wxEXPAND);

if (blnIsLEDLamp == false)
{
		hInputSizer2->Add( wxchkUniCode, 0, wxEXPAND);
}		

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
vBox->Add( hInputSizer2, 0, wxEXPAND);
vBox->Add( hButtonSizer, 0, wxEXPAND);

RBPanel->SetSizer( vBox);
vBox->SetSizeHints( this );
RBPanel->Fit();
vBox->Fit( RBPanel);
Centre();
}

DefineDisplayDialog:: ~DefineDisplayDialog()
{}

bool DefineDisplayDialog::IsValidDisplay() const
{
auto lLines=0L, lDisplayLength =0L, lStripCount =0L;

txtDisplayLines->GetValue() .ToLong( &lLines);
txtDisplayLength->GetValue() .ToLong( &lDisplayLength);
txtStripCount->GetValue() .ToLong( &lStripCount);

return (( !txtDisplayName->GetValue().IsEmpty())
&& (lLines > 0)
&& (lDisplayLength > 0)
&& (lStripCount > 0)
&& (!strHash.empty())); 
}

std::wstring DefineDisplayDialog::GetDisplayName() const
{
return txtDisplayName->GetValue().ToStdWstring();
}

long DefineDisplayDialog::GetLineCount() const
{
auto lTemp1 =0L;
	txtDisplayLines->GetValue() .ToLong( &lTemp1);
	return lTemp1;
}

long DefineDisplayDialog::GetDisplayLength() const
{
auto lTemp1 =0L;
txtDisplayLength->GetValue() .ToLong( &lTemp1); 
	return lTemp1;
}

bool DefineDisplayDialog::IsCursorFromLeft()
{
return blnCursorFromLeft;
}

bool DefineDisplayDialog::IsUniCode()
{
return blnUniCode;
}

long DefineDisplayDialog::GetStripCount() const
{
	auto lTemp1 =0L;
	txtStripCount->GetValue().ToLong( &lTemp1);
	return lTemp1;
}

std::string DefineDisplayDialog::GetDisplayHash() const
{
return strHash;
}

std::vector <unsigned char> DefineDisplayDialog::GetSysExAddressBytes()
{
	return vSysExAddressBytes;
}

// Event handlers
void DefineDisplayDialog::OnCursorFromLeft( wxCommandEvent& event)
{
blnCursorFromLeft = wxchkCursorFromLeft->GetValue();
}

void DefineDisplayDialog::OnUniCode( wxCommandEvent& event)
{
blnUniCode = wxchkUniCode->GetValue();
}

void DefineDisplayDialog::OnDefineMIDI( wxCommandEvent& event)				
{
	DefineMessageDialog * myDefinitionDialog = new DefineMessageDialog( wstrDisplaySetupTitle, txtDisplayName->GetValue(), pMyProtocol->GetSysExHeader(), strHash);
	
if (myDefinitionDialog->ShowModal() == wxID_OK)
{
	strHash = myDefinitionDialog->GetMessageHash();
	vSysExAddressBytes = myDefinitionDialog->GetSysExAddressBytes();
}

// Tidy up
myDefinitionDialog->Destroy(); 
}

void DefineDisplayDialog::OnCancel(wxCommandEvent& event)
	{
					EndModal( wxID_CANCEL);
	}

void DefineDisplayDialog::OnOK( wxCommandEvent& event)
	{
					EndModal( wxID_OK);
	}

// We need to implement an event table in which the events received by a wxNewSurfaceDialog are routed to their respective handler functions 
	BEGIN_EVENT_TABLE(DefineDisplayDialog, wxDialog)
		EVT_CHECKBOX( ID_CursorFromLeft, DefineDisplayDialog::OnCursorFromLeft)
		EVT_CHECKBOX( ID_UniCode, DefineDisplayDialog::OnUniCode)
		
EVT_BUTTON(ID_DEFINE_MIDI, DefineDisplayDialog::OnDefineMIDI)        
		EVT_BUTTON(wxID_CANCEL, DefineDisplayDialog::OnCancel)
EVT_BUTTON(wxID_OK, DefineDisplayDialog::OnOK)
END_EVENT_TABLE()