// ProtocolWizard.h

#ifndef PROTOCOL_WIZARD_H
#define PROTOCOL_WIZARD_H

#pragma once

//#include "SurfaceParameters.hpp"
#include "SurfacePage.h"
#include "SysExPage.h"
#include "TextDisplaysPage.h"
#include "LEDLampsPage.h"
#include "HardwarePage.h"
#include "ProtocolCollection.h"
#include "SurfaceReaderHelp.h"

#include <memory>

//#include <wx/platform.h>

#ifdef __WINDOWS__ 
#include <wx/msw/helpchm.h> // (MS HTML Help controller)
#else
#include <wx/generic/helpext.h>
#endif  // if Windows

#include <wx/wizard.h>
#include <wx/frame.h>
#include <wx/menu.h>
#include <wx/msw/winundef.h> 


class ProtocolWizard : public wxWizard
{
public:
// Constructor for brand new protocol:
	ProtocolWizard(wxFrame * myFrame, bool blnUseSizer, std::shared_ptr<SurfaceProtocol> pProtocol, bool EncryptionMode, std::shared_ptr<ProtocolCollection>  pProtocols, void * pHelpController);
// Constructor for editing an existing protocol:
	ProtocolWizard(wxFrame * myFrame, bool blnUseSizer, std::shared_ptr<SurfaceProtocol> pProtocol, std::shared_ptr<SurfaceParameters> pParameters, bool blnEncryptionMode, std::shared_ptr<ProtocolCollection> pProtocols, void * pHelpController);

SurfaceParameters GetSurfaceParameters();

        	wxWizardPage * GetSurfacePage()
{ 
return m_SurfacePage; 
}

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
// Event handlers
void OnWizardHelp(wxWizardEvent& event);

std::shared_ptr<SurfaceParameters> pMyParameters;
	std::shared_ptr<ProtocolCollection> pMyProtocols;
std::shared_ptr<SurfaceProtocol> pMyProtocol;
bool blnEncryptionMode;

SurfacePage * m_SurfacePage;
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