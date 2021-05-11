// DefineMessageDialog.cpp
// Author:  T A Burgess
// Raised Bar Ltd.
// http://www.raisedbar.net

#define _HAS_STD_BYTE 0

#include "DefineMessageDialog.h"

DefineMessageDialog::DefineMessageDialog(const wxString & title, const wxString &ControlLabel, std::vector <unsigned char> vHeader)
       : wxDialog(NULL, -1, title, wxDefaultPosition, wxSize(250, 230))
{  
// Set the window title to reflect the control being edited
std::wstring wstrTitle = title.ToStdWstring();
wstrTitle.append( wstrSpacedDash);
wstrTitle.append( ControlLabel);
	SetTitle( wstrTitle);

	// Set the header for system exclusive messages
	vSysExHeader = vHeader;
	
	wxPanel * myPanel = new wxPanel(this, -1);

// Prompt for the control type to be defined
wxBoxSizer * hBoxMessageType = new wxBoxSizer( wxHORIZONTAL);
	wxBoxSizer * vBoxRadioButtons = new wxBoxSizer( wxVERTICAL);
wxStaticText * lblMessageTypePrompt = new wxStaticText(myPanel, wxID_ANY, wstrMessageTypePrompt);
// Message type radio buttons
auto wstrRadioButtonName = wstrRB_NoteOn;
wstrRadioButtonName.append( wstrSpacedOpenParen);

std::string strHexByte = ByteToHex( (unsigned char) MIDI_CMD_NOTE_ON);
wstrRadioButtonName.append( strHexByte.begin(), strHexByte.end());

wstrRadioButtonName.append( wstrCloseParen);
wxRadioButton *rb_Note_On = new wxRadioButton( myPanel, ID_NOTE_ON, wstrRadioButtonName);

wstrRadioButtonName = wstrRB_NoteOnOff;
wstrRadioButtonName.append( wstrSpacedOpenParen);

strHexByte = ByteToHex( (unsigned char) MIDI_CMD_NOTE_OFF);
wstrRadioButtonName.append( strHexByte.begin(), strHexByte.end());

wstrRadioButtonName.append( wstrCloseParen);
wxRadioButton *rb_Note_OnOff = new wxRadioButton( myPanel, ID_NOTE_ON_OFF, wstrRadioButtonName);
  
wstrRadioButtonName = wstrRB_CC;
wstrRadioButtonName.append( wstrSpacedOpenParen);

strHexByte = ByteToHex( (unsigned char) MIDI_CMD_CONTROL_CHANGE);
wstrRadioButtonName.append( strHexByte.begin(), strHexByte.end());

wstrRadioButtonName.append( wstrCloseParen);
wxRadioButton *rb_CC = new wxRadioButton( myPanel, ID_CC, wstrRadioButtonName);
  
wstrRadioButtonName = wstrRB_PitchWheel;
wstrRadioButtonName.append( wstrSpacedOpenParen);

strHexByte = (ByteToHex( (unsigned char) MIDI_CMD_PITCH_WHEEL));
wstrRadioButtonName.append( strHexByte.begin(), strHexByte.end());

wstrRadioButtonName.append( wstrCloseParen);
wxRadioButton *rb_PitchWheel = new wxRadioButton( myPanel, ID_PITCH_WHEEL, wstrRadioButtonName);
  
wxRadioButton *rb_RPN = new wxRadioButton( myPanel, ID_RPN, wstrRB_RPN);
wxRadioButton *rb_NRPN = new wxRadioButton( myPanel, ID_NRPN, wstrRB_NRPN);

wxRadioButton *rb_SysEx = new wxRadioButton( myPanel, ID_SYSEX, wstrRB_SysEx);
wxRadioButton *rb_Nibble = new wxRadioButton( myPanel, ID_NIBBLE, wstrRB_Nibble);

vBoxRadioButtons->Add( rb_Note_On, wxEXPAND);
vBoxRadioButtons->Add( rb_Note_OnOff, wxEXPAND);
vBoxRadioButtons->Add( rb_CC, wxEXPAND);
vBoxRadioButtons->Add( rb_PitchWheel, wxEXPAND);
vBoxRadioButtons->Add( rb_RPN, wxEXPAND);
vBoxRadioButtons->Add( rb_NRPN, wxEXPAND);

vBoxRadioButtons->Add( rb_SysEx, wxEXPAND);
vBoxRadioButtons->Add( rb_Nibble, wxEXPAND);

hBoxMessageType->Add( lblMessageTypePrompt);
hBoxMessageType->Add( vBoxRadioButtons);

wxBoxSizer * hBoxShortMessage = new wxBoxSizer( wxHORIZONTAL);
lblMessageCommandNumberPrompt = new wxStaticText(myPanel, wxID_ANY, wstrCommandNumberPrompt);
txtMessageCommand = new wxTextCtrl( myPanel, wxID_ANY);
txtMessageCommand->SetValue( wstrDefaultCommand);
lblMessageChannelPrompt = new wxStaticText(myPanel, wxID_ANY, wstrChannelPrompt);
txtMessageChannel = new wxTextCtrl( myPanel, wxID_ANY);
txtMessageChannel->SetValue( wstrDefaultChannel);
lblMessageData1Prompt = new wxStaticText(myPanel, wxID_ANY, wstrData1Prompt);
txtMessageData1 = new wxTextCtrl( myPanel, wxID_ANY);
txtMessageData1->SetValue( wstrDefaultData1);

hBoxShortMessage->Add( lblMessageCommandNumberPrompt, 0, wxEXPAND);
hBoxShortMessage->Add( txtMessageCommand, 0, wxEXPAND);
hBoxShortMessage->Add( lblMessageChannelPrompt, 0, wxEXPAND);
hBoxShortMessage->Add( txtMessageChannel, 0, wxEXPAND);
hBoxShortMessage->Add( lblMessageData1Prompt, 0, wxEXPAND);
hBoxShortMessage->Add( txtMessageData1, 0, wxEXPAND);

wxBoxSizer * hBoxSysExMessage = new wxBoxSizer( wxHORIZONTAL);
lblSysExPrompt = new wxStaticText(myPanel, wxID_ANY, wstrSysExPrompt);
rbtxtSysEx = new RBTextCtrl( myPanel, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_READONLY);
rbtxtSysEx->SetValue( BytesToHex( vSysExHeader));
hBoxSysExMessage->Add( lblSysExPrompt , 0, wxEXPAND);
hBoxSysExMessage->Add( rbtxtSysEx, 0, wxEXPAND);

wxBoxSizer * hBoxSysExButtons= new wxBoxSizer( wxHORIZONTAL);
btnAddByte = new wxButton(myPanel, ID_ADD, wstrAddByteButtonName, wxDefaultPosition, wxSize( wxDefaultSize));
btnRemoveByte = new wxButton(myPanel, ID_DELETE, wstrRemoveByteButtonName, wxDefaultPosition, wxSize( wxDefaultSize));
hBoxSysExButtons->Add( btnAddByte, 0, wxEXPAND);
hBoxSysExButtons->Add( btnRemoveByte, 0, wxEXPAND);

wxBoxSizer * hBoxNibbleMessage = new wxBoxSizer( wxHORIZONTAL);
lblNibblePrompt = new wxStaticText(myPanel, wxID_ANY, wstrNibblePrompt);
rbtxtNibble = new RBTextCtrl( myPanel, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_READONLY);
rbtxtNibble->SetValue( wxEmptyString);
hBoxNibbleMessage->Add( lblNibblePrompt , 0, wxEXPAND);
hBoxNibbleMessage->Add( rbtxtNibble, 0, wxEXPAND);

wxBoxSizer * hBoxNibbleButtons= new wxBoxSizer( wxHORIZONTAL);
btnAddNibble = new wxButton(myPanel, ID_ADD_NIBBLE, wstrAddNibbleButtonName, wxDefaultPosition, wxSize( wxDefaultSize));
btnRemoveNibble = new wxButton(myPanel, ID_DELETE_NIBBLE, wstrRemoveNibbleButtonName, wxDefaultPosition, wxSize( wxDefaultSize));
hBoxNibbleButtons->Add( btnAddNibble, 0, wxEXPAND);
hBoxNibbleButtons->Add( btnRemoveNibble, 0, wxEXPAND);

// Specify the radio button in the group to be checked by default and configure the edit boxes appropriately 
rb_Note_On->SetValue( true); 	  
nMessageType = ID_NOTE_ON;

// Dialog buttons
wxBoxSizer * hButtonSizer = new wxBoxSizer( wxHORIZONTAL);
wxButton * OKButton = new wxButton(myPanel, wxID_OK, wstrOKButtonName, wxDefaultPosition, wxSize( wxDefaultSize));
OKButton->SetDefault();
wxButton * CancelButton = new wxButton(myPanel, wxID_CANCEL, wstrCancelButtonName, wxDefaultPosition, wxSize( wxDefaultSize));
hButtonSizer->Add( OKButton, 0, wxEXPAND);
hButtonSizer->Add( CancelButton, 0, wxEXPAND);

wxBoxSizer * vBox1 = new wxBoxSizer( wxVERTICAL);
	vBox1->Add( hBoxMessageType);
	vBox1->Add( hBoxShortMessage);
	vBox1->Add( hBoxSysExMessage);
vBox1->Add( hBoxSysExButtons);
	vBox1->Add( hButtonSizer);	
	
	myPanel->SetSizer( vBox1);
vBox1->SetSizeHints( this );
myPanel->Fit();
vBox1->Fit( myPanel);
Centre();

DisableCommandEdits();
EnableChannelEdits();
EnableData1Edits( ID_NOTE_ON);
// DisableData2Edits();
DisableSysExEdits();
DisableNibbleEdits();
}

// Constructor to allow editing of an existing message definition
DefineMessageDialog::DefineMessageDialog(const wxString & title, const wxString &ControlLabel, std::vector <unsigned char> vHeader, std::string strHash)
       : wxDialog(NULL, -1, title, wxDefaultPosition, wxSize(250, 230))
{  
// Set the window title to reflect the control being edited
std::wstring wstrTitle = title.ToStdWstring();
wstrTitle.append( wstrSpacedDash);
wstrTitle.append( ControlLabel);
	SetTitle( wstrTitle);
	// Set the header for system exclusive messages
	vSysExHeader = vHeader;
	
// What kind of message are we editing?
	nMessageType = MessageType( strHash);

wxPanel * myPanel = new wxPanel(this, -1);

// Prompt for the control type to be defined
wxBoxSizer * hBoxMessageType = new wxBoxSizer( wxHORIZONTAL);
	wxBoxSizer * vBoxRadioButtons = new wxBoxSizer( wxVERTICAL);
wxStaticText * lblMessageTypePrompt = new wxStaticText(myPanel, wxID_ANY, wstrMessageTypePrompt);
// Message type radio buttons
// Note On
auto wstrRadioButtonName = wstrRB_NoteOn;
wstrRadioButtonName.append( wstrSpacedOpenParen);
std::string strHexByte = ByteToHex( (unsigned char) MIDI_CMD_NOTE_ON);
wstrRadioButtonName.append( strHexByte.begin(), strHexByte.end());
wstrRadioButtonName.append( wstrCloseParen);
wxRadioButton *rb_Note_On = new wxRadioButton( myPanel, ID_NOTE_ON, wstrRadioButtonName);
if (nMessageType == ID_NOTE_ON)
{
	rb_Note_On->SetValue( true);
}

// Note On/Off
wstrRadioButtonName = wstrRB_NoteOnOff;
wstrRadioButtonName.append( wstrSpacedOpenParen);
strHexByte = ByteToHex( (unsigned char) MIDI_CMD_NOTE_OFF);
wstrRadioButtonName.append( strHexByte.begin(), strHexByte.end());
wstrRadioButtonName.append( wstrCloseParen);
wxRadioButton *rb_Note_OnOff = new wxRadioButton( myPanel, ID_NOTE_ON_OFF, wstrRadioButtonName);
if (nMessageType == ID_NOTE_ON_OFF)
{
	rb_Note_OnOff->SetValue( true);
}
  
// CC
wstrRadioButtonName = wstrRB_CC;
wstrRadioButtonName.append( wstrSpacedOpenParen);
strHexByte = ByteToHex( (unsigned char) MIDI_CMD_CONTROL_CHANGE);
wstrRadioButtonName.append( strHexByte.begin(), strHexByte.end());
wstrRadioButtonName.append( wstrCloseParen);
wxRadioButton *rb_CC = new wxRadioButton( myPanel, ID_CC, wstrRadioButtonName);
if (nMessageType == ID_CC)
{
	rb_CC->SetValue( true);
}
  
// Pitch wheel
wstrRadioButtonName = wstrRB_PitchWheel;
wstrRadioButtonName.append( wstrSpacedOpenParen);
strHexByte = (ByteToHex( (unsigned char) MIDI_CMD_PITCH_WHEEL));
wstrRadioButtonName.append( strHexByte.begin(), strHexByte.end());
wstrRadioButtonName.append( wstrCloseParen);
wxRadioButton *rb_PitchWheel = new wxRadioButton( myPanel, ID_PITCH_WHEEL, wstrRadioButtonName);
if (nMessageType == ID_PITCH_WHEEL)
{
	rb_PitchWheel->SetValue( true);
}

// RPN
wxRadioButton *rb_RPN = new wxRadioButton( myPanel, ID_RPN, wstrRB_RPN);
if (nMessageType == ID_RPN)
{
	rb_RPN->SetValue( true);
}

// NRPN
wxRadioButton *rb_NRPN = new wxRadioButton( myPanel, ID_NRPN, wstrRB_NRPN);
if (nMessageType == ID_NRPN)
{
	rb_NRPN->SetValue( true);
}

// System exclusive
wxRadioButton *rb_SysEx = new wxRadioButton( myPanel, ID_SYSEX, wstrRB_SysEx);
if (nMessageType == ID_SYSEX)
{
	rb_SysEx->SetValue( true);
}

// Nibble by nibble
wxRadioButton *rb_Nibble = new wxRadioButton( myPanel, ID_NIBBLE, wstrRB_Nibble);
if (nMessageType == ID_NIBBLE)
{
	rb_Nibble ->SetValue( true);
}

vBoxRadioButtons->Add( rb_Note_On, wxEXPAND);
vBoxRadioButtons->Add( rb_Note_OnOff, wxEXPAND);
vBoxRadioButtons->Add( rb_CC, wxEXPAND);
vBoxRadioButtons->Add( rb_PitchWheel, wxEXPAND);
vBoxRadioButtons->Add( rb_RPN, wxEXPAND);
vBoxRadioButtons->Add( rb_NRPN, wxEXPAND);

vBoxRadioButtons->Add( rb_SysEx, wxEXPAND);
vBoxRadioButtons->Add( rb_Nibble, wxEXPAND);

hBoxMessageType->Add( lblMessageTypePrompt);
hBoxMessageType->Add( vBoxRadioButtons);

wxBoxSizer * hBoxShortMessage = new wxBoxSizer( wxHORIZONTAL);
lblMessageCommandNumberPrompt = new wxStaticText(myPanel, wxID_ANY, wstrCommandNumberPrompt);
txtMessageCommand = new wxTextCtrl( myPanel, wxID_ANY);
txtMessageCommand->SetValue( wstrDefaultCommand);
lblMessageChannelPrompt = new wxStaticText(myPanel, wxID_ANY, wstrChannelPrompt);
txtMessageChannel = new wxTextCtrl( myPanel, wxID_ANY);
txtMessageChannel->SetValue( wstrDefaultChannel);
lblMessageData1Prompt = new wxStaticText(myPanel, wxID_ANY, wstrData1Prompt);
txtMessageData1 = new wxTextCtrl( myPanel, wxID_ANY);
txtMessageData1->SetValue( wstrDefaultData1);

hBoxShortMessage->Add( lblMessageCommandNumberPrompt, 0, wxEXPAND);
hBoxShortMessage->Add( txtMessageCommand, 0, wxEXPAND);
hBoxShortMessage->Add( lblMessageChannelPrompt, 0, wxEXPAND);
hBoxShortMessage->Add( txtMessageChannel, 0, wxEXPAND);
hBoxShortMessage->Add( lblMessageData1Prompt, 0, wxEXPAND);
hBoxShortMessage->Add( txtMessageData1, 0, wxEXPAND);

wxBoxSizer * hBoxSysExMessage = new wxBoxSizer( wxHORIZONTAL);
lblSysExPrompt = new wxStaticText(myPanel, wxID_ANY, wstrSysExPrompt);
rbtxtSysEx = new RBTextCtrl( myPanel, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_READONLY);
vSysExAddressBytes = GetSysExAddressBytesFromHash( strHash, vSysExHeader.size());
auto strSysExMessage = BytesToHex( vSysExHeader);
strSysExMessage.append( BytesToHex( vSysExAddressBytes));
rbtxtSysEx->SetValue( strSysExMessage);

hBoxSysExMessage->Add( lblSysExPrompt , 0, wxEXPAND);
hBoxSysExMessage->Add( rbtxtSysEx, 0, wxEXPAND);

wxBoxSizer * hBoxSysExButtons= new wxBoxSizer( wxHORIZONTAL);
btnAddByte = new wxButton(myPanel, ID_ADD, wstrAddByteButtonName, wxDefaultPosition, wxSize( wxDefaultSize));
btnRemoveByte = new wxButton(myPanel, ID_DELETE, wstrRemoveByteButtonName, wxDefaultPosition, wxSize( wxDefaultSize));
hBoxSysExButtons->Add( btnAddByte, 0, wxEXPAND);
hBoxSysExButtons->Add( btnRemoveByte, 0, wxEXPAND);

wxBoxSizer * hBoxNibbleMessage = new wxBoxSizer( wxHORIZONTAL);
lblNibblePrompt = new wxStaticText(myPanel, wxID_ANY, wstrNibblePrompt);
rbtxtNibble = new RBTextCtrl( myPanel, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_READONLY);
vNibbles = GetNibblesFromHash( strHash);
rbtxtNibble->SetValue( NibblesToHex( vNibbles));
hBoxNibbleMessage->Add( lblNibblePrompt , 0, wxEXPAND);
hBoxNibbleMessage->Add( rbtxtNibble, 0, wxEXPAND);

wxBoxSizer * hBoxNibbleButtons= new wxBoxSizer( wxHORIZONTAL);
btnAddNibble = new wxButton(myPanel, ID_ADD_NIBBLE, wstrAddNibbleButtonName, wxDefaultPosition, wxSize( wxDefaultSize));
btnRemoveNibble = new wxButton(myPanel, ID_DELETE_NIBBLE, wstrRemoveNibbleButtonName, wxDefaultPosition, wxSize( wxDefaultSize));
hBoxNibbleButtons->Add( btnAddNibble, 0, wxEXPAND);
hBoxNibbleButtons->Add( btnRemoveNibble, 0, wxEXPAND);

// Dialog buttons
wxBoxSizer * hButtonSizer = new wxBoxSizer( wxHORIZONTAL);
wxButton * OKButton = new wxButton(myPanel, wxID_OK, wstrOKButtonName, wxDefaultPosition, wxSize( wxDefaultSize));
OKButton->SetDefault();
wxButton * CancelButton = new wxButton(myPanel, wxID_CANCEL, wstrCancelButtonName, wxDefaultPosition, wxSize( wxDefaultSize));
hButtonSizer->Add( OKButton, 0, wxEXPAND);
hButtonSizer->Add( CancelButton, 0, wxEXPAND);

wxBoxSizer * vBox1 = new wxBoxSizer( wxVERTICAL);
	vBox1->Add( hBoxMessageType);
	vBox1->Add( hBoxShortMessage);
	vBox1->Add( hBoxSysExMessage);
vBox1->Add( hBoxSysExButtons);
	vBox1->Add( hButtonSizer);	
	
// Show/hide controls appropriately
		InitUI( nMessageType);

	myPanel->SetSizer( vBox1);
vBox1->SetSizeHints( this );
myPanel->Fit();
vBox1->Fit( myPanel);
Centre();
}

DefineMessageDialog::~DefineMessageDialog()
{}

int DefineMessageDialog::MessageType( std::string strMsgHash)
{
// Figure out what kind of message is represented by the hash string
	
	if (strMsgHash.starts_with( strSysExHashPrefix))
{
	return ID_SYSEX;
}
	else if (strMsgHash.starts_with( strNibbleHashPrefix))
	{
	return ID_NIBBLE;
		}
	else if (strMsgHash.starts_with( strDoubleHashPrefix))
	{
// Need more analysis here for note on/off, rpn, nrpn, etc.
return ID_NOTE_ON_OFF;
	// ID_RPN
	// ID_NRPN
	}
	else
	{
return ID_NOTE_ON;
// ID_CC
// 	ID_PITCH_WHEEL
	}
}	

void DefineMessageDialog::InitUI( int nType)
{
switch( nType)
{
case ID_NOTE_ON:
	{
		DisableCommandEdits();			
EnableChannelEdits();
EnableData1Edits( nMessageType);
// DisableData2Edits();
DisableSysExEdits();
DisableNibbleEdits();
}
	break;

case ID_NOTE_ON_OFF:
	{
DisableCommandEdits();			
EnableChannelEdits();
		EnableData1Edits( nMessageType);
// DisableData2Edits();
DisableSysExEdits();
DisableNibbleEdits();
}
	break;

case ID_CC:
	{
		EnableCommandEdits();			
EnableChannelEdits();
		EnableData1Edits( nMessageType);
// DisableData2Edits();
DisableSysExEdits();
DisableNibbleEdits();
}
	break;

case ID_PITCH_WHEEL:
	{
DisableCommandEdits();			
EnableChannelEdits();
		DisableData1Edits();
// DisableData2Edits();
DisableSysExEdits();
DisableNibbleEdits();
}
	break;

case ID_RPN:
	{
		EnableCommandEdits();			
EnableChannelEdits();
		EnableData1Edits( nMessageType);
// DisableData2Edits();
DisableSysExEdits();
DisableNibbleEdits();
}
	break;

case ID_NRPN:
	{
EnableCommandEdits();			
EnableChannelEdits();
		EnableData1Edits( nMessageType);
// DisableData2Edits();
DisableSysExEdits();
DisableNibbleEdits();
}
	break;

case ID_SYSEX:
	{
DisableCommandEdits();			
DisableChannelEdits();
		DisableData1Edits();
// DisableData2Edits();
EnableSysExEdits();
DisableNibbleEdits();
		// rb_SysEx->SetValue( true);
	}
	break;

case ID_NIBBLE:
{
	DisableCommandEdits();			
DisableChannelEdits();
DisableData1Edits();
DisableSysExEdits();
EnableNibbleEdits();
}
break;

default:
	{}
break;
}  // end switch
}

bool DefineMessageDialog::IsValidMessageDefinition()
{
	return !strHash.empty();
	}

bool DefineMessageDialog::IsValidChannel()
{
auto lTemp =0L;
txtMessageChannel->GetValue().ToLong( &lTemp);
return (lTemp >= MIN_MIDI_CHANNEL) && (lTemp <= MAX_MIDI_CHANNEL);
}

bool DefineMessageDialog::IsValidVoiceCommand()
{
auto lTemp =0L;
txtMessageCommand->GetValue().ToLong( &lTemp);

return (lTemp >= MIDI_CMD_NOTE_OFF) && (lTemp <= MIDI_CMD_PITCH_WHEEL);
}

bool DefineMessageDialog::IsValidData1()
{
auto lTemp =0L;
txtMessageData1->GetValue().ToLong( &lTemp);

return (lTemp >= 0) && (lTemp <= MAX_MIDI_DATA_BYTE);
}

		std::string DefineMessageDialog::GetMessageHash()
			{
MIDI myMIDI;

			switch (nMessageType)
	{
case ID_NOTE_ON:
{
if ((IsValidChannel() == false) || (IsValidData1() == false))
{
	wxMessageBox( wstrInvalidChannelOrNoteNumberError, wstrErrorTitle, wxOK | wxICON_ERROR);
}
else
{
	std::vector <unsigned char> myNoteOnMessage;
	myNoteOnMessage.resize( 3);
	myNoteOnMessage.clear();
	// Construct the message
auto lChannel =0L, lNoteNumber =0l;
auto lCommand = MIDI_CMD_NOTE_ON;

txtMessageChannel->GetValue().ToLong( &lChannel);
// User sees channels as 1-16, MIDI sees them as 0-f, so need to subtract 1 from the channel number:
lChannel = lChannel -1;
auto lStatus = (lCommand *16) + (lChannel);

txtMessageData1->GetValue().ToLong( &lNoteNumber);

myNoteOnMessage.push_back( lStatus);
myNoteOnMessage.push_back( lNoteNumber);
	myNoteOnMessage.push_back( 0);
	strHash = GetSingleMessageHash( myNoteOnMessage);			
}  // end if valid
}
 	break;

case ID_NOTE_ON_OFF:
{
if ((IsValidChannel() == false) || (IsValidData1() == false))
{
	wxMessageBox( wstrInvalidChannelOrNoteNumberError, wstrErrorTitle, wxOK | wxICON_ERROR);
}
else
{
	std::vector <unsigned char> myNoteOnMessage, myNoteOffMessage;

// Construct the messages
auto lCommand =0L, lChannel =0L, lNoteNumber =0L;
txtMessageChannel->GetValue().ToLong( &lChannel);
txtMessageData1->GetValue().ToLong( &lNoteNumber);

lCommand = MIDI_CMD_NOTE_ON;
auto lStatus = (lCommand *16) + (lChannel -1);

myNoteOnMessage.push_back( lStatus);
myNoteOnMessage.push_back( lChannel);
myNoteOnMessage.push_back( lNoteNumber);
lCommand = MIDI_CMD_NOTE_OFF;	
lStatus = (lCommand *16) + (lChannel -1);
myNoteOffMessage.push_back( lStatus);
myNoteOffMessage.push_back( lNoteNumber);
myNoteOffMessage.push_back( MIDI_NOTE_OFF_VELOCITY);
	strHash = DoubleMessageHash( myNoteOnMessage, myNoteOffMessage);
}
}
break;

case ID_PITCH_WHEEL:
{
	if (IsValidChannel() == false)
{
	wxMessageBox( wstrInvalidChannelError, wstrErrorTitle, wxOK | wxICON_ERROR);
}
else
{
std::vector <unsigned char> myPitchWheelMessage;
// Construct the message
auto lCommand =0L, lChannel =0L;
txtMessageChannel->GetValue().ToLong( &lChannel);
lCommand = MIDI_CMD_PITCH_WHEEL;
auto lStatus = (lCommand *16) + (lChannel -1);
myPitchWheelMessage.push_back( lStatus);
myPitchWheelMessage.push_back( 0);
myPitchWheelMessage.push_back( 0);
strHash = GetSingleMessageHash( myPitchWheelMessage);
	}
}
	break;

case ID_CC:
	{
	if ((IsValidChannel() == false) || (IsValidData1() == false))
{
	wxMessageBox( wstrInvalidChannelOrNoteNumberError, wstrErrorTitle, wxOK | wxICON_ERROR);
}
else
{
std::vector <unsigned char> myCCMessage;
// Construct the message
auto lCommand =0L, lCCNumber =0L, lChannel =0L;
txtMessageChannel->GetValue().ToLong( &lChannel);
txtMessageData1->GetValue().ToLong( &lCCNumber);

lCommand = MIDI_CMD_CONTROL_CHANGE;
auto lStatus = (lCommand * 16) + (lChannel -1);
myCCMessage.push_back( lStatus);
myCCMessage.push_back( lCCNumber);
myCCMessage.push_back( MIN_MIDI_VELOCITY);
strHash = GetSingleMessageHash( myCCMessage);
	}
}
	break;

case ID_RPN:
{
if ((IsValidChannel() == false))
{
	wxMessageBox( wstrInvalidChannelError, wstrErrorTitle, wxOK | wxICON_ERROR);
}
else
{
	std::vector <unsigned char> myMSBMessage, myLSBMessage;
auto lChannel =0L;
txtMessageChannel->GetValue().ToLong( &lChannel);

myMSBMessage.push_back( myMIDI.ShortMsgStatus( MIDI_CMD_CONTROL_CHANGE, (lChannel -1)));
myMSBMessage.push_back( CC_RPN_MSB);
myMSBMessage.push_back( MIN_MIDI_VELOCITY);

myLSBMessage.push_back( myMIDI.ShortMsgStatus( MIDI_CMD_CONTROL_CHANGE, (lChannel -1)));
myLSBMessage.push_back( CC_RPN_LSB);
myLSBMessage.push_back( MIN_MIDI_VELOCITY);
strHash = DoubleMessageHash( myMSBMessage, myLSBMessage);
	}
}
break;

case ID_NRPN:
	{
if ((IsValidChannel() == false))
{
	wxMessageBox( wstrInvalidChannelError, wstrErrorTitle, wxOK | wxICON_ERROR);
}
else
{
std::vector <unsigned char> myMSBMessage, myLSBMessage;
auto lChannel =0L;
txtMessageChannel->GetValue().ToLong( &lChannel);

myMSBMessage.push_back( myMIDI.ShortMsgStatus( MIDI_CMD_CONTROL_CHANGE, (lChannel -1)));
myMSBMessage.push_back( CC_NRPN_MSB);
myMSBMessage.push_back( MIN_MIDI_VELOCITY);

myLSBMessage.push_back( myMIDI.ShortMsgStatus( MIDI_CMD_CONTROL_CHANGE, (lChannel -1)));
myLSBMessage.push_back( CC_NRPN_LSB);
myLSBMessage.push_back( MIN_MIDI_VELOCITY);
strHash = DoubleMessageHash( myMSBMessage, myLSBMessage);
	}
}
	break;

case ID_SYSEX:
	{
std::vector <unsigned char> vMessage = vSysExHeader;

for(auto vSysExAddressByte : vSysExAddressBytes)
{
	vMessage.push_back(vSysExAddressByte);
}

		strHash = SysExMessageHash( vMessage, true);
		}
	break;

				case ID_NIBBLE:
	{
strHash = strNibbleHashPrefix;		
		
for(auto vNibble : vNibbles)
		{
	unsigned char nNibble = vNibble;

if (nNibble == NIBBLE_RANGE_MARKER)
{
	strHash.append( NIBBLE_RANGE_MARKER);
}

else if (nNibble == NIBBLE_WILD_CARD_MARKER)
	{
		strHash.append( NIBBLE_WILD_CARD_MARKER);	
	}
else
	{
		strHash.append( boost::lexical_cast <std::string> ( (unsigned int) nNibble));
}
		
strHash.append( HASH_DELIMITER);		
}  // end for
}
 	break;

default:
			{
				std::vector <unsigned char> myMessage;
}
					break;
}  // end switch

	return strHash;
		}
			
	void DefineMessageDialog::DisableCommandEdits()
		{
			lblMessageCommandNumberPrompt->Disable();
			txtMessageCommand->Disable();
}

		void DefineMessageDialog::EnableCommandEdits()
		{
			lblMessageCommandNumberPrompt->Enable();
			txtMessageCommand->Enable();
}

		void DefineMessageDialog::DisableChannelEdits()
		{
			lblMessageChannelPrompt->Disable();
			txtMessageChannel ->Disable();
}

		void DefineMessageDialog::EnableChannelEdits()
		{
			lblMessageChannelPrompt->Enable();
			txtMessageChannel->Enable();
}

		void DefineMessageDialog::DisableData1Edits()
		{
			lblMessageData1Prompt->Disable();
			txtMessageData1->Disable();
		}

/*
void DefineMessageDialog::DisableData2Edits()
		{
			lblMessageData2Prompt->Disable();
			txtMessageData2->Disable();
		}

void DefineMessageDialog::EnableData2Edits()
		{
			lblMessageData2Prompt->Enable();
			txtMessageData2->Enable();
		}
*/

		void DefineMessageDialog::EnableData1Edits( int nRadioButton)
		{
			switch (nRadioButton)
				{
			case ID_NOTE_ON:
			case ID_NOTE_ON_OFF:
				{
					lblMessageData1Prompt->SetLabelText( wstrNoteNumberPrompt);
				}
				break;

			case ID_CC:
				{
lblMessageData1Prompt->SetLabelText( wstrCCNumberPrompt);
}
				break;

			case ID_RPN:
			{
			lblMessageData1Prompt->SetLabelText( wstrRPNMSBPrompt);
				}
			break;
			
			case ID_NRPN:
				{
				lblMessageData1Prompt->SetLabelText( wstrNRPNNumberPrompt);
				}
				break;

case ID_NIBBLE:
				{
				lblMessageData1Prompt->SetLabelText( wstrNibble1Prompt);
				// lblMessageData2Prompt->SetLabelText( wstrNibble2Prompt);
				}
				break;

						default:
				{
				lblMessageData1Prompt->SetLabelText( wxEmptyString);
				}
				break;
			}  // end switch
			
			lblMessageData1Prompt->Enable();
			txtMessageData1->Enable();
		}

void DefineMessageDialog::DisableSysExEdits()
		{
		lblSysExPrompt->Disable();
rbtxtSysEx->Disable();
btnAddByte->Disable();
btnRemoveByte->Disable();
}

		void DefineMessageDialog::EnableSysExEdits()
		{
lblSysExPrompt->Enable();
rbtxtSysEx->Enable();
btnAddByte->Enable();
btnRemoveByte->Enable();
		}

		void DefineMessageDialog::DisableNibbleEdits()
		{
		lblNibblePrompt->Disable();
rbtxtNibble->Disable();
btnAddNibble->Disable();
btnRemoveNibble->Disable();
}

		void DefineMessageDialog::EnableNibbleEdits()
		{
lblNibblePrompt->Enable();
rbtxtNibble->Enable();
btnAddNibble->Enable();
btnRemoveNibble->Enable();
		}

std::string DefineMessageDialog::GetSingleMessageHash( std::vector <unsigned char> myNewMessage)
		{
MIDI myMIDI( myNewMessage, false);
return myMIDI.MIDIHash();
}

std::string DefineMessageDialog::DoubleMessageHash( std::vector <unsigned char> myMSBMessage, std::vector <unsigned char> myLSBMessage)
{
// Join the two messages
	for(auto LSBItem : myLSBMessage)
	{
		myMSBMessage.push_back(LSBItem);
	}

MIDI myMIDI( myMSBMessage, false);
return myMIDI.MIDIHash();
}

std::string DefineMessageDialog::SysExMessageHash( std::vector <unsigned char> vNewMessage, bool blnFake)
{
			MIDI myMIDI( vNewMessage, true);
return myMIDI.MIDIHash();
}

std::vector <unsigned char> DefineMessageDialog::GetSysExAddressBytes()
{
return vSysExAddressBytes;
}

unsigned char DefineMessageDialog::GetChannelFromHash(std::string strMsgHash)
{
unsigned char nChannel = 0;

return nChannel;
}

unsigned char DefineMessageDialog::GetData1FromHash(std::string strMsgHash)
{
unsigned char nData1 = 0;

return nData1;
}

unsigned char DefineMessageDialog::GetData2FromHash(std::string strMsgHash)
{
unsigned char nData2 = 0;

return nData2;
}

std::vector <unsigned char> DefineMessageDialog::GetNibblesFromHash( std::string strMsgHash)
{
std::vector <unsigned char> vNewNibbles;

if (!strMsgHash.starts_with( strNibbleHashPrefix) )
{
	// Not a nibblized message, so return an empty vector
	return vNewNibbles;
}

std::string strByte;

// Ignore the first character ("N"), which is the message type identifier
for (unsigned int i = 1; i < strMsgHash.length(); i++)
{
std::string strChar = strMsgHash.substr( i, 1);	
	
if (strChar.compare( HASH_DELIMITER) == 0)
	{
if (strByte.compare( NIBBLE_RANGE_MARKER) == 0)
{
vNewNibbles.push_back( (unsigned char) NIBBLE_RANGE_MARKER.at( 0));
}
else if (strByte.compare( NIBBLE_WILD_CARD_MARKER) == 0)
	{
		vNewNibbles.push_back( (unsigned char) NIBBLE_WILD_CARD_MARKER.at( 0));
}
	else
	{
try
	{
		unsigned char nByte = (unsigned char) boost::lexical_cast <int> (strByte);
		vNewNibbles.push_back( nByte);
}
catch(bad_lexical_cast &)
{
// wxMessageBox( strByte, wstrErrorTitle, wxOK | wxICON_ERROR);
}
		}
// Re-initialise ready for the next nibble	
strByte.clear();
	}
	else  // Not a nibble delimiter
	{
		strByte.append( strChar);
	}
}  // end for

return vNewNibbles;
}

std::string DefineMessageDialog::NibblesToHex( std::vector <unsigned char> vNewNibbles)
{
std::string strOut;

for(auto vNibbleItem : vNewNibbles)
{
unsigned char nNibble = vNibbleItem;

if ((nNibble == NIBBLE_WILD_CARD_MARKER)
	|| (nNibble == NIBBLE_RANGE_MARKER))
{
	strOut.push_back( nNibble);
}
else
{
	strOut.append( NibbleToHex( nNibble));
}
}  // end for

return strOut;
}

// Event handlers:

				void DefineMessageDialog::OnOK( wxCommandEvent& event)
		{
	EndModal( wxID_OK);
		}

		
		void DefineMessageDialog::OnCancel( wxCommandEvent& event)
		{
		EndModal( wxID_CANCEL);
		}

		
		void DefineMessageDialog::OnNoteOn( wxCommandEvent& event)
		{
			nMessageType = event.GetId();
DisableCommandEdits();			
EnableChannelEdits();
EnableData1Edits( nMessageType);
// DisableData2Edits();
DisableSysExEdits();
DisableNibbleEdits();
		}


void DefineMessageDialog::OnNoteOnOff( wxCommandEvent& event)
{
nMessageType = event.GetId();
DisableCommandEdits();			
EnableChannelEdits();
		EnableData1Edits( nMessageType);
// DisableData2Edits();
DisableSysExEdits();
DisableNibbleEdits();
}


void DefineMessageDialog::OnCC( wxCommandEvent& event)
{
nMessageType = event.GetId();
EnableCommandEdits();			
EnableChannelEdits();
		EnableData1Edits( nMessageType);
// DisableData2Edits();
DisableSysExEdits();
DisableNibbleEdits();
}


void DefineMessageDialog::OnPitchWheel( wxCommandEvent& event)
{
nMessageType = event.GetId();
DisableCommandEdits();			
EnableChannelEdits();
		DisableData1Edits();
// DisableData2Edits();
DisableSysExEdits();
DisableNibbleEdits();
}


void DefineMessageDialog::OnRPN( wxCommandEvent& event)
{
nMessageType = event.GetId();
EnableCommandEdits();			
EnableChannelEdits();
		EnableData1Edits( nMessageType);
// DisableData2Edits();
DisableSysExEdits();
DisableNibbleEdits();
}


void DefineMessageDialog::OnNRPN( wxCommandEvent& event)
{
nMessageType = event.GetId();
EnableCommandEdits();			
EnableChannelEdits();
		EnableData1Edits( nMessageType);
// DisableData2Edits();
DisableSysExEdits();
DisableNibbleEdits();
}


void DefineMessageDialog::OnSysEx( wxCommandEvent& event)
{
nMessageType = event.GetId();
DisableCommandEdits();			
DisableChannelEdits();
		DisableData1Edits();
// DisableData2Edits();
EnableSysExEdits();
DisableNibbleEdits();
}


void DefineMessageDialog::OnNibbled( wxCommandEvent& event)
{
nMessageType = event.GetId();
DisableCommandEdits();			
DisableChannelEdits();
DisableData1Edits();
DisableSysExEdits();
EnableNibbleEdits();
}


void DefineMessageDialog::OnAddSysExByte( wxCommandEvent& event)
{
RBInputBox * myInputBox = new RBInputBox( wstrDefineMessageTitle, wstrSysExBytePrompt);

if (myInputBox->ShowModal() == wxID_OK)
{
std::wstring wstrNewByte = myInputBox->GetText();
	
if (IsSingleHexByteWString( wstrNewByte))
{
	long lTemp;
	wxString wxstrTemp( wstrNewByte);
	wxstrTemp.ToLong( &lTemp, 16);	
	vSysExAddressBytes.push_back( (unsigned char) lTemp);

// Update the display
std::string strMessage = BytesToHex( vSysExHeader);
strMessage.append( BytesToHex( vSysExAddressBytes));
rbtxtSysEx->SetValue( strMessage);
}  // end if valid hex
else
{
wxMessageBox( wstrBadHexByteError,                wstrErrorTitle, wxOK | wxICON_ERROR);
}
}  // end if OK

// Tidy up
myInputBox->Destroy(); 
}


void DefineMessageDialog::OnRemoveSysExByte( wxCommandEvent& event)
{
	if (vSysExAddressBytes.size() == 0)
{
	wxMessageBox( wstrNoSysExAddressRemovalError, wstrErrorTitle, wxOK | wxICON_ERROR);
}
	else
	{
			vSysExAddressBytes.pop_back();
// Update the display
std::string strMessage = BytesToHex( vSysExHeader);
strMessage.append( BytesToHex( vSysExAddressBytes));
rbtxtSysEx->SetValue( strMessage);
rbtxtSysEx->Refresh();
	}
}


void DefineMessageDialog::OnAddNibble( wxCommandEvent& event)
{
RBInputBox * myInputBox = new RBInputBox( wstrDefineMessageTitle, wstrNewNibblePrompt);

if (myInputBox->ShowModal() == wxID_OK)
{
	std::wstring wstrNewNibble = myInputBox->GetText();
	
if (IsHexNibbleWString( wstrNewNibble))
{
long lTemp;
	wxString wxstrTemp( wstrNewNibble);
	wxstrTemp.ToLong( &lTemp, 16);	
vNibbles.push_back( (unsigned char) lTemp);

// Update the display
rbtxtNibble->SetValue( NibblesToHex( vNibbles));
rbtxtNibble->Refresh();
}  // end if valid hex nibble 
else
	{
if (IsWNibbleWildCard( wstrNewNibble))
{
	vNibbles.push_back( (unsigned char) NIBBLE_WILD_CARD_MARKER.at( 0));
// Update the display
	std::wstring wstrMessage = rbtxtNibble->GetValue().ToStdWstring();
wstrMessage.append( wstrNewNibble);
rbtxtNibble->SetValue( wstrMessage);
rbtxtNibble->Refresh();
}

		else if (IsWHexNibbleRange( wstrNewNibble))
{
	vNibbles.push_back( (unsigned char) NIBBLE_RANGE_MARKER.at( 0));

	long lTemp;
	wxString wxstrTemp( wstrNewNibble.at( 0));
	wxstrTemp.ToLong( &lTemp, 16);	
vNibbles.push_back( (unsigned char) lTemp);

wxstrTemp = wstrNewNibble.at( wstrNewNibble.length() -1);
wxstrTemp.ToLong( &lTemp, 16);	
vNibbles.push_back( (unsigned char) lTemp);

// Update the display
std::wstring wstrMessage = rbtxtNibble->GetValue().ToStdWstring();
wstrMessage.append( wstrNewNibble);
rbtxtNibble->SetValue( wstrMessage);
rbtxtNibble->Refresh();
}
else
{
wxMessageBox( wstrBadHexNibbleError,                wstrErrorTitle, wxOK | wxICON_ERROR);
}
}
}  // end if OK

// Tidy up
myInputBox->Destroy(); 
}


void DefineMessageDialog::OnRemoveNibble( wxCommandEvent& event)
{
	if (vNibbles.size() == 0)
{
		wxMessageBox( wstrNoNibbleRemovalError, wstrErrorTitle, wxOK | wxICON_ERROR);
}
	else
	{
			vNibbles.pop_back();
// Update the display
std::string strMessage = NibblesToHex( vNibbles);
rbtxtNibble->SetValue( strMessage);
rbtxtNibble->Refresh();
	}
}


// We need to implement an event table in which the events received by a wxNewSurfaceDialog are routed to their respective handler functions 

	BEGIN_EVENT_TABLE(DefineMessageDialog, wxDialog)
        // Radio buttons
		EVT_RADIOBUTTON(ID_NOTE_ON, DefineMessageDialog::OnNoteOn)
EVT_RADIOBUTTON(ID_NOTE_ON_OFF, DefineMessageDialog::OnNoteOnOff)
EVT_RADIOBUTTON(ID_CC, DefineMessageDialog::OnCC)
EVT_RADIOBUTTON(ID_PITCH_WHEEL, DefineMessageDialog::OnPitchWheel)
EVT_RADIOBUTTON(ID_RPN, DefineMessageDialog::OnRPN)
EVT_RADIOBUTTON(ID_NRPN, DefineMessageDialog::OnNRPN)
EVT_RADIOBUTTON(ID_SYSEX, DefineMessageDialog::OnSysEx)
EVT_RADIOBUTTON(ID_NIBBLE, DefineMessageDialog::OnNibbled)
	
// SysEx buttons
EVT_BUTTON( ID_ADD, DefineMessageDialog::OnAddSysExByte)
EVT_BUTTON( ID_DELETE, DefineMessageDialog::OnRemoveSysExByte)

// Nibble buttons
EVT_BUTTON( ID_ADD_NIBBLE, DefineMessageDialog::OnAddNibble)
EVT_BUTTON( ID_DELETE_NIBBLE, DefineMessageDialog::OnRemoveNibble)

// Navigation buttons
EVT_BUTTON( wxID_OK, DefineMessageDialog::OnOK)
	    EVT_BUTTON( wxID_CANCEL, DefineMessageDialog::OnCancel)
END_EVENT_TABLE()
