// SurfaceReader.cpp
// Purpose:     User interface for the SurfaceReader application
// Author:      T A Burgess


#include "SurfaceReader.h"
#include "Queue.h"

// Queue to handle processing of messages - only creates 1 thread
dispatch_queue SpeechQ(1);

IMPLEMENT_APP( SurfaceReaderApp)

	SurfaceFrame * mySurfaceFrame = NULL;


SurfaceReaderApp::SurfaceReaderApp()
{
	AllowEncryption =false;   // Standard users not allowed to encrypt/decrypt protocol data
	}
		

// 'Main program' equivalent: the program execution starts here
bool SurfaceReaderApp::OnInit()
{
 m_checker = new wxSingleInstanceChecker;
        
					if ( m_checker->IsAnotherRunning() )
        {
            wxMessageBox( wstrAlreadyRunningError, wstrErrorTitle, wxOK | wxICON_EXCLAMATION);
                delete m_checker; 
				// OnExit() won't be called if we return false
            m_checker = NULL;
return false;
        }
	
       // call the base class initialization method, currently it only parses a few common command-line options but it could do more in the future
    if ( !wxApp::OnInit() )
        return false;

    		if ( !wxTaskBarIcon::IsAvailable() )
    {
        wxMessageBox( wstrNoTaskBarSupportError, wstrErrorTitle, wxOK | wxICON_EXCLAMATION);
    }

    // create the main application window
mySurfaceFrame = new SurfaceFrame( wstrAppTitle, AllowEncryption);
	
// and, optionally, show it (the frames, unlike simple controls, are not shown when created initially)
if (mySurfaceFrame->GetStartSized())
{
mySurfaceFrame->Show( true);
}

// success: wxApp::OnRun() will be called which will enter the main message loop and the application will run. If we returned false here, the application would exit immediately.
    return true;
}


// Exception handling

/*
bool SurfaceReaderApp::OnExceptionInMainLoop()
{
    try
    {
        throw;
    }
    	catch ( RBException& e )
	    {
        }
catch ( ... )
    {
        throw;
    }

    return true;
}
*/

/*
void SurfaceReaderApp::OnUnhandledException()
{
    // this shows how we may let some exception propagate uncaught
    try
    {
        throw;
    }
    catch( int nError)
		{
	if (nError == ID_LOAD_DATA_EXCEPTION)
		{
wxMessageBox( wxstrFileLoadError, wxstrErrorTitle, wxOK | wxICON_ERROR);
		}
	else
		{
			wxMessageBox( wxstrUnknownError, wxstrErrorTitle, wxOK | wxICON_ERROR);
	}
	}

	catch ( UnhandledException& )
    {
        throw;
    }
catch ( ... )
    {
        wxMessageBox( wxstrUnhandledExceptionError, wxstrErrorTitle, wxOK | wxICON_ERROR);
    }
}
*/


// Termination code

/*
int SurfaceReaderApp::OnExit()
 {
             int nResult = 0;

return nResult;
}
*/


/*
void SurfaceReaderApp::ShowHelpContents()
{
myHelpController->LoadFile( wxstrHelpFileName);
myHelpController->DisplayContents();
};


void SurfaceReaderApp::ShowHelpSection( unsigned int nID)
{
	myHelpController->DisplaySection( nID);
};


void SurfaceReaderApp::ShowHelpSection( wxString wxstrSection)
{
#ifdef __WINDOWS__ 
myHelpController->LoadFile( wxstrHelpFileName);
myHelpController->DisplaySection( wxstrSection);
#else
myHelpController->DisplaySection( 0);
#endif
};


void SurfaceReaderApp::QuitHelp()
{
	myHelpController->Quit();
delete myHelpController;
}
*/

void SurfaceReaderApp::OnInitCmdLine(wxCmdLineParser& parser)
{
    parser.SetDesc (g_cmdLineDesc);
    // must refuse '/' as parameter starter or cannot use "/path" style paths
    parser.SetSwitchChars ( wstrDash);
}
 

bool SurfaceReaderApp::OnCmdLineParsed(wxCmdLineParser& parser)
{
AllowEncryption = parser.Found( wstrAdminSwitchChar);
    return true;
} 


