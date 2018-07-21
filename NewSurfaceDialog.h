// NewSurfaceDialog.h
// Custom dialog class definition for the creation of a new control surface
// Author:  T A Burgess
// Raised Bar Ltd.
// http://www.raisedbar.net


#ifndef NEWSURFACE_H
#define NEWSURFACE_H

#pragma once

#include <boost/shared_ptr.hpp>

#include <wx/StatText.h>
#include <wx/TextCtrl.h>
#include <wx/listbox.h>
#include <wx/sizer.h>
#include <wx/button.h>

#include "MIDISurface.h"
#include "protocolCollection.h"
#include "MIDIDialog.h"
#include "RBWizardPage.h"

		
class NewSurfaceDialog: public wxDialog
{
public:
NewSurfaceDialog(const wxString& title, boost::shared_ptr <ProtocolCollection> pMyProtocols, bool PickProtocol);
NewSurfaceDialog(const wxString& title, boost::shared_ptr <ProtocolCollection> pMyProtocols, bool PickProtocol, MIDISurface * pMySurface);
~NewSurfaceDialog();

std::wstring SurfaceName() const;
	std::string ProtocolID();
int Hardware_MIDIIn() const;
int Hardware_VirtualMIDIOut() const;
int Display_VirtualMIDIIn() const;
int Display_MIDIOut() const;
std::string Hardware_MIDIInName() const;
std::string Hardware_VirtualMIDIOutName() const;
std::string Display_VirtualMIDIInName() const;
std::string Display_MIDIOutName() const;
bool IsValidSurface();

void OnDisplaySetup(wxCommandEvent& event);
    void OnHardwareSetup(wxCommandEvent& event);
void OnAdd(wxCommandEvent& event);
void OnNext(wxCommandEvent& event);    
void OnCancel(wxCommandEvent& event);

private:
void ListProtocols();
bool IsValidPortID( int nPortID);
bool IsValidSurfaceName();
bool IsValidProtocol();

wxStaticText * lblSurfaceNamePrompt;
wxStaticText * lblProtocolPrompt;
 	wxTextCtrl * txtSurfaceName;
		wxListBox * lbxSurfaceProtocols;

// Pointer to the surface we're working on
		MIDISurface * pSurface;
		
		// Pointer to the protocol collection
		boost::shared_ptr <ProtocolCollection> pProtocols;
		
		// Path to protocol files
wxString wxstrPath; 		
wxUniChar wxuPathDivider;
wxString wxstrProtocolFileName;
		
// Assigned port ID values
		int nHardware_MIDIIn, nHardware_VirtualMIDIOut, nDisplay_VirtualMIDIIn, nDisplay_MIDIOut;
std::string strHardware_MIDIIn, strHardware_VirtualMIDIOut, strDisplay_VirtualMIDIIn, strDisplay_MIDIOut;	
	
// Event handler for list box selection
void OnProtocolSelect( wxCommandEvent& event);

bool blnListProtocols;

DECLARE_EVENT_TABLE()
};


#endif       // NEWSURFACE_H
