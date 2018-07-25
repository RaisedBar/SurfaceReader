// DefineDisplayDialog.h
// Allows definition of a MIDI-controlled display
// Author:  T A Burgess
// Raised Bar Ltd.
// http://www.raisedbar.net

// #define _SCL_SECURE_NO_WARNINGS

#ifndef DefineDisplayDialog_H
#define DefineDisplayDialog_H

#pragma once

#include "DefineMessageDialog.h"
#include "DisplayDefinition.h"
#include "Protocol.h"
#include "RBEnums.h"
#include "SRConstants.h"

#include <boost/shared_ptr.hpp>

#include <wx/bitmap.h>
#include <wx/button.h>
#include <wx/checkbox.h>
#include <wx/gdiobj.h>
#include <wx/gdicmn.h>
#include <wx/dialog.h>
#include <wx/stattext.h>
#include <wx/textctrl.h>
#include <wx/button.h>
#include <wx/sizer.h>
#include <wx/msw/winundef.h> 


class DefineDisplayDialog: public wxDialog
{
public:
	DefineDisplayDialog( const wxString& title, const bool blnIsLED, boost::shared_ptr<SurfaceProtocol> pProtocol);
	DefineDisplayDialog( const wxString& title, const wxString &ControlLabel, const std::wstring &strDisplayHash, DisplayDefinition myDisplay, boost::shared_ptr<SurfaceProtocol> pProtocol);
~DefineDisplayDialog();

 bool IsValidDisplay() const;

 std::wstring GetDisplayHash() const;
std::wstring GetDisplayName() const;
long GetLineCount() const;
long GetDisplayLength() const;
bool IsCursorFromLeft();
bool IsUniCode();
long GetStripCount() const; 
std::vector <unsigned char> GetSysExAddressBytes();

private:
void OnCursorFromLeft( wxCommandEvent& event);	
void OnUniCode( wxCommandEvent& event);	
	
void OnCancel( wxCommandEvent& event);
void OnOK( wxCommandEvent& event);
void OnDefineMIDI( wxCommandEvent& event);

wxPanel * RBPanel;

// Prompt and edit boxes for display properties
wxStaticText * lblDisplayNamePrompt, * lblDisplayLinesPrompt, * lblDisplayLengthPrompt, * lblDisplaySysExCursorOffsetPrompt, * lblStripCountPrompt; 
wxTextCtrl * txtDisplayName, * txtDisplayLines, * txtDisplayLength, * txtDisplaySysExCursorOffset, * txtStripCount; 

// Check box for cursor positioning
wxCheckBox * wxchkCursorFromLeft, * wxchkUniCode;

// Dialog buttons
wxButton * MIDIButton, * OKButton, * CancelButton;

bool blnIsLEDLamp;
std::wstring strHash;
 DisplayDefinition myDisplayDefinition;
boost::shared_ptr<SurfaceProtocol> pMyProtocol;
std::vector <unsigned char> vSysExAddressBytes;
bool blnCursorFromLeft, blnUniCode;

DECLARE_EVENT_TABLE()
};


#endif       // DefineDisplayDialog_H