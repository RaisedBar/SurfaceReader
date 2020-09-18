// SysExPage.h
// Custom dialog class definition for the System Exclusive page of the control surface protocol definition wizard
// Author:  T A Burgess
// Raised Bar Ltd.
// http://www.raisedbar.net

#ifndef PROTOCOLSYSEXPAGE_H
#define PROTOCOLSYSEXPAGE_H

#pragma once

#include <vector>
#include <string>

#include <memory>
#include <boost/lexical_cast.hpp>
using boost::lexical_cast;
using boost::bad_lexical_cast;

#include "Protocol.h"
#include "RBStringFuncs.h"

#include <wx/sizer.h>
#include <wx/msgdlg.h>
#include <wx/StatText.h>
#include <wx/TextCtrl.h>
#include <wx/wizard.h>
#include <wx/checkbox.h>
#include <wx/msw/winundef.h> 


class ProtocolSysExPage: public wxWizardPageSimple
{
public:
ProtocolSysExPage(wxWizard * wizParent, const wxString& title, std::shared_ptr<SurfaceProtocol> pProtocol, bool EncryptionMode);

std::wstring GetProtocolName() const;
void SetProtocolName( std::wstring wstrProtocolName);
std::vector <long> GetSysExManufacturerID();
void SetSysExManufacturerID( std::vector <long> vNewManufacturerID);
long GetSysExModelID() const;
void SetSysExModelID( long lModelID);
long GetSysExDeviceID() const;
void SetSysExDeviceID( long lDeviceID);
bool IsValidWizardPage();

virtual bool TransferDataFromWindow()
{
bool blnResult = IsValidWizardPage();

if (blnResult)
{
	pMyProtocol->SetProtocolName( this->GetProtocolName());
	pMyProtocol->SetSysExManufacturerID( this->GetSysExManufacturerID());
	pMyProtocol->SetSysExModelID( this->GetSysExModelID());
	pMyProtocol->SetSysExDeviceID( this->GetSysExDeviceID());
}
return blnResult;
}

private:
bool IsValidProtocolName();
bool IsValidSysExManufacturerID();
bool IsValidSysExModelID();
bool IsValidSysExDeviceID() const;

// Event handlers
void OnWizardCancel(wxWizardEvent& event);
void OnEncryptionChange(wxCommandEvent& event);    
    
		// Internal storage
std::shared_ptr<SurfaceProtocol> pMyProtocol;
wxTextCtrl * txtProtocolName; 
		wxTextCtrl * txtSysExManufacturerByte1, * txtSysExManufacturerByte2, * txtSysExManufacturerByte3;
wxTextCtrl * txtSysExModelID;
wxTextCtrl * txtSysExDeviceID;
wxCheckBox * wxchkEncryptedProtocol;
bool blnEncryptionMode;

// Temporary storage for manufacturer ID
std::vector <long> vManufacturerID;

DECLARE_EVENT_TABLE()
};

#endif       // PROTOCOLSYSEXPAGE_H