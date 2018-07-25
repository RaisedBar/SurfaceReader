// SurfacePage.h
// Custom dialog class definition for the creation of a new control surface
// Author:  T A Burgess
// Raised Bar Ltd.
// http://www.raisedbar.net

// #define _SCL_SECURE_NO_WARNINGS

#ifndef SURFACE_PAGE_H
#define SURFACE_PAGE_H

#pragma once

#include "ProtocolCollection.h"
#include "SurfaceParameters.h"
#include "protocolCollection.h"
#include "MIDIDialog.h"
#include "SRConstants.h"

#include <boost/shared_ptr.hpp>

#include <wx/msgdlg.h>
#include <wx/wizard.h>
#include <wx/StatText.h>
#include <wx/TextCtrl.h>
#include <wx/listbox.h>
#include <wx/sizer.h>
#include <wx/button.h>
#include <wx/msw/winundef.h> 

		
class SurfacePage: public wxWizardPageSimple
{
public:
	SurfacePage( wxWizard * wizParent, const wxString & title, bool blnPickProtocol, boost::shared_ptr<SurfaceParameters> pSurfaceParameters, boost::shared_ptr<ProtocolCollection> pProtocols);
	
		// ~SurfacePage();

virtual bool TransferDataFromWindow()
{
pMySurfaceParameters->SetSurfaceName( this->GetSurfaceName());
pMySurfaceParameters->SetProtocolID( this->GetProtocolID());
pMySurfaceParameters->SetHardwareInID( this->GetHardware_MIDIIn());
pMySurfaceParameters->SetHardwareOutID( this->GetHardware_VirtualMIDIOut());
pMySurfaceParameters->SetDisplayInID( this->GetDisplay_VirtualMIDIIn());
pMySurfaceParameters->SetDisplayOutID( this->GetDisplay_MIDIOut());
pMySurfaceParameters->SetHardwareInName( this->GetHardware_MIDIInName());
pMySurfaceParameters->SetHardwareOutName( this->GetHardware_VirtualMIDIOutName());
pMySurfaceParameters->SetDisplayInName( this->GetDisplay_VirtualMIDIInName());
pMySurfaceParameters->SetDisplayOutName( this->GetDisplay_MIDIOutName());
	return IsValidSurface();
}

boost::shared_ptr<SurfaceParameters> GetSurfaceParameters()
{
	return pMySurfaceParameters;
}

bool IsValidSurface();

private:
	std::wstring GetSurfaceName();
void SetSurfaceName( std::wstring wstrName);

std::wstring GetProtocolID();
void SetProtocolID( std::wstring strID);

int GetHardware_MIDIIn();
int GetHardware_VirtualMIDIOut();
int GetDisplay_VirtualMIDIIn();
int GetDisplay_MIDIOut();

void SetHardware_MIDIIn( int nID);
void SetHardware_VirtualMIDIOut( int nID);
void SetDisplay_VirtualMIDIIn( int nID);
void SetDisplay_MIDIOut( int nID);

std::wstring GetHardware_MIDIInName();
std::wstring GetHardware_VirtualMIDIOutName();
std::wstring GetDisplay_VirtualMIDIInName();
std::wstring GetDisplay_MIDIOutName();

void GetHardware_MIDIInName( std::wstring strName);
void GetHardware_VirtualMIDIOutName( std::wstring strName);
void GetDisplay_VirtualMIDIInName( std::wstring strName);
void GetDisplay_MIDIOutName( std::wstring strName);

void ListProtocols();
bool IsValidPortID( int nPortID);
bool IsValidSurfaceName();
bool IsValidProtocol();

// Event handler for list box selection
void OnProtocolSelect( wxCommandEvent& event);
void OnDisplaySetup(wxCommandEvent& event);
    void OnHardwareSetup(wxCommandEvent& event);
	void OnWizardCancel(wxWizardEvent& event);
    
    	// Path to protocol files
std::wstring wstrPath; 		
wxUniChar wxuPathDivider;
std::wstring wstrProtocolFileName;
		
// Internal storage
int nHardware_MIDIIn, nHardware_VirtualMIDIOut, nDisplay_VirtualMIDIIn, nDisplay_MIDIOut;
std::wstring strHardware_MIDIInName, strHardware_VirtualMIDIOutName, strDisplay_VirtualMIDIInName, strDisplay_MIDIOutName;	
	
boost::shared_ptr<SurfaceParameters> pMySurfaceParameters;
boost::shared_ptr<ProtocolCollection> pMyProtocols;


wxTextCtrl * txtSurfaceName; 
bool blnListProtocols;
wxStaticText * lblProtocolPrompt;
wxListBox * lbxSurfaceProtocols;

DECLARE_EVENT_TABLE()
};


#endif       // SURFACE_PAGE_H
