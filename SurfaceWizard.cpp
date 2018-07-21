// SurfaceWizard.cpp
// Provides a wizard allowing the user to define a new control surface based on an existing protocol
// Author:  T A Burgess
// Raised Bar Ltd.
// http://www.raisedbar.net


#include "SurfaceWizard.h"


SurfaceWizard::SurfaceWizard( wxFrame * myFrame, boost::shared_ptr<ProtocolCollection>  pProtocols, void * pHelpController, bool useSizer)
	:
pMyProtocols( new ProtocolCollection()),
	pSurfaceParameters( new SurfaceParameters ())
	{
pMyProtocols = pProtocols;
// Different platforms use different help controllers
#ifdef __WINDOWS__ 
pMyHelpController = (wxCHMHelpController *) pHelpController;
#else
pMyHelpController = (wxExtHelpController *) pHelpController;
#endif

SetExtraStyle(wxWIZARD_EX_HELPBUTTON);
Create( myFrame,wxID_ANY, wstrAddSurfaceWizardTitle);
	
	// a wizard page may be either an object of predefined class
m_SurfacePage = new SurfacePage( this, wstrAddSurfaceTitle, true, pSurfaceParameters, pMyProtocols);

	if ( useSizer )
    {
        // allow the wizard to size itself around the pages
        GetPageAreaSizer()->Add(m_SurfacePage);
    }
}


SurfaceParameters SurfaceWizard::GetSurfaceParameters()
{
return *pSurfaceParameters;
}


void SurfaceWizard::OnWizardHelp(wxWizardEvent& event)
{
if (event.GetPage() == GetSurfacePage())
{
	pMyHelpController->DisplaySection( wstrSurfacePageHelp);
}
}


// We need to implement an event table in which the events received by a wxSurfacePage are routed to their respective handler functions 
BEGIN_EVENT_TABLE( SurfaceWizard, wxWizard)
	EVT_WIZARD_HELP( wxID_ANY, SurfaceWizard::OnWizardHelp)
END_EVENT_TABLE()
