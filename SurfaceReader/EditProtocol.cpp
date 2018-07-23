// EditProtocol.cpp
// Provides a wizard allowing the user to define or edit a control surface protocol
// Author:  T A Burgess
// Raised Bar Ltd.
// http://www.raisedbar.net


#include "EditProtocol.h"


// Constructor for editing an existing protocol

EditProtocolWizard::EditProtocolWizard( wxFrame * myFrame, bool blnUseSizer, boost::shared_ptr<SurfaceProtocol> pProtocol, boost::shared_ptr<SurfaceParameters> pParameters, bool EncryptionMode, void * pHelpController)
	:
pMyProtocol( new SurfaceProtocol()),
	pMyParameters( new SurfaceParameters())
	{
pMyProtocol = pProtocol;
pMyParameters = pParameters;
blnEncryptionMode = EncryptionMode;	
		
// Different platforms use different help controllers
#ifdef __WINDOWS__ 
pMyHelpController = (wxCHMHelpController *) pHelpController;
#else
pMyHelpController = (wxExtHelpController *) pHelpController;
#endif

SetExtraStyle(wxWIZARD_EX_HELPBUTTON);
		
// Set up the accellerator keys
const unsigned int nKeys = 1;
wxAcceleratorEntry wxaEntries[ nKeys];

// F1 = Help contents    
wxaEntries[ 0].Set( wxACCEL_NORMAL, WXK_F1, wxID_HELP);
wxAcceleratorTable wxatKeys( nKeys, wxaEntries);
SetAcceleratorTable(wxatKeys);

Create( myFrame,wxID_ANY,wstrProtocolWizardTitle);

m_SysExPage= new ProtocolSysExPage(this, wstrSysExPageTitle, pMyProtocol, blnEncryptionMode);
m_TextDisplaysPage = new TextDisplaysPage(this, wstrTextDisplaysPageTitle, pMyProtocol);
m_LEDLampsPage = new LEDLampsPage(this, wstrLEDLampsPageTitle, pMyProtocol);
m_HardwareControlsPage = new HardwareControlsPage( this, wstrHardwareControlsPageTitle, pMyProtocol, pMyParameters);

// Set page ordering
m_SysExPage->SetNext( m_TextDisplaysPage);
m_TextDisplaysPage->SetPrev( m_SysExPage);
m_TextDisplaysPage->SetNext( m_LEDLampsPage);
m_LEDLampsPage->SetPrev( m_TextDisplaysPage);
m_LEDLampsPage->SetNext( m_HardwareControlsPage);
m_HardwareControlsPage->SetPrev( m_LEDLampsPage);

if (blnUseSizer )
    {
        // allow the wizard to size itself around the pages
        GetPageAreaSizer()->Add(m_SysExPage);
    }
}


SurfaceParameters EditProtocolWizard::GetSurfaceParameters()
{
return *pMyParameters;
}


void EditProtocolWizard::OnWizardHelp(wxWizardEvent& event)
{
if (event.GetPage() == GetSysExPage())
{
	pMyHelpController->DisplaySection( wstrSysExPageHelp);
return;
}

if (event.GetPage() == GetTextDisplaysPage())
{
pMyHelpController->DisplaySection( wstrTextDisplaysPageHelp);
return;
}

if (event.GetPage() == GetLEDLampsPage())
{
pMyHelpController->DisplaySection( wstrLEDLampsPageHelp);
return;
}

if (event.GetPage() == GetHardwareControlsPage())
{
pMyHelpController->DisplaySection( wstrHardwareControlsPageHelp);
return;
}
}

		
// We need to implement an event table in which the events received by a wxSurfacePage are routed to their respective handler functions 
BEGIN_EVENT_TABLE(EditProtocolWizard, wxWizard)
	EVT_WIZARD_HELP( wxID_ANY, EditProtocolWizard::OnWizardHelp)
END_EVENT_TABLE()
