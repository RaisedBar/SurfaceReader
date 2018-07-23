// AppConfigWizard.h

// #define _SCL_SECURE_NO_WARNINGS

#ifndef APP_CONFIG_WIZARD_H
#define APP_CONFIG_WIZARD_H

#pragma once


#include <map>
#include <vector>
#include <string>
#include <memory>

#include "AppConfig.h"
#include "DisplaysPage.h"
#include "MessageDefinition.h"
#include "Protocol.h"
#include "RBSpeech.h"
#include "SurfaceReaderHelp.h"

#include <boost/shared_ptr.hpp>

//#include <wx/platform.h>
#include <wx/wizard.h>
#include <wx/accel.h>
#include <wx/frame.h>
#include <wx/menu.h>
#include <wx/string.h>

#ifdef __WINDOWS__ 
#include <wx/msw/helpchm.h> // (MS HTML Help controller)
#include "ActionsPageWin.h"
#else
#include <wx/generic/helpext.h>
#include "ActionsPageMac.h"
#endif  // if Windows

#include <wx/msw/winundef.h> 


class AppConfigWizard : public wxWizard
{
public:
AppConfigWizard(wxFrame * myFrame, bool blnUseSizer, boost::shared_ptr<AppConfig> pAppConfig, boost::shared_ptr<SurfaceProtocol> pProtocol, boost::shared_ptr<RBSpeech> pSpeech, boost::shared_ptr<JawsCacheType> pJawsCache, void * pHelpController);

        	boost::shared_ptr <AppConfig> GetAppConfig();
			
			wxWizardPage * GetActionsPage()
{ 
return m_ActionsPage; 
}

wxWizardPage * GetDisplaysPage() const
{ 
return m_DisplaysPage;
}

private:
// Event handlers
void OnF1(wxCommandEvent& event);    
void OnWizardHelp(wxWizardEvent& event);

// Internal storage
ActionsPage * m_ActionsPage;
	DisplaysPage * m_DisplaysPage;

	boost::shared_ptr<SurfaceProtocol> pMyProtocol;
boost::shared_ptr<AppConfig> pMyAppConfig;
boost::shared_ptr<RBSpeech> pMySpeech;
boost::shared_ptr<JawsCacheType> pMyJawsCache;

	
#ifdef __WINDOWS__ 
wxCHMHelpController * pMyHelpController;
#else
wxExtHelpController * pMyHelpController;
#endif

DECLARE_EVENT_TABLE()
};

#endif