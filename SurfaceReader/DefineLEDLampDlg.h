// DefineLEDLampDialog.h
// Allows definition of a MIDI-controlled LED
// Author:  T A Burgess
// Raised Bar Ltd.
// http://www.raisedbar.net

// #define _SCL_SECURE_NO_WARNINGS

#ifndef Define_LED_LAMPDialog_H
#define Define_LED_LAMPDialog_H

#pragma once

#include "SRConstants.h"
#include "Protocol.h"
#include "DisplayDefinition.h"
#include "RBEnums.h"
#include "DefineMessageDialog.h"

#include <boost/shared_ptr.hpp>

#include <wx/button.h>
#include <wx/gdiobj.h>
#include <wx/gdicmn.h>
#include <wx/dialog.h>
#include <wx/stattext.h>
#include <wx/textctrl.h>
#include <wx/button.h>
#include <wx/sizer.h>
#include <wx/msw/winundef.h> 


class DefineLEDLampDialog: public wxDialog
{
public:
	DefineLEDLampDialog( const wxString& title, boost::shared_ptr<SurfaceProtocol> pProtocol);
 ~DefineLEDLampDialog();

 bool IsValidLEDLamp() const;

 std::string GetDisplayHash() const;
std::wstring GetLEDLampName() const;
std::vector <unsigned char> GetSysExAddressBytes();

private:
	void OnCancel( wxCommandEvent& event);
void OnOK( wxCommandEvent& event);
void OnDefineMIDI( wxCommandEvent& event);

wxPanel * RBPanel;

// Prompt and edit boxes for display properties
wxStaticText * lblLampNamePrompt;
wxTextCtrl * txtLampName;

std::string strHash;
 DisplayDefinition myDisplayDefinition;

// Dialog buttons
wxButton * MIDIButton;
wxButton * OKButton;
wxButton * CancelButton;
boost::shared_ptr<SurfaceProtocol> pMyProtocol;
std::vector <unsigned char> vSysExAddressBytes;

DECLARE_EVENT_TABLE()
};


#endif       // Define_LED_LAMPDialog_H