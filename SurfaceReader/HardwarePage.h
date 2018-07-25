// HardwarePage.h
// Author:  T A Burgess
// Raised Bar Ltd.
// http://www.raisedbar.net

// #define _SCL_SECURE_NO_WARNINGS

#ifndef HardwareControlsPage_H
#define HardwareControlsPage_H

#pragma once

#include <map>

#include "RBEnums.h"
#include "RBInputBox.h"
#include "Protocol.h"
#include "SurfacePage.h"
#include "MIDI.h"
#include "DefineMessageDialog.h"

#include <RtMidi.h>
#include <wx/msw/winundef.h> 

#include <boost/shared_ptr.hpp>

#include <wx/wizard.h>
#include <wx/sizer.h>
#include <wx/StatText.h>
#include <wx/TextCtrl.h>
#include <wx/listbox.h>
#include <wx/button.h>
#include <wx/checkbox.h>
#include <wx/msgdlg.h> 
#include <wx/wizard.h>
#include <wx/msw/winundef.h> 


class HardwareControlsPage: public wxWizardPageSimple
{
public:
HardwareControlsPage( wxWizard * wizParent, const wxString & title, boost::shared_ptr<SurfaceProtocol> pProtocol, boost::shared_ptr<SurfaceParameters> pParameters);
~HardwareControlsPage()
	{
delete myHardwareIn;
};

bool IsValidWizardPage();

virtual bool TransferDataFromWindow();

private:
void ListDefinedControls();
std::wstring FindControlName( wxString wxstrName);
void SetMIDIInputID( int nInputID);
std::wstring MIDILearn();
void CloseInput();

// Event handlers:
void OnDropStatusBytes( wxCommandEvent& event);
void OnAddDefinition( wxCommandEvent& event);
void OnLearnDefinition( wxCommandEvent& event);
void OnEditDefinition( wxCommandEvent& event);
void OnRenameControl( wxCommandEvent& event);
void OnDeleteDefinition( wxCommandEvent& event);
void OnMatchLEDs( wxCommandEvent& event);
    void OnWizardCancel(wxWizardEvent& event);
void OnWizardPageChanging(wxWizardEvent& event);
void OnWizardPageChanged(wxWizardEvent& event);
    
// Internal storage
wxString wxstrTitle;
// List box of defined control names
wxListBox * lbxControlNames; 

// Pointer to the protocol being modified
boost::shared_ptr<SurfaceProtocol> pMyProtocol;
// Pointer to the surface setup
boost::shared_ptr<SurfaceParameters> pMyParameters;

// Buttons to manipulate the list box contents
wxButton * btnAdd, * btnLearn, *btnEdit, * btnDelete, * btnRename, * btnMatchLEDs;

// Check box for message buffering control
wxCheckBox * wxchkDropStatusBytes;

// Port ID for the required input device
int nPortID;
RtMidiIn * myHardwareIn; 
// Flag state of input port
bool blnIsOpen;

// Vector to hold messages for analysis
std::vector <unsigned char> MIDIEvents;	

DECLARE_EVENT_TABLE()
};

#endif       // HardwareControlsPage_H