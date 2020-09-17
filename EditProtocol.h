// EditProtocol.h

#ifndef EDIT_PROTOCOL_H
#define EDIT_PROTOCOL_H

#pragma once

#include "SurfaceParameters.h"
#include "SysExPage.h"
#include "TextDisplaysPage.h"
#include "LEDLampsPage.h"
#include "HardwarePage.h"
#include "SurfaceReaderHelp.h"

#include <boost/shared_ptr.hpp>

#include <wx/frame.h>
#include <wx/menu.h>
#include <wx/platform.h>
#include <wx/wizard.h>

#ifdef __WINDOWS__ 
#include <wx/msw/helpchm.h> // (MS HTML Help controller)
#else
#include <wx/generic/helpext.h>
#endif  // if Windows

#include <wx/msw/winundef.h> 


class EditProtocolWizard : public wxWizard
{
public:
	EditProtocolWizard(wxFrame * myFrame, bool blnUseSizer, boost::shared_ptr<SurfaceProtocol> pProtocol, boost::shared_ptr<SurfaceParameters> pParameters, bool blnEncryptionMode, void * pHelpController);

SurfaceParameters GetSurfaceParameters();

        	wxWizardPage * GetSysExPage() const
{ 
return m_SysExPage;
}


    wxWizardPage * GetTextDisplaysPage() const 
{ 
return m_TextDisplaysPage;
}


    wxWizardPage * GetLEDLampsPage() const 
{ 
return m_LEDLampsPage;
}


    wxWizardPage * GetHardwareControlsPage() const 
{ 
return m_HardwareControlsPage;
}

private:
boost::shared_ptr<SurfaceParameters> pMyParameters;
	boost::shared_ptr<SurfaceProtocol> pMyProtocol;
bool blnEncryptionMode;
    	
// Event handlers
void OnWizardHelp(wxWizardEvent& event);

// Members
ProtocolSysExPage * m_SysExPage;
TextDisplaysPage * m_TextDisplaysPage; 
	LEDLampsPage * m_LEDLampsPage;
HardwareControlsPage * m_HardwareControlsPage;

#ifdef __WINDOWS__ 
wxCHMHelpController * pMyHelpController;
#else
wxExtHelpController * pMyHelpController;
#endif

DECLARE_EVENT_TABLE()
};

#endif