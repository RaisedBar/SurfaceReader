// DefineMessageDialog.h
// Custom dialog class definition for the MIDI display message pages of the control surface protocol definition wizard
// Author:  T A Burgess
// Raised Bar Ltd.
// http://www.raisedbar.net

// #define _SCL_SECURE_NO_WARNINGS

#ifndef DEFINE_DISPLAY_MESSAGE_DLG_H
#define DEFINE_DISPLAY_MESSAGE_DLG_H

#pragma once

#include <vector>
#include <algorithm>

#include "MIDI.h"
#include "RBStringFuncs.h"
#include "RBEnums.h"
#include "RBInputBox.h"
#include "RBTextCtrl.h"

#include <boost/lexical_cast.hpp>
using boost::lexical_cast;
using boost::bad_lexical_cast;

#include <RtMidi.h>
#include <wx/msw/winundef.h> 

#include <wx/gdiobj.h>
#include <wx/gdicmn.h>
#include <wx/StatText.h>
#include <wx/TextCtrl.h>
#include <wx/radiobut.h>
#include <wx/sizer.h>
#include <wx/msgdlg.h> 
#include <wx/button.h>
#include <wx/msw/winundef.h> 


const int SINGLE_MIDI_MSG = 1;
const int DUAL_MIDI_MSG = 2;


class DefineMessageDialog: public wxDialog
{
public:
DefineMessageDialog( const wxString& title, const wxString &ControlLabel, std::vector <unsigned char> vHeader);
DefineMessageDialog(const wxString & title, const wxString &ControlLabel, std::vector <unsigned char> vHeader, std::wstring strHash);
       ~DefineMessageDialog();

std::wstring GetMessageHash();
std::vector <unsigned char> GetSysExAddressBytes();
std::vector <unsigned char> GetNibbles();

DECLARE_EVENT_TABLE()

private:
int MessageType( std::wstring strMsgHash);
void InitUI( int nType);
std::wstring GetSingleMessageHash( std::vector <unsigned char> myMessage);
std::wstring DoubleMessageHash( std::vector <unsigned char> myMSBMessage, std::vector <unsigned char> myLSBMessage);
std::wstring SysExMessageHash( std::vector <unsigned char> myMessage, bool blnFake);	

unsigned char GetChannelFromHash(std::string strMsgHash);
unsigned char GetData1FromHash(std::string strMsgHash);
unsigned char GetData2FromHash(std::string strMsgHash);
// std::vector <unsigned char> GetSysExAddressBytesFromHash(std::string strMsgHash, unsigned int nHeaderLength);
std::vector <unsigned char> GetNibblesFromHash( std::wstring strMsgHash);
std::wstring NibblesToHex( std::vector <unsigned char> vNewNibbles);

void EnableCommandEdits();
void DisableCommandEdits();
void EnableChannelEdits();
void DisableChannelEdits();
void EnableData1Edits( int nRadioButton);
void DisableData1Edits();
void EnableData2Edits();
void DisableData2Edits();
void EnableSysExEdits();
void DisableSysExEdits();
void EnableNibbleEdits();
void DisableNibbleEdits();

bool IsValidMessageDefinition();
bool IsValidChannel();
bool IsValidVoiceCommand();
bool IsValidData1();
bool IsValidData2();

// Event handlers:
void OnNoteOn( wxCommandEvent& event);
void OnNoteOnOff( wxCommandEvent& event);
void OnCC( wxCommandEvent& event);
void OnPitchWheel( wxCommandEvent& event);
void OnRPN( wxCommandEvent& event);
void OnNRPN( wxCommandEvent& event);
void OnSysEx( wxCommandEvent& event);
void OnNibbled( wxCommandEvent& event);

void OnAddSysExByte( wxCommandEvent& event);
void OnRemoveSysExByte( wxCommandEvent& event);
void OnAddNibble( wxCommandEvent& event);
void OnRemoveNibble( wxCommandEvent& event);
void OnOK( wxCommandEvent& event);
void OnCancel( wxCommandEvent& event);

// Message type radio buttons
  wxRadioButton *rb_Note_On, *rb_Note_OnOff, *rb_CC, * rb_PitchWheel, *rb_RPN, *rb_NRPN, *rb_SysEx, *rb_Nibble;

  wxStaticText * lblMessageCommandNumberPrompt, * lblMessageChannelPrompt, * lblMessageData1Prompt, * lblMessageData2Prompt;
wxTextCtrl * txtMessageCommand, * txtMessageChannel, * txtMessageData1, * txtMessageData2;

wxStaticText * lblSysExPrompt, * lblNibblePrompt;
RBTextCtrl * rbtxtSysEx, * rbtxtNibble;
wxButton * btnAddByte, * btnRemoveByte, *btnAddNibble, * btnRemoveNibble;

  // Internal storage
    int nMessageType;
std::wstring strHash;
std::vector <unsigned char> vSysExHeader;
		std::vector <unsigned char> vSysExAddressBytes;
std::vector <unsigned char> vNibbles;
};
#endif       // DEFINE_DISPLAY_MESSAGE_DLG_H
