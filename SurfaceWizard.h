// SurfaceWizard.h

#ifndef SURFACE_WIZARD_H
#define SURFACE_WIZARD_H

#pragma once

#include <memory>

#include "SurfacePage.h"
#include "ProtocolCollection.h"
#include "SurfaceReaderHelp.h"

//#include <wx/frame.h>
//#include <wx/menu.h>
#include <wx/wizard.h>
//#include <wx/platform.h>

#ifdef __WINDOWS__ 
#include <wx/msw/helpchm.h> // (MS HTML Help controller)
#else
#include <wx/generic/helpext.h>
#endif  // if Windows

//#include <wx/msw/winundef.h> 


class SurfaceWizard : public wxWizard
{
public:
SurfaceWizard(wxFrame *frame, std::shared_ptr <ProtocolCollection> pProtocols, void * pHelpController, bool useSizer = true);

SurfaceParameters GetSurfaceParameters();

        	wxWizardPage * GetSurfacePage()
{ 
return m_SurfacePage; 
}

private:
void OnWizardHelp(wxWizardEvent& event);
	
std::shared_ptr <SurfaceParameters> pSurfaceParameters;
	    std::shared_ptr <ProtocolCollection> pMyProtocols;

SurfacePage * m_SurfacePage;

#ifdef __WINDOWS__ 
wxCHMHelpController * pMyHelpController;
#else
wxExtHelpController * pMyHelpController;
#endif

DECLARE_EVENT_TABLE()
};

#endif