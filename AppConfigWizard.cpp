// AppConfigWizard.cpp
// Provides a wizard allowing the user to define or edit a control surface protocol
// Author:  T A Burgess
// Raised Bar Ltd.
// http://www.raisedbar.net

#define _HAS_STD_BYTE 0

#include "AppConfigWizard.h"

AppConfigWizard::AppConfigWizard( wxFrame * myFrame, bool blnUseSizer, std::shared_ptr<AppConfig> pAppConfig, std::shared_ptr<SurfaceProtocol> pProtocol, std::shared_ptr<RBSpeech> pSpeech, std::shared_ptr<JawsCacheType> pJawsCache, void * pHelpController)
	:
pMyProtocol( new SurfaceProtocol()),
pMyAppConfig( new AppConfig()),
pMySpeech( new RBSpeech()),
pMyJawsCache( new JawsCacheType())
{
pMyAppConfig = pAppConfig;
pMyProtocol = pProtocol;
pMySpeech = pSpeech;
pMyJawsCache = pJawsCache;

// Different platforms use different help controllers 
#ifdef __WINDOWS__ 
pMyHelpController = (wxCHMHelpController *) pHelpController;
#else
pMyHelpController = (wxExtHelpController *) pHelpController;
#endif

SetExtraStyle(wxWIZARD_EX_HELPBUTTON);
auto wstrTitle = wstrAppConfigWizardTitle;
	wstrTitle.append( wstrSpacedOpenParen).append(pMyAppConfig->GetAppName()).append( wstrCloseParen);
Create( myFrame,wxID_ANY,wstrTitle);

wstrTitle = wstrActionsPageTitle;
	wstrTitle.append( wstrSpacedOpenParen).append( wstrAppName).append( wstrCloseParen);

	// Different versions of the ActionsPage class for Windows and Mac
	#ifdef __WINDOWS__ 
		m_ActionsPage = new ActionsPage( this, wstrTitle, pMyAppConfig, pMyProtocol, pMySpeech, pMyJawsCache);
#else
m_ActionsPage = new ActionsPage( this, wstrTitle, pMyAppConfig, pMyProtocol, pMySpeech);
#endif

wstrTitle = wstrDisplaysPageTitle;
	wstrTitle.append( wstrSpacedOpenParen).append( wstrAppName).append( wstrCloseParen);
	m_DisplaysPage = new DisplaysPage(this, wstrTitle, pMyAppConfig, pMyProtocol);
	
// Set page ordering
m_ActionsPage->SetNext( m_DisplaysPage);
m_DisplaysPage->SetPrev( m_ActionsPage);

if (blnUseSizer )
    {
        // allow the wizard to size itself around the pages
        GetPageAreaSizer()->Add(m_ActionsPage);
    }

// Set up the accellerator keys
const unsigned int nKeys = 1;
wxAcceleratorEntry wxaEntries[ nKeys];

// F1 = Help contents    
wxaEntries[ 0].Set( wxACCEL_NORMAL, WXK_F1, ID_WizardHelp);
wxAcceleratorTable wxatKeys( nKeys, wxaEntries);
SetAcceleratorTable(wxatKeys);
}


std::shared_ptr <AppConfig> AppConfigWizard::GetAppConfig()
{
// Apply any page updates
pMyAppConfig->SetHardwareControls( m_ActionsPage->GetActions());
pMyAppConfig->SetDisplays( m_DisplaysPage->GetDisplays());	
	return pMyAppConfig;
}

void AppConfigWizard::OnF1(wxCommandEvent& event)
{
if (true)
{
pMyHelpController->DisplaySection( wstrActionsPageHelp);
return;
}

if (true)
	{
pMyHelpController->DisplaySection( wstrDisplaysPageHelp);
return;
}
}


void AppConfigWizard::OnWizardHelp(wxWizardEvent& event)
{
if (event.GetPage() == GetActionsPage())
{
pMyHelpController->DisplaySection( wstrActionsPageHelp);
return;
}

if (event.GetPage() == GetDisplaysPage())
{
pMyHelpController->DisplaySection( wstrDisplaysPageHelp);
return;
}
}

		
// We need to implement an event table in which the events received by a wxSurfacePage are routed to their respective handler functions 
BEGIN_EVENT_TABLE( AppConfigWizard, wxWizard)
	EVT_WIZARD_HELP( wxID_ANY, AppConfigWizard::OnWizardHelp)
EVT_BUTTON( ID_WizardHelp, AppConfigWizard::OnF1)
END_EVENT_TABLE()
