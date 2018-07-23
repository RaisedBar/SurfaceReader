// SurfaceWizard.h

// #define _SCL_SECURE_NO_WARNINGS

#ifndef SURFACE_WIZARD_H
#define SURFACE_WIZARD_H

#pragma once

#include <boost/shared_ptr.hpp>

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
SurfaceWizard(wxFrame *frame, boost::shared_ptr <ProtocolCollection> pProtocols, void * pHelpController, bool useSizer = true);

SurfaceParameters GetSurfaceParameters();

        	wxWizardPage * GetSurfacePage()
{ 
return m_SurfacePage; 
}

private:
void OnWizardHelp(wxWizardEvent& event);
	
boost::shared_ptr <SurfaceParameters> pSurfaceParameters;
	    boost::shared_ptr <ProtocolCollection> pMyProtocols;

SurfacePage * m_SurfacePage;

#ifdef __WINDOWS__ 
wxCHMHelpController * pMyHelpController;
#else
wxExtHelpController * pMyHelpController;
#endif

DECLARE_EVENT_TABLE()
};

#endif