	// SurfaceFrame.cpp


#include "SurfaceFrame.h"

#ifdef __WINDOWS__
// #include <vld.h>
#endif

#include "smile.xpm"
#include "sample.xpm"


// Implementation of the task bar/dock handling


extern SurfaceFrame * mySurfaceFrame;

enum TBOptions
{
    PU_SHOWHIDE = 10001,
    PU_QUIT
           };


BEGIN_EVENT_TABLE(myTaskBarIcon, wxTaskBarIcon)
    EVT_MENU(PU_SHOWHIDE, myTaskBarIcon::OnMenuShowHide)
    EVT_MENU(PU_QUIT,    myTaskBarIcon::OnMenuQuit)
        EVT_TASKBAR_LEFT_DCLICK  (myTaskBarIcon::OnLeftButtonDClick)
    END_EVENT_TABLE()


	wxMenu *myTaskBarIcon::CreatePopupMenu()
{
    menu = new wxMenu;
    menu->Append( PU_SHOWHIDE, wstrTBShow);
    menu->AppendSeparator();
    
    // OSX has built-in quit menu for the dock menu, but not for the status item 
#ifdef __WXOSX__ 
    if ( OSXIsStatusItem() )
#endif
    {
        menu->AppendSeparator();
        menu->Append( PU_QUIT, wstrTBQuit);
    }
    return menu;
}

void myTaskBarIcon::OnMenuShowHide(wxCommandEvent& )
{
if (mySurfaceFrame->IsShown())
{
	mySurfaceFrame->Show(false);
	menu->SetLabel( PU_SHOWHIDE, wxT( "Show"));
				}
else
{
mySurfaceFrame->Show(true);
menu->SetLabel( PU_SHOWHIDE, wxT( "Hide"));
}
}

void myTaskBarIcon::OnMenuQuit(wxCommandEvent& )
{
	mySurfaceFrame->Shutdown();
}

void myTaskBarIcon::OnLeftButtonDClick(wxTaskBarIconEvent&)
{
    mySurfaceFrame->Show(true);
}


SurfaceFrame::SurfaceFrame(const wxString& title, bool EncryptionMode):
wxFrame(NULL, wxID_ANY, title),
Speech(new RBSpeech()),
JawsCache(new JawsCacheType()),
Protocols( new ProtocolCollection()),
Apps( new AppCollection()),
blnEncryptionMode( EncryptionMode),
blnLogging( false)
{
	// Load user options
	LoadOptions();

	// Initialise the help system
#ifdef __WINDOWS__
path myHelpPath( HelpPath());
wstrHelpFileName = HelpPath().generic_wstring();
	wstrHelpFileName.append( wstrCHMHelpFile);
pHelpController = new wxCHMHelpController;
	
	if (! pHelpController->Initialize( wstrHelpFileName))
	{
wxMessageBox( wstrHelpInitError, wstrErrorTitle, wxOK | wxICON_ERROR);	
	}
		#else
	std::wstring wstrHelpURL = PathToURL( HelpPath().generic_wstring());
// 	wstrHelpFileName = "SurfaceReader.htb";
pHelpController = new wxExtHelpController;

	// Required for wxWidgets HTML help
// wxFileSystem::AddHandler( new wxZipFSHandler);

// pHelpController = new wxHtmlHelpController;

if (! pHelpController->Initialize( HelpPath().generic_string()))
		{
wxMessageBox( wstrHelpInitError, wstrErrorTitle, wxOK | wxICON_ERROR);	
	}

// 	pHelpController->AddBook( wstrHelpFileName);
#endif

// Set up the accellerator keys
const unsigned int nKeys = 13;
wxAcceleratorEntry wxaEntries[ nKeys];

// Delete = Remove surface
wxaEntries[ 0].Set( wxACCEL_NORMAL, WXK_DELETE, ID_SurfaceRemove);
// F1 = Help contents    
wxaEntries[ 1].Set( wxACCEL_NORMAL, WXK_F1, ID_HelpContents);
// F2 = Rename surface
wxaEntries[ 2].Set( wxACCEL_NORMAL, WXK_F2, ID_SurfaceRename);
// F3 = Live mode
wxaEntries[ 3].Set( wxACCEL_NORMAL, WXK_F3, ID_SurfaceModeLive);
// F4 = Live mode
wxaEntries[ 4].Set( wxACCEL_NORMAL, WXK_F4, ID_SurfaceModeID);
// F5 = Screen reader mode
wxaEntries[ 5].Set( wxACCEL_NORMAL, WXK_F5, ID_SurfaceModeScreenReader);
// F6 = Configuration mode
wxaEntries[ 6].Set( wxACCEL_NORMAL, WXK_F6, ID_SurfaceModeConfig);
// Alt+Enter = Surface protocol properties
wxaEntries[ 7].Set( wxACCEL_ALT, WXK_RETURN, ID_SurfaceProtocolProperties);
wxaEntries[ 8].Set( wxACCEL_ALT, WXK_NUMPAD_ENTER, ID_SurfaceProtocolProperties);
// Ctrl+Enter = Surface protocol properties
wxaEntries[ 9].Set( wxACCEL_CTRL, WXK_RETURN, ID_SurfaceDisplayReport);
wxaEntries[ 10].Set( wxACCEL_CTRL, WXK_NUMPAD_ENTER, ID_SurfaceDisplayReport);
// F11 = hide to the taskbar
wxaEntries[ 11].Set( wxACCEL_NORMAL, WXK_F11, ID_Hide);
// F7 = Toggle focus between the first display element and the list box
wxaEntries[ 12].Set( wxACCEL_NORMAL, WXK_F7, ID_ToggleDisplays);

wxAcceleratorTable wxatKeys( nKeys, wxaEntries);
SetAcceleratorTable(wxatKeys);
	
 // set the frame icon
 //std::string strIcon = AppDataPath();
//append( wstrAppIconFileName);	
//SetIcon( wxICON( wstrIcon));
	
// create a menu bar
InitMenus();
	
        // Set up the rest of the UI
	myPanel = new wxPanel(this, -1);
	wxBoxSizer * hBox = new wxBoxSizer(wxHORIZONTAL);
  
wxBoxSizer * hListSizer= new wxBoxSizer(wxHORIZONTAL);
wxStaticText * lblSurfaces = new wxStaticText( this, wxID_ANY, wstrSurfacesPrompt, wxPoint( LEFT_MARGIN, TOP_MARGIN), wxSize( NORMAL_WIDTH, -1));
lbxSurfaces = new SurfaceList( this, ID_SURFACE_LIST_BOX,  &SurfacePointers, Protocols, wxPoint( SURFACE_LIST_LEFT, TOP_MARGIN), wxSize( NORMAL_WIDTH, -1));
hListSizer->Add(lblSurfaces, 0, wxEXPAND);
	hListSizer->Add( lbxSurfaces, 0, wxEXPAND);
hBox->Add( hListSizer, 0, wxEXPAND);

	  // Sizer to hold the display information
myDisplaySizer= new wxBoxSizer(wxVERTICAL);
hBox->Add( myDisplaySizer, wxEXPAND);
	SetSizer( hBox);
  hBox->Fit( this);
lbxSurfaces->SetFocus();

// We want to show what the application is up to
CreateStatusBar();

Centre();
		
// Always show the frame if any errors happen during data load or if no surfaces have yet been defined
if ((InitData() == false)
	|| (SurfacePointers.size() == 0))
{
	this->Show( true);
}

// Taskbar 
    m_taskBarIcon = new myTaskBarIcon();

// we should be able to show up to 128 characters for the tooltip on recent Windows versions
wxIcon myIcon( smile_xpm);
bool blnOK = myIcon.IsOk();

if ((blnOK == false) || (! m_taskBarIcon->SetIcon( myIcon, wstrAppTitle)))
    {
        wxMessageBox(wstrTaskbarIconError, wstrErrorTitle, wxOK | wxICON_ERROR);
    }

#if defined(__WXOSX__) && wxOSX_USE_COCOA
    m_dockIcon = new myTaskBarIcon(wxTBI_DOCK);
	
	if ( !m_dockIcon->SetIcon(myIcon))
    {
        wxMessageBox(wstrDockIconError, wstrErrorTitle, wxOK | wxICON_ERROR);
    }
#endif
}


SurfaceFrame::~SurfaceFrame()
	{
      	pHelpController->Quit();
delete pHelpController;
m_taskBarIcon->Destroy();
}


bool SurfaceFrame::GetStartSized()
{
	return myOptions.GetStartSized();
}


bool SurfaceFrame::InitData()
	{
bool blnResult = true;

// Load any previous configuration data  
			
if (LoadProtocols() == false)
			{
				blnResult = false;
			}

	if (LoadAppConfigs() == false)
	{
		blnResult = false;
	}

if (LoadSurfaces() == false)
{
	blnResult = false;
}

#ifdef __WXOSX_COCOA__   
// Load speech options for the Mac
Speech->LoadSpeechParameters();
#endif
		
// Set up the UI to reflect the loaded surfaces
UpdateMenuStates();
UpdateStatusBar();
	return blnResult;
}


void SurfaceFrame::InitMenus()
{
FileMenu = new wxMenu;
   FileNewSubMenu = new wxMenu;
   FileImportSubMenu = new wxMenu;
FileExportSubMenu = new wxMenu;
EditMenu = new wxMenu;
		    SurfaceMenu = new wxMenu;
			   SurfacePropertiesSubMenu = new wxMenu;
SurfaceModeSubMenu = new wxMenu;
OptionsMenu = new wxMenu;
				     HelpMenu = new wxMenu;
    
FileNewSubMenu->Append( ID_NewProtocol, wstrFileProtocol);
	FileNewSubMenu->Append( ID_NewAppConfig, wstrFileAppConfig);
FileMenu->Append(	ID_FileNew, wstrFileNewMenu, FileNewSubMenu);

	FileImportSubMenu->Append( ID_ImportProtocol, wstrFileProtocol);
	FileImportSubMenu->Append( ID_ImportAppConfig, wstrFileAppConfig);
FileMenu->Append(	ID_FileImport, wstrFileImportMenu, FileImportSubMenu);

FileExportSubMenu->Append( ID_ExportProtocol, wstrFileProtocol);
FileExportSubMenu->Append( ID_ExportAppConfig, wstrFileAppConfig);
FileMenu->Append(	ID_FileExport, wstrFileExportMenu, FileExportSubMenu);

FileMenu->AppendSeparator();
FileMenu->Append( ID_Hide, wstrHide);	
	FileMenu->AppendSeparator();

FileMenu->Append( wxID_EXIT);
	
EditMenu->Append( ID_EditAppConfig, wstrEditAppConfig);
EditMenu->Append( ID_EditProtocol, wstrEditProtocol);
EditMenu->Append( ID_EditCloneProtocol, wstrEditCloneProtocol);
EditMenu->Append( ID_EditCloneAppConfig, wstrEditCloneAppConfig);

SurfaceMenu->Append( ID_SurfaceAdd, wstrSurfaceAdd);
SurfaceMenu->Append( ID_SurfaceLoadAppConfig, wstrSurfaceLoadAppConfig);
SurfaceMenu->Append( ID_SurfaceDisplayReport, wstrSurfaceDisplayReport);

SurfaceMenu->AppendSeparator();
	
SurfacePropertiesSubMenu->Append(ID_SurfaceHardwareMIDIProperties, wstrSurfaceHardwareProperties);
SurfacePropertiesSubMenu ->Append( ID_SurfaceDisplayMIDIProperties, wstrSurfaceDisplayProperties);
	SurfacePropertiesSubMenu ->Append( ID_SurfaceProtocolProperties, wstrSurfaceProtocolProperties);
SurfacePropertiesSubMenu ->Append( ID_SurfaceDelayProperties, wstrSurfaceReadDisplayDelay);	
SurfaceMenu->Append(	ID_SurfaceProperties, wstrSurfacePropertiesMenu, SurfacePropertiesSubMenu);
SurfaceMenu->AppendSeparator();
	
SurfaceMenu->Append( ID_SurfaceRename, wstrRename);	
SurfaceMenu->Append( ID_SurfaceRemove, wstrRemove);	

SurfaceMenu->AppendSeparator();

SurfaceModeSubMenu ->Append( ID_SurfaceModeLive, wstrSurfaceModeLive);
SurfaceModeSubMenu ->Append( ID_SurfaceModeID, wstrSurfaceModeID);
SurfaceModeSubMenu ->Append( ID_SurfaceModeScreenReader, wstrSurfaceModeScreenReader);
SurfaceModeSubMenu ->Append( ID_SurfaceModeConfig, wstrSurfaceModeConfig);
SurfaceMenu->Append(	ID_SurfaceMode, wstrSurfaceModeMenu, SurfaceModeSubMenu);
		
OptionsMenu->AppendCheckItem( ID_VoiceMute, wstrVoiceMute, wstrVoiceMuteHelp);
		#ifdef __WXOSX_COCOA__
	OptionsMenu->Append( ID_VoiceSetup, wstrVoiceSetup);
#endif

	OptionsMenu->AppendCheckItem( ID_StartMinimised, wstrStartMinimised, wstrStartMinimisedHelp);
OptionsMenu->AppendCheckItem( ID_EnableLogging, wstrEnableLogging, wstrEnableLoggingHelp);
OptionsMenu->Append( ID_ViewLog, wstrViewLog);

HelpMenu->Append( ID_HelpContents, wstrHelpContents );	
HelpMenu->Append( ID_Help_Update_Check, wstrHelpUpdateCheck);	

	HelpMenu->Append( wxID_ABOUT);

    myMenuBar = new wxMenuBar();
myMenuBar->Append( FileMenu, wstrFileMenu);
    myMenuBar->Append( EditMenu, wstrEditMenu);
	myMenuBar->Append( SurfaceMenu, wstrSurfaceMenu);
myMenuBar->Append( OptionsMenu, wstrOptionsMenu);
myMenuBar->Append( HelpMenu, wstrHelpMenu);
SetMenuBar( myMenuBar );


// Initialise menu check values
myMenuBar->Check( ID_StartMinimised, (! myOptions.GetStartSized()));
myMenuBar->Check( ID_EnableLogging, (! blnLogging));
}


void SurfaceFrame::UpdateMenuStates()
{
	size_t nSurfaceCount = SurfacePointers.size();

	if ((nSurfaceCount == 0)
		|| (lbxSurfaces->GetSelection() < 0))
	{
		EditMenu->Enable(ID_EditAppConfig, false);
		EditMenu->Enable(ID_EditProtocol, false);
		EditMenu->Enable(ID_EditCloneProtocol, false);

		EditMenu->Enable(ID_EditCloneAppConfig, false);
		SurfaceMenu->Enable(ID_SurfaceLoadAppConfig, false);
		SurfaceMenu->Enable(ID_SurfaceProperties, false);
		SurfaceMenu->Enable(ID_SurfaceRename, false);
		SurfaceMenu->Enable(ID_SurfaceRemove, false);
		SurfaceMenu->Enable(ID_SurfaceMode, false);

		OptionsMenu->Enable(ID_ViewLog, false);
	}
	else
	{
		EditMenu->Enable(ID_EditAppConfig, true);
		EditMenu->Enable(ID_EditProtocol, true);
		EditMenu->Enable(ID_EditCloneProtocol, true);
		EditMenu->Enable(ID_EditCloneAppConfig, true);

		SurfaceMenu->Enable(ID_SurfaceLoadAppConfig, true);
		SurfaceMenu->Enable(ID_SurfaceProperties, true);
		SurfaceMenu->Enable(ID_SurfaceRename, true);
		SurfaceMenu->Enable(ID_SurfaceRemove, true);
		SurfaceMenu->Enable(ID_SurfaceMode, true);

		OptionsMenu->Enable(ID_ViewLog, true);
	}
}


void SurfaceFrame::UpdateStatusBar()
{
	    size_t nSurfaceCount = SurfacePointers.size();

if (nSurfaceCount == 0)
	{
SetStatusText( wstrStatusNoSurfaces);
}
else
{
	int nSurfaceNumber = lbxSurfaces->GetSelection();
	
	if (nSurfaceNumber > -1)
		{
			std::wstring wstrMode = SurfacePointers.at( nSurfaceNumber)->GetModeDescription( SurfacePointers.at( nSurfaceNumber)->GetWidgetMode());	
			std::string strProtocolID = SurfacePointers.at( nSurfaceNumber)->GetProtocolID();
			std::wstring wstrProtocolName = Protocols->GetProtocol( strProtocolID).GetProtocolName();
			
			std::wstring wstrAppName = SurfacePointers.at( nSurfaceNumber)->GetAppConfigID().first;
			if (wstrAppName.empty())
			{
wstrAppName = wstrDefaultAppName;
			}

// Speak the active mode for the selected surface
	DolphinReturnCode dpReturn;
	std::wstring wstrOut = lbxSurfaces->GetStringSelection().ToStdWstring();
	wstrOut.append( wstrSpace);
wstrOut.append( wstrMode);

Speech->Speak( wstrOut, true, dpReturn);

// Actually update the status bar
wstrOut = wstrMode.append( wstrSpacedDash).append( wstrProtocolName);
wstrOut.append( wstrSpacedOpenParen).append( wstrAppName).append( wstrCloseParen);
	
	SetStatusText( wstrOut);
}  // end if valid selection
	else  // There are surfaces in the list, but n item is selected
{
SetStatusText( wstrStatusNoSelectedSurface);
}  // end if selected item
}  // end if surface count > 0
}


void SurfaceFrame::ListSurfaces()
{
	      size_t nSurfaceCount = SurfacePointers.size();

for (unsigned int i = 0; (i < nSurfaceCount); i++)
	{
		lbxSurfaces->Append( SurfacePointers.at( i)->GetLabel());
			}
}


void SurfaceFrame::ClearDisplays()
{
// Remove all of the display components from the sizer
	myDisplaySizer->Clear();
// Empty the map of displays
	myDisplays.clear();
}


bool SurfaceFrame::OpenSurfacePorts( SurfacePointer pMySurface)
		{
bool blnResult = true;
std::string strErrors;
wxString strReportTitle = strPortError;
strReportTitle.append( pMySurface->GetLabel());
strReportTitle.append( strSpacedColon);
strReportTitle.append( strNewLine);

						if (pMySurface->OpenDisplayThru( pMySurface->GetDisplayInID(), pMySurface->GetDisplayOutID()) == false)
							{
// Gather error messages to form an overall error report
strErrors.append( GetPortErrorReport( pMySurface));
pMySurface->ClearPortErrors();
blnResult = false;
	}
	
	if (pMySurface->OpenHardwareThru( pMySurface->GetHardwareInID(), pMySurface->GetHardwareOutID()) == false)
												{
// Gather error messages to form an overall error report
strErrors.append( GetPortErrorReport( pMySurface));
pMySurface->ClearPortErrors();
blnResult = false;
	}
		
if (blnResult == false)
	{
		wxMessageBox( strReportTitle.append( strErrors), wstrErrorTitle, wxOK | wxICON_ERROR);	
			}

			return blnResult;
}


		void SurfaceFrame::CloseSurfacePorts(SurfacePointer pMySurface)
		{
			pMySurface->CloseHardwareThru();
		pMySurface->CloseDisplayThru();
			}
		

		void SurfaceFrame::OpenAllSurfaces()
		{
			if (SurfacePointers.size() > 0)
				{
for (unsigned int i =0; i < SurfacePointers.size(); i++)
			{
if (OpenSurfacePorts( SurfacePointers.at( i)) == false)
{
// Gather error messages to form an overall error report
wxString strReportTitle = strPortError;
strReportTitle.append( SurfacePointers.at( i)->GetLabel());
strReportTitle.append( strSpacedColon);
strReportTitle.append( strNewLine);

std::string strErrors = GetPortErrorReport( SurfacePointers.at( i));
SurfacePointers.at( i)->ClearPortErrors();

wxMessageBox( strReportTitle.append( strErrors), wstrErrorTitle, wxOK | wxICON_ERROR);	
}  // end if errors
else   // Enable processing
{
	SurfacePointers.at( i)->ResumeDisplayProcessing();
	SurfacePointers.at( i)->ResumeHardwareProcessing();
}
}  // end for		
			}  // end if number of surfaces > 0
		}


		std::string SurfaceFrame::GetPortErrorReport(SurfacePointer pMySurface)
{
	std::string strOut;
	std::vector <std::string> vErrors = pMySurface->GetPortErrors();

	for (unsigned int i = 0; i < vErrors.size(); i++)
	{
		strOut.append( vErrors.at( i));
		strOut.append( strNewLine);
	}

	return strOut;
}


			void SurfaceFrame::CloseAllSurfaces()
		{
			if (SurfacePointers.size() > 0)
				{
					for (unsigned int i =0; i < SurfacePointers.size(); i++)
			{
				SurfacePointers.at( i)->StopDisplayProcessing();
				SurfacePointers.at( i)->StopHardwareProcessing();
				CloseSurfacePorts( SurfacePointers.at( i));
	}
			}
		}


		bool SurfaceFrame::IsSurfaceNameUnique( std::wstring wstrName)
{
if (lbxSurfaces->FindString( wstrName) < 0)
{
	return true;
}
else
{
	return false;
}
}


void SurfaceFrame::Shutdown()
{
SaveSurfaces();
SaveProtocols();
SaveAppConfigs();

// Clear out the display elements
ClearDisplays();
	
// Destroy our surface pointers
SurfacePointers.clear();
	
// Release the speech mechanism
#ifdef __WINDOWS__
	Speech->UnloadAPI();
#endif

	// Close the frame
Close( true);

#ifdef __WINDOWS__
	// VLDReportLeaks();
#endif
	}


bool SurfaceFrame::DeleteSurfaceFiles()
	{
		    path myPath( SurfacePath());
	
  try
  {
    if (! exists( myPath))    // does the path actually exist?
    {
      return false;
	}  // if exists
	
	if (! is_directory( myPath))      // myPath must be a directory?
      {
	  return false;
	}  // if directory
	}  // end try
  catch (const filesystem_error &error)
  {
    	  wxMessageBox( error.what(), wstrErrorTitle, wxOK | wxICON_ERROR);
		  return false;
  }
          
// Directory exists so process the contents
  directory_iterator end_itr; // Default ctor yields past-the-end

for (directory_iterator i( myPath); i != end_itr; ++i )
{
    // Skip if not a file
    if (! is_regular_file( i->status() ) ) 
		{
			continue;
	}

// Is this one of our desired files?
	if (i->path().filename().extension() == wstrSurfaceFileExtension)
	{
	// File matches, so delete it
		try
		{
			boost::filesystem::remove(i->path());
		}
				catch (const filesystem_error &error)
					{
						wxMessageBox(error.what(), wstrErrorTitle, wxOK | wxICON_ERROR);
												return false;
					}
				}  // end if match
}  // end for
return true;
}
	

void SurfaceFrame::SaveSurfaces()
{
path myPath( SurfacePath());
	
  try
  {
    if (! exists( myPath))    // does the path actually exist?
    {
      return;
	}  // if exists
	
	if (! is_directory( myPath))      // myPath must be a directory?
      {
	  return;
	}  // if directory
	}  // end try
  catch (const filesystem_error &error)
  {
    	  wxMessageBox( error.what(), wstrErrorTitle, wxOK | wxICON_ERROR);
		  return;
  }
          
DeleteSurfaceFiles();

// SurfaceVectorType::iterator it;
for (unsigned int i = 0; i < SurfacePointers.size(); i++)
{
	std::wstring wstrFileName = SurfacePointers.at( i)->GetSurfaceName();
	wstrFileName.append( wstrSurfaceFileExtension);

	myPath = SurfacePath();
myPath /= wstrFileName;

SurfaceParameters myParameters;
myParameters.SetSurfaceName( SurfacePointers.at( i)->GetSurfaceName());
myParameters.SetProtocolID( SurfacePointers.at( i)->GetProtocolID());
myParameters.SetProtocolName( SurfacePointers.at( i)->GetProtocolName());
myParameters.SetHardwareInID( SurfacePointers.at( i)->GetHardwareInID());
myParameters.SetHardwareInName( SurfacePointers.at( i)->GetHardwareInName());
myParameters.SetHardwareOutID( SurfacePointers.at( i)->GetHardwareOutID());
myParameters.SetHardwareOutName( SurfacePointers.at( i)->GetHardwareOutName());
myParameters.SetDisplayInID( SurfacePointers.at( i)->GetDisplayInID());
myParameters.SetDisplayInName( SurfacePointers.at( i)->GetDisplayInName());
myParameters.SetDisplayOutID( SurfacePointers.at( i)->GetDisplayOutID());
myParameters.SetDisplayOutName( SurfacePointers.at( i)->GetDisplayOutName());
myParameters.SetAppConfigID( SurfacePointers.at( i)->GetAppConfigID());
myParameters.SetAppConfigName( SurfacePointers.at( i)->GetAppConfigName());
myParameters.SetSurfaceMode( SurfacePointers.at( i)->GetWidgetMode());
myParameters.SetReadDisplayDelay( SurfacePointers.at( i)->GetReadDisplayDelay());
SaveData <SurfaceParameters> (myParameters, myPath, false);
}  // end for
}


bool SurfaceFrame::LoadSurfaces()
{
	bool blnResult = true;
path myPath( SurfacePath());
std::vector <std::string> vFailedFiles;

try
  {
    if (! exists( myPath))    // does the path actually exist?
    {
      return false;
	}  // if exists
	
	if (! is_directory( myPath))      // myPath must be a directory?
      {
	  return false;
	}  // if directory
	}  // end try
  catch (const filesystem_error &error)
  {
    	  wxMessageBox( error.what(), wstrErrorTitle, wxOK | wxICON_ERROR);
return false;
  }
          
// Directory exists so process the contents
  directory_iterator end_itr; // Default ctor yields past-the-end

for (directory_iterator i( myPath); i != end_itr; ++i )
{
    // Skip if not a file
    if (! is_regular_file( i->status() ) ) 
		{
			continue;
	}

// Is this one of our desired files?
	if (i->path().filename().extension() == wstrSurfaceFileExtension)
	{
	// File matches, so try to load it
		bool blnSuccess = true;
		try
			{
			SurfaceParameters myParameters{ LoadData <SurfaceParameters>(i->path(), false) };
			bool blnProtocolExists = Protocols->ProtocolExists(myParameters.GetProtocolID());

			if (blnProtocolExists)
			{
				// Look up the name strings for the loaded AppConfig and Protocol
				try
				{
					myParameters.SetAppConfigName(Apps->GetAppConfig(myParameters.GetAppConfigID().first, myParameters.GetAppConfigID().second).GetAppName());
				}
				catch (...)
				{
					myParameters.SetAppConfigName(wstrEmpty);
				}

				try
				{
					myParameters.SetProtocolName(Protocols->GetProtocol(myParameters.GetProtocolID()).GetProtocolName());
				}
				catch (...)
				{
					myParameters.SetProtocolName(wstrEmpty);
				}

				// Add the new surface to the master vector
				ActiveProduct myProduct;
				DolphinProduct dpProduct;
				Speech->GetActiveProduct(myProduct, dpProduct);

				SurfacePointer pSurface(new MIDISurface(this, myProduct, &myParameters, Protocols, Apps));
				SurfacePointers.push_back(pSurface);

				// Activate the new surface
				if (OpenSurfacePorts(SurfacePointers.back()) == false)
				{
					blnResult = false;
				}

				// Update the surface list
				lbxSurfaces->Append(SurfacePointers.at(SurfacePointers.size() - 1)->GetSurfaceName());
			}  // end if protocol exists
		}
		catch (boost::archive::archive_exception & myException)
		{
blnSuccess = false;
blnResult = false;
vFailedFiles.push_back( i->path().filename().generic_string());
		}
}  // end if match
}  // end for
    
// Display the names of any files that failed to load
if (vFailedFiles.size() > 0)
{
std::wstring wstrErrorText = wstrFailedFilesError;

for( unsigned int i = 0; i < vFailedFiles.size(); i++)
	{
		std::string strFileName = vFailedFiles.at( i);
		std::wstring wstrFileName( strFileName.begin(), strFileName.end());
		wstrErrorText.append( wstrFileName);
	wstrErrorText.append( wstrNewLine);
}  // end for

wxMessageBox( wstrErrorText, wstrErrorTitle, wxOK | wxICON_ERROR);
					}  // end if vector not empty

	// Make sure that the correct menu options are enabled
	UpdateMenuStates();
	// Update the status bar for testing
UpdateStatusBar();
return blnResult;
}


bool SurfaceFrame::DeleteAppConfigFiles()
	{
			path myPath( AppConfigPath());
	
  try
  {
    if (! exists( myPath))    // does the path actually exist?
    {
      return false;
	}  // if exists
	
	if (! is_directory( myPath))      // myPath must be a directory?
      {
	  return false;
	}  // if directory
	}  // end try
  catch (const filesystem_error &error)
  {
    	  wxMessageBox( error.what(), wstrErrorTitle, wxOK | wxICON_ERROR);
		  return false;
  }
          
// Directory exists so process the contents
  directory_iterator end_itr; // Default ctor yields past-the-end

for (directory_iterator i( myPath); i != end_itr; ++i )
{
    // Skip if not a file
    if (! is_regular_file( i->status() ) ) 
		{
			continue;
	}

// Is this one of our desired files?
	if ((i->path().filename().extension() == wstrUserAppConfigFileExtension.c_str())
		|| (i->path().filename().extension() == wstrPROTECTED_APP_CONFIG_EXTENSION.c_str()))
	{
				// File matches, so delete it
		try
		{
			boost::filesystem::remove(i->path());
		}
		catch (const filesystem_error &error)
		{
			wxMessageBox(error.what(), wstrErrorTitle, wxOK | wxICON_ERROR);
			return false;
			}
	}  // end if match
}  // end for
return true;
}


void SurfaceFrame::SaveAppConfigs()
{
path myPath( AppConfigPath());
	
  try
  {
    if (! exists( myPath))    // does the path actually exist?
    {
      return;
	}  // if exists
	
	if (! is_directory( myPath))      // myPath must be a directory?
      {
	  return;
	}  // if directory
	  }  // end try
  catch (const filesystem_error &error)
  {
    	  wxMessageBox( error.what(), wstrErrorTitle, wxOK | wxICON_ERROR);
		  return;
  }
          
// DeleteAppConfigFiles();

		for ( int i = 0; i < Apps->Count(); i++)
{
		std::wstring wstrFileName= Apps->GetAppConfigItem(i).GetAppName();
			wstrFileName.append( wstrDash).append(Protocols->GetProtocol( Apps->GetAppConfigItem(i).GetProtocolID()).GetProtocolName());
std::string strProtocolID = Apps->GetAppConfigItem( i).GetProtocolID();
		bool blnEncrypted = Protocols->GetProtocol(strProtocolID).IsEncrypted();
	AppConfig myAppConfig;
	
		if (blnEncrypted)
	{
		wstrFileName.append( wstrPROTECTED_APP_CONFIG_EXTENSION);
	}
	else
		{
			wstrFileName.append(wstrUserAppConfigFileExtension);
}	
	
myAppConfig = Apps->GetAppConfigItem( i);
myPath = AppConfigPath();
myPath /= wstrFileName;

if (blnEncrypted)
{
	SaveData <AppConfig> ( myAppConfig, myPath, true);
}
else
{
SaveData <AppConfig> ( myAppConfig, myPath, false);
}
		}  // end for
}


bool SurfaceFrame::LoadAppConfigs()
{
bool blnResult = true;
path myPath( AppConfigPath());
std::vector <std::string> vFailedFiles;

try
  {
    if (! exists( myPath))    // does the path actually exist?
    {
      return false;
	}  // if exists
	
	if (! is_directory( myPath))      // myPath must be a directory?
      {
	  return false;
	}  // if directory
	}  // end try
  catch (const filesystem_error &error)
  {
    	  wxMessageBox( error.what(), wstrErrorTitle, wxOK | wxICON_ERROR);
return false;
  }
          
// Directory exists so process the contents
  directory_iterator end_itr; // Default ctor yields past-the-end

for (directory_iterator i( myPath); i != end_itr; ++i )
{
    // Skip if not a file
    if (! is_regular_file( i->status() ) ) 
		{
			continue;
	}

	if (i->path().filename().extension() == wstrUserAppConfigFileExtension.c_str())
	{
		try
			{
					AppConfig myAppConfig{ LoadData <AppConfig>(i->path(), false) };
					// Add the new configuration to the collection
					std::string strProtocolID = myAppConfig.GetProtocolID();
					std::wstring wstrProtocolID(strProtocolID.begin(), strProtocolID.end());
					Apps->Add(myAppConfig.GetAppName(), wstrProtocolID, myAppConfig);
}
				catch (boost::archive::archive_exception & myException)
		{
blnResult = false;
vFailedFiles.push_back( i->path().filename().generic_string());
		}
	}  // if user file
	else
	{
	// Is this a copy-protected file?
	if (i->path().filename().extension() == wstrPROTECTED_APP_CONFIG_EXTENSION.c_str())
	{
	// File matches, so try to load it
						try
			{
							AppConfig myAppConfig{ LoadData <AppConfig>(i->path(), true) };
							// Add the new configuration to the collection
							std::string strProtocolID = myAppConfig.GetProtocolID();
							std::wstring wstrProtocolID(strProtocolID.begin(), strProtocolID.end());
							Apps->Add(myAppConfig.GetAppName(), wstrProtocolID, myAppConfig);
						}
						catch (boost::archive::archive_exception & myException)
		{
blnResult = false;
vFailedFiles.push_back( i->path().filename().generic_string());
		}
					}  // end if copy-protected
	}  // end else: if user file
}  // end for
    
// Display the names of any files that failed to load
if (vFailedFiles.size() > 0)
{
std::wstring wstrErrorText = wstrFailedFilesError;

for( unsigned int i = 0; i < vFailedFiles.size(); i++)
	{
		std::string strFileName = vFailedFiles.at( i);
		std::wstring wstrFileName( strFileName.begin(), strFileName.end());
		wstrErrorText.append( wstrFileName);
	wstrErrorText.append( wstrNewLine);
}  // end for

wxMessageBox( wstrErrorText, wstrErrorTitle, wxOK | wxICON_ERROR);
					}  // end if vector not empty
	return blnResult;
}


bool SurfaceFrame::DeleteProtocolFiles()
	{
		path myPath( ProtocolPath());
	
  try
  {
    if (! exists( myPath))    // does the path actually exist?
    {
      return false;
	}  // if exists
	
	if (! is_directory( myPath))      // myPath must be a directory?
      {
	  return false;
	}  // if directory
	}  // end try
  catch (const filesystem_error &error)
  {
    	  wxMessageBox( error.what(), wstrErrorTitle, wxOK | wxICON_ERROR);
		  return false;
  }
          
// Directory exists so process the contents
  directory_iterator end_itr; // Default ctor yields past-the-end

for (directory_iterator i( myPath); i != end_itr; ++i )
{
    // Skip if not a file
    if (! is_regular_file( i->status() ) ) 
		{
			continue;
	}

// Is this one of our desired files?
	if ((i->path().filename().extension() == wstrPROTECTED_PROTOCOL_EXTENSION.c_str())
		|| (i->path().filename().extension() == wstrUSER_PROTOCOL_EXTENSION .c_str()))
	{
	// File matches, so delete it
		try
		{
			boost::filesystem::remove(i->path());
		}
				catch (const filesystem_error &error)
			{
				wxMessageBox(error.what(), wstrErrorTitle, wxOK | wxICON_ERROR);
				return false;
							}
		}  // end if match
}  // end for
	return true;
	}


void SurfaceFrame::SaveProtocols()
{
path myPath( ProtocolPath());
	
  try
  {
    if (! exists( myPath))    // does the path actually exist?
    {
      return;
	}  // if exists
	
	if (! is_directory( myPath))      // myPath must be a directory?
      {
	  return;
	}  // if directory
	}  // end try
  catch (const filesystem_error &error)
  {
    	  wxMessageBox( error.what(), wstrErrorTitle, wxOK | wxICON_ERROR);
		  return;
  }
          
// DeleteProtocolFiles();

for (int i = 0; i < Protocols->count(); i++)
{
		SurfaceProtocol myProtocol = Protocols->GetProtocol( i);

		std::wstring wstrFileName = myProtocol.GetProtocolName(); 
					
					if (myProtocol.IsEncrypted())
	{
		wstrFileName.append( wstrPROTECTED_PROTOCOL_EXTENSION);
	}
	else
		{
			wstrFileName.append(wstrUSER_PROTOCOL_EXTENSION);
}	
	
		myPath = ProtocolPath();
				myPath /= wstrFileName;

if (myProtocol.IsEncrypted())
{
	SaveData <SurfaceProtocol> (myProtocol, myPath, true);
}
else
{
SaveData <SurfaceProtocol> (myProtocol, myPath, false);
}
}  // end for
}


bool SurfaceFrame::LoadProtocols()
{
	bool blnResult = true;
			path myPath( ProtocolPath().c_str());
std::vector <std::string> vFailedFiles;

try
  {
    if (! exists( myPath))    // does the path actually exist?
    {
      return false;
	}  // if exists
	
	if (! is_directory( myPath))      // myPath must be a directory?
      {
	  return false;
	}  // if directory
	}  // end try
  catch (const filesystem_error &error)
  {
    	  wxMessageBox( error.what(), wstrErrorTitle, wxOK | wxICON_ERROR);
return false;
  }
          
// Directory exists so process the contents
  directory_iterator end_itr; // Default ctor yields past-the-end

for (directory_iterator i( myPath); i != end_itr; ++i )
{
    // Skip if not a file
    if (! is_regular_file( i->status() ) ) 
		{
			continue;
	}

bool blnSuccess = true;
		// Is this a user-created file?
	if (i->path().filename().extension() == wstrUSER_PROTOCOL_EXTENSION.c_str())
	{
	// File matches, so try to load it
				try
			{
					SurfaceProtocol myProtocol{ LoadData <SurfaceProtocol>(i->path(), false) };
					// Add the new protocol to the collection
					int nCount = Protocols->count();
					Protocols->Add(myProtocol);
				}
				catch (boost::archive::archive_exception & myException)
				{
					wxMessageBox(myException.what(), wstrErrorTitle, wxOK | wxICON_ERROR);
				blnResult = false;
vFailedFiles.push_back( i->path().filename().generic_string());
		}
		}  // if user file
	else
	{
	// Is this a copy-protected file?
		if (i->path().filename().extension() == wstrPROTECTED_PROTOCOL_EXTENSION.c_str())
	{
	// File matches, so try to load it
	try
			{
		SurfaceProtocol myProtocol{ LoadData <SurfaceProtocol>(i->path(), true) };
		// Add the new protocol to the collection
		int nCount = Protocols->count();
		Protocols->Add(myProtocol);
	}
	catch (RBException & myException)
	{
		wxMessageBox(myException.what(), wstrErrorTitle, wxOK | wxICON_ERROR);
	blnResult = false;
vFailedFiles.push_back( i->path().filename().generic_string());
		}  // end catch
		}  // end if copy-protected match
	}  // end else: if user protocol
}  // end for
    
// Display the names of any files that failed to load
if (vFailedFiles.size() > 0)
{
std::wstring wstrErrorText = wstrFailedFilesError;

for( unsigned int i = 0; i < vFailedFiles.size(); i++)
	{
		std::string strFileName = vFailedFiles.at( i);
		std::wstring wstrFileName( strFileName.begin(), strFileName.end());
		wstrErrorText.append( wstrFileName);
	wstrErrorText.append( wstrNewLine);
}  // end for

wxMessageBox( wstrErrorText, wstrErrorTitle, wxOK | wxICON_ERROR);
					}  // end if vector not empty
	return blnResult;
}


void SurfaceFrame::AddDisplay( std::wstring wstrDisplayLabel)
{}


bool SurfaceFrame::CopyDir( boost::filesystem::path const & source, boost::filesystem::path const & destination)
{
    namespace fs = boost::filesystem;
    try
    {
        // Check whether the function call is valid
        if ((! fs::exists(source)) 
			||  (!fs::is_directory(source)))
{
            std::string strErr = "Source directory ";
			strErr.append( source.string());
			strErr.append( " does not exist or is not a directory.");
                        wxMessageBox( strErr, wstrErrorTitle, wxOK | wxICON_ERROR);	
			return false;
        }
        
if(fs::exists( destination))
        {
            std::string strErr = "Destination directory ";
			strErr.append( destination.string());
			strErr.append( " already exists.");
			wxMessageBox( strErr, wstrErrorTitle, wxOK | wxICON_ERROR);	
return false;
        }

// Create the destination directory
        if(!fs::create_directory(destination))
        {
            std::string strErr = "Unable to create destination directory";
			strErr.append( destination.string());
			wxMessageBox( strErr, wstrErrorTitle, wxOK | wxICON_ERROR);	
return false;
        }
    }
    catch(fs::filesystem_error const & e)
    {
        std::string strErr = e.what();
		wxMessageBox( strErr, wstrErrorTitle, wxOK | wxICON_ERROR);	
return false;
    }
    // Iterate through the source directory
    for(
        fs::directory_iterator file(source);
        file != fs::directory_iterator(); ++file
    )
    {
        try
        {
            fs::path current(file->path());
            if(fs::is_directory(current))
            {
                // Found directory: Recursion
                if(! CopyDir( current, destination / current.filename()))
                {
                    return false;
                }
            }
            else
            {
                // Found file: Copy
                fs::copy_file( current, destination / current.filename());
            }
        }
        catch(fs::filesystem_error const & e)
        {
            std::string strErr = e.what();
        	wxMessageBox( strErr, wstrErrorTitle, wxOK | wxICON_ERROR);	
}
    }
    return true;
}


void SurfaceFrame::ShowLog( boost::filesystem::path myPath, bool blnIsEncrypted)
{
	std::ifstream myFile( myPath.generic_string().c_str());
	std::string strContents;

	if (myFile.is_open())
	{
		while (myFile.good())
		{
			std::string strLine;
			getline(myFile,strLine);
			
			if (blnIsEncrypted)
				{
					//strContents.append( Decrypt( strLine));
					strContents.append( strLine);
			}
			else
			{
			strContents.append( strLine);
			}
		}
		myFile.close();

		std::wstring wstrTitle = wstrLogTitle;
		wstrTitle.append( myPath.filename().generic_wstring());
		wxMessageBox( strContents, wstrTitle, wxOK | wxICON_INFORMATION );	
	}
	else
	{
		std::wstring wstrError = wstrBadLogError;
		wstrError.append( myPath.generic_wstring());
		wxMessageBox( wstrError, wstrErrorTitle, wxOK | wxICON_ERROR);	
	}
}
 

// Event handlers

void SurfaceFrame::OnNewProtocol(wxCommandEvent& event)
{
	boost::shared_ptr <SurfaceProtocol> pProtocol( new SurfaceProtocol());
	ProtocolWizard myProtocolWizard( this, true, pProtocol, blnEncryptionMode, Protocols, pHelpController);

if (myProtocolWizard.RunWizard( myProtocolWizard.GetSurfacePage()))
{
	if (myProtocolWizard.GetSurfacePage()->TransferDataFromWindow())
{
		// Check that the new protocol has a unique name
	if (Protocols->IsProtocolNameUnique( pProtocol->GetProtocolName()) == false)
	{
wxMessageBox( wstrDuplicatedProtocolNameError, wstrErrorTitle, wxOK | wxICON_ERROR);
					}
	else
	{
		// Add the new protocol to the protocol collection
Protocols->Add( *pProtocol);
// Save the updated protocol collection
SaveProtocols();

// Add a new Surface to the Surface vector
ActiveProduct myProduct;
DolphinProduct dpProduct;
Speech->GetActiveProduct(myProduct, dpProduct);

SurfacePointer pSurface(new MIDISurface(this, myProduct, &myProtocolWizard.GetSurfaceParameters(), Protocols, Apps));
SurfacePointers.push_back(pSurface);

// Activate the new surface
bool blnResult = true;
if (OpenSurfacePorts(SurfacePointers.back()) == false)
{
	blnResult = false;
}

		// Update the surface list
lbxSurfaces->Append( SurfacePointers.back()->GetSurfaceName());

	// Make sure that the correct menu options are enabled
	UpdateMenuStates();
// Update the status bar for testing
UpdateStatusBar();

		// Activate the new surface
OpenSurfacePorts( SurfacePointers.back());
	SaveSurfaces();
	}  // end if protocol name is unique
	}   // if OK button
else  // wizard cancelled, so remove the surface we were working on
{
	if ( SurfacePointers.size() > 0)
    {
		SurfacePointers.pop_back();
}
}  // end if TransferDataFromWindow
}  // if wizard completed

// Tidy up
myProtocolWizard.Destroy();
}


void SurfaceFrame::OnNewAppConfig(wxCommandEvent& event)
{
// Find the protocol ID for the highlighted surface
	int nMySelection = lbxSurfaces->GetSelection();
	
	if (nMySelection < 0)   // No selection in list box
{
	return;
}

	std::wstring wstrSurfaceName = lbxSurfaces->GetStringSelection().ToStdWstring();
std::string strProtocolID;
bool blnNoSurfaces = (SurfacePointers.size() == 0);

if (blnNoSurfaces == true)
{
                	return;
}

strProtocolID = SurfacePointers.at( nMySelection)->GetProtocolID();
		// Get the name of the application that this configuration is designed to work with
		std::wstring wstrAppName;
		
		RBInputBox * AppNameDlg = new RBInputBox( wstrAppNamePrompt, wstrAppName);
		
		if (AppNameDlg ->ShowModal() == wxID_OK)
		{
			wstrAppName = AppNameDlg ->GetText();
		}
		else
		{
return;
		}

// Tidy up
		AppNameDlg->Destroy();

		if (wstrAppName.empty())
		{
			wxMessageBox( wstrNoAppNameError, wstrErrorTitle, wxOK | wxICON_ERROR);
			return;
		}

			// Check to see if there's already a matching configuration
std::map <std::pair <std::wstring, std::string>, AppConfig>::iterator it;
std::pair <std::wstring, std::wstring> myPair;
std::wstring wstrEmpty;
wstrEmpty.clear();

myPair = std::make_pair( wstrEmpty, wstrEmpty);
std::wstring wstrProtocolID( strProtocolID.begin(), strProtocolID.end());
myPair = Apps->FindAppConfig( wstrAppName, wstrProtocolID);

if (myPair.first.empty() == false)
{
std::wstring wstrMessage = wstrAppConfigOverwriteCheck;
wstrMessage.append( wstrAppName);		
		
	if (		wxMessageBox( wstrMessage,                wstrAppTitle, wxYES_NO | wxNO_DEFAULT | wxICON_QUESTION) == wxNO) 
{
return;
	}
}

	// Create a new configuration
	AppConfig myAppConfig;
	myAppConfig.SetAppName( wstrAppName);
myAppConfig.SetProtocolID( strProtocolID);
boost::shared_ptr <AppConfig> pAppConfig( new AppConfig (myAppConfig));

// Populate the control structure from the defaults
	SurfaceProtocol myProtocol = Protocols->GetProtocol( strProtocolID);
boost::shared_ptr <SurfaceProtocol> pProtocol( new SurfaceProtocol (myProtocol));

pAppConfig->SetHardwareControls( pProtocol->GetHardwareControls());
pAppConfig->SetDisplays( pProtocol->GetDisplays());

// Allow the user to define the desired behaviour for this application
AppConfigWizard myAppConfigWizard( this, true, pAppConfig, pProtocol, Speech, JawsCache, pHelpController);
			
if (myAppConfigWizard.RunWizard( myAppConfigWizard.GetActionsPage()))
{
	pAppConfig = myAppConfigWizard.GetAppConfig();
	
	// Add the updated configuration to the map of available configurations
std::string strProtocolID = pAppConfig->GetProtocolID();
std::wstring wstrProtocolID( strProtocolID.begin(), strProtocolID.end()); ;
	Apps->Add( pAppConfig->GetAppName(), wstrProtocolID, * pAppConfig);
SaveAppConfigs();
	LoadAppConfigs();
}

myAppConfigWizard.Destroy();
}


void SurfaceFrame::OnImportProtocol( wxCommandEvent& event)
{
   wxFileDialog myFileImportDlg( this, wstrImportProtocolPrompt, wxEmptyString, wxEmptyString, PROTOCOL_FILE_TEMPLATE);
   myFileImportDlg.CenterOnParent();

        if (myFileImportDlg.ShowModal() == wxID_OK)
    {
path mySourcePath( myFileImportDlg.GetPath());
	  		 path myTargetPath( ProtocolPath());
		  myTargetPath /= myFileImportDlg.GetFilename().ToStdWstring();

if ((exists( myTargetPath))    
&& (! is_directory( myTargetPath)))
      {
		  std::string strErr = myTargetPath.generic_string();
		  strErr.append( strOverwritePrompt);

		  if ( wxMessageBox( strErr, wstrAppTitle, wxYES_NO | wxNO_DEFAULT | wxICON_QUESTION) == wxNO)
{
return;
	}
}

		  try
	{
		copy_file( mySourcePath, myTargetPath, copy_option::overwrite_if_exists);
}
		catch( ...)
		{
		std::string strErr = strCopyError;
		strErr.append( mySourcePath.generic_string());
		strErr.append( " to ");
		strErr.append( myTargetPath.generic_string());
			wxMessageBox( strErr, wstrErrorTitle, wxOK | wxICON_ERROR);	
}
		}
}


void SurfaceFrame::OnImportAppConfig( wxCommandEvent& event)
{
   wxFileDialog myFileImportDlg( this, wstrImportAppConfigPrompt, wxEmptyString, wxEmptyString, APP_CONFIG_FILE_TEMPLATE);
   myFileImportDlg.CenterOnParent();

        if (myFileImportDlg.ShowModal() == wxID_OK)
    {
path mySourcePath( myFileImportDlg.GetPath());
	  		 path myTargetPath( AppConfigPath());
		  myTargetPath /= myFileImportDlg.GetFilename().ToStdWstring();

if ((exists( myTargetPath))    
&& (! is_directory( myTargetPath)))
      {
		  std::string strErr = myTargetPath.generic_string();
		  strErr.append( strOverwritePrompt);
		  
		  if (		wxMessageBox( strErr,                wstrAppTitle, wxYES_NO | wxNO_DEFAULT | wxICON_QUESTION) == wxNO)
{
return;
	}
}

		  try
	{
		copy_file( mySourcePath, myTargetPath, copy_option::overwrite_if_exists);
}
		catch( ...)
		{
		std::string strErr = strCopyError;
		strErr.append( mySourcePath.generic_string());
		strErr.append( " to ");
		strErr.append( myTargetPath.generic_string());
			wxMessageBox( strErr, wstrErrorTitle, wxOK | wxICON_ERROR);	
}
		}
}


void SurfaceFrame::OnExportProtocol( wxCommandEvent& event)
{
path mySourcePath( ProtocolPath());
wxDirDialog myFileExportDlg(this, wstrExportProtocolPrompt, mySourcePath.generic_string(), wxDD_DEFAULT_STYLE | wxDD_DIR_MUST_EXIST);
myFileExportDlg.CenterOnParent();

        if (myFileExportDlg.ShowModal() == wxID_OK)
    {
		path myTargetPath( myFileExportDlg.GetPath());

if ((exists( myTargetPath))    
&& (is_directory( myTargetPath)))
        {
myTargetPath /= wstrProtocolDir;

try
	{
		CopyDir( mySourcePath, myTargetPath);
}
		catch( ...)
		{
		std::string strErr = strCopyError;
		strErr.append( mySourcePath.generic_string());
		strErr.append( " to ");
		strErr.append( myTargetPath.generic_string());
			wxMessageBox( strErr, wstrErrorTitle, wxOK | wxICON_ERROR);
}  // end try/catch
		}  // if folder exists
}  // if dialog returns OK
}


void SurfaceFrame::OnExportAppConfig( wxCommandEvent& event)
{
path mySourcePath( AppConfigPath());
wxDirDialog myFileExportDlg(this, wstrExportProtocolPrompt, mySourcePath.generic_string(), wxDD_DEFAULT_STYLE | wxDD_DIR_MUST_EXIST);
myFileExportDlg.CenterOnParent();

        if (myFileExportDlg.ShowModal() == wxID_OK)
    {
		path myTargetPath( myFileExportDlg.GetPath());

if ((exists( myTargetPath))    
&& (is_directory( myTargetPath)))
        {
myTargetPath /= wstrAppConfigDir;

try
	{
		CopyDir( mySourcePath, myTargetPath);
}
		catch( ...)
		{
		std::string strErr = strCopyError;
		strErr.append( mySourcePath.generic_string());
		strErr.append( " to ");
		strErr.append( myTargetPath.generic_string());
			wxMessageBox( strErr, wstrErrorTitle, wxOK | wxICON_ERROR);	
}  // end try/catch
		}  // if folder exists
}  // if dialog returns OK
}


void SurfaceFrame::OnHide( wxCommandEvent& event)
{
    Show(false);
}


void SurfaceFrame::OnQuit(wxCommandEvent& WXUNUSED(event))
{
	this->Shutdown();
}


void SurfaceFrame::OnEditAppConfig(wxCommandEvent& event )
{
int nMySelection = lbxSurfaces->GetSelection();
	
if (nMySelection < 0)   // No selection in list box
{
	return;
}

	// Find the protocol ID for the highlighted surface
	std::wstring wstrSurfaceName; 
	std::string strProtocolID;

	try
	{
		strProtocolID = SurfacePointers.at( nMySelection)->GetProtocolID();
	}
	catch (RBException &myException)
	{
#ifdef __WINDOWS__ 
		OutputDebugString(myException.what());
#endif

		wxMessageBox(myException.what(), wstrErrorTitle, wxOK | wxICON_ERROR);
		return;
	}  // end catch

	std::pair <std::wstring, std::wstring> myAppConfigID;
	
	try
	{
				// Get the ID of the loaded AppConfig
		myAppConfigID = SurfacePointers.at( nMySelection)->GetAppConfigID();
}
catch( RBException &myException)
{
			#ifdef __WINDOWS__ 
// OutputDebugString( myException.what());
wxMessageBox(myException.what(), wstrErrorTitle, wxOK | wxICON_ERROR);
					#endif

wxMessageBox( wstrNoAppConfigError, wstrErrorTitle, wxOK | wxICON_ERROR);
return;
}  // end catch

// Get the appropriate configuration
AppConfig myAppConfig;

	try
	 {
		 myAppConfig = Apps->GetAppConfig( myAppConfigID.first, myAppConfigID.second); 
 }
 catch( RBException &myException)
{
			#ifdef __WINDOWS__ 
OutputDebugString( myException.what());
					#endif

wxMessageBox( wstrNoAppConfigError, wstrErrorTitle, wxOK | wxICON_ERROR);
return;
}  // end catch

boost::shared_ptr <AppConfig> pAppConfig( new AppConfig(myAppConfig));
boost::shared_ptr <SurfaceProtocol> pProtocol(new SurfaceProtocol(Protocols->GetProtocol(strProtocolID)));

	 AppConfigWizard myAppConfigWizard( this, true, pAppConfig, pProtocol, Speech, JawsCache, pHelpController);
			
if (myAppConfigWizard.RunWizard( myAppConfigWizard.GetActionsPage()))
{
	pAppConfig = myAppConfigWizard.GetAppConfig();
	
	// Add the updated configuration to the map of available configurations
std::string strProtocolID = pAppConfig->GetProtocolID();
std::wstring wstrProtocolID( strProtocolID.begin(), strProtocolID.end());
	Apps->Add( pAppConfig->GetAppName(), wstrProtocolID, *pAppConfig);
	SaveAppConfigs();
}

// Tidy up
myAppConfigWizard.Destroy();
}


void SurfaceFrame::OnEditProtocol(wxCommandEvent& event )
{
int nMySelection = lbxSurfaces->GetSelection();
	
if (nMySelection < 0)   // No selection in list box
{
return;
}

		int nMIDIInID; 
	std::string strProtocolID; 
bool blnProtocolExists = true;

CloseSurfacePorts( SurfacePointers.at( nMySelection));

	// Get the MIDI input port being used for hardware monitoring
nMIDIInID = SurfacePointers.at( nMySelection)->GetHardwareInID(); 
				// Get the protocol and check that it's editable (i.e. not deemed to be encrypted
	strProtocolID = SurfacePointers.at( nMySelection)->GetProtocolID();
SurfaceProtocol myProtocol;

	try
		{
			myProtocol = Protocols->GetProtocol( strProtocolID);
	}
	catch( int nError)
	{
wxMessageBox( wstrProtocolNotFoundError, wstrErrorTitle, wxOK | wxICON_ERROR);

if (nError == ID_NO_MATCHING_PROTOCOL_ID)
	{
		blnProtocolExists = false;	
}
else
	{
blnProtocolExists = false;	
}
	}  // end catch
	
	if (blnProtocolExists)
		{
boost::shared_ptr <SurfaceProtocol> pProtocol( new SurfaceProtocol (myProtocol));
			
			if ((pProtocol->IsEncrypted()) && (blnEncryptionMode == false))
	{
wxMessageBox( wstrEncryptedProtocolError, wstrErrorTitle, wxOK | wxICON_ERROR);
}
	else
	{
// Get the surface parameters to keep the wizard happy
		boost::shared_ptr<SurfaceParameters> pParameters( new SurfaceParameters());

pParameters->SetSurfaceName( SurfacePointers.at( nMySelection)->GetSurfaceName());
pParameters->SetProtocolID( SurfacePointers.at( nMySelection)->GetProtocolID());
pParameters->SetHardwareInID( SurfacePointers.at( nMySelection)->GetHardwareInID());
pParameters->SetHardwareInName( SurfacePointers.at( nMySelection)->GetHardwareInName());
pParameters->SetHardwareOutID( SurfacePointers.at( nMySelection)->GetHardwareOutID());
pParameters->SetHardwareOutName( SurfacePointers.at( nMySelection)->GetHardwareOutName());
pParameters->SetDisplayInID( SurfacePointers.at( nMySelection)->GetDisplayInID());
pParameters->SetDisplayInName( SurfacePointers.at( nMySelection)->GetDisplayInName());
pParameters->SetDisplayOutID( SurfacePointers.at( nMySelection)->GetDisplayOutID());
pParameters->SetDisplayOutName( SurfacePointers.at( nMySelection)->GetDisplayOutName());
pParameters->SetSurfaceMode( SurfacePointers.at( nMySelection)->GetWidgetMode());

// Edit the protocol
EditProtocolWizard myProtocolEdit(this, true, pProtocol, pParameters, blnEncryptionMode, pHelpController);

if (myProtocolEdit.RunWizard( myProtocolEdit.GetSysExPage()))
{
	// Update the map of available protocols
	Protocols->Add( *pProtocol);
SaveProtocols();

// Update any dependent application configurations
std::string strProtocolID = pProtocol->GetProtocolID();
std::wstring wstrProtocolID( strProtocolID.begin(), strProtocolID.end());
Apps->UpdateDisplays( wstrProtocolID, pProtocol->GetDisplays());
SaveAppConfigs();
}  // if edit protocol wizard returns OK
				
// Open the ports again
OpenSurfacePorts( SurfacePointers.at( nMySelection));

// Tidy up
myProtocolEdit.Destroy();
	}  // end if encrypted
}  // end if blnProtocolExists
}


void SurfaceFrame::OnEditCloneProtocol(wxCommandEvent& event )
{
// Find the protocol ID for the highlighted surface
	int nMySelection = lbxSurfaces->GetSelection();
	
if (nMySelection < 0)   // No selection in list box
{
	return;
}

std::wstring wstrSurfaceName; 
std::string strProtocolID = SurfacePointers.at( nMySelection)->GetProtocolID();
				boost::shared_ptr <SurfaceProtocol> pProtocol( new SurfaceProtocol (Protocols->GetProtocol( strProtocolID)));

if ((pProtocol->IsEncrypted()) && (blnEncryptionMode == false))
	{
wxMessageBox( wstrEncryptedProtocolError, wstrErrorTitle, wxOK | wxICON_ERROR);
return;
}

// Prompt for a new protocol name
RBInputBox * protocolNameDlg = new RBInputBox( wstrProtocolNamePrompt, wxEmptyString);
		
		if (protocolNameDlg ->ShowModal() == wxID_CANCEL)
		{
			return;
		}

		if (protocolNameDlg ->GetText().empty())
		{
			return;
		}
		
		std::wstring wstrProtocolName = protocolNameDlg ->GetText();
				
	// Check that the new protocol has a unique name
bool blnUnique = Protocols->IsProtocolNameUnique( wstrProtocolName);
		
		if (blnUnique == false)
		{
wxMessageBox( wstrDuplicatedProtocolNameError, wstrErrorTitle, wxOK | wxICON_ERROR);
					return;
	}
		
	// Create a new protocol based on the original
	SurfaceProtocol myNewProtocol = *pProtocol;
	myNewProtocol.SetProtocolName( wstrProtocolName);
	
	// Add the new protocol to the collection
Protocols->Add( myNewProtocol);		
		SaveProtocols();

// Now clone all application configurations relevant to the old protocol
//&Apps->Clone( pProtocol->GetProtocolID(), myNewProtocol.GetProtocolID());		
		SaveAppConfigs();
}


void SurfaceFrame::OnEditCloneAppConfig(wxCommandEvent& event )
{
// Find the AppConfig ID for the highlighted surface
	int nMySelection = lbxSurfaces->GetSelection();
	
if (nMySelection < 0)   // No selection in list box
{
	return;
}

std::pair <std::wstring, std::wstring> myAppConfigID;
AppConfig mySourceAppConfig;

try
{
	myAppConfigID = SurfacePointers.at(nMySelection)->GetAppConfigID();
	mySourceAppConfig = Apps->GetAppConfig(myAppConfigID.first, myAppConfigID.second);
}
catch (RBException &myException)
{
#ifdef __WINDOWS__ 
	OutputDebugString(myException.what());
#endif

	wxMessageBox(wstrAppConfigCloneError, wstrErrorTitle, wxOK | wxICON_ERROR);
	return;
}

// Prompt for a new application configuration name
RBInputBox * AppNameDlg = new RBInputBox( wstrAppConfigNamePrompt, wxEmptyString);
		
		if (AppNameDlg ->ShowModal() == wxID_CANCEL)
		{
			return;
		}

		if (AppNameDlg ->GetText().empty())
		{
			return;
		}
		
		std::wstring wstrAppName = AppNameDlg ->GetText();

		// Update the ID to reflect the new app name
		myAppConfigID.first = wstrAppName;

	// Check that the new application has a unique name
bool blnUnique = Apps->IsAppConfigUnique( myAppConfigID);
		
		if (blnUnique == false)
		{
wxMessageBox( wstrDuplicatedAppConfigNameError, wstrErrorTitle, wxOK | wxICON_ERROR);
					return;
	}
		
	// Create a new app config based on the original
		AppConfig myNewAppConfig = mySourceAppConfig;
		myNewAppConfig.SetAppName( myAppConfigID.first);

	// Add the new app config to the collection
		Apps->Add( myAppConfigID.first, myAppConfigID.second, myNewAppConfig);		
		SaveAppConfigs();
}


void SurfaceFrame::OnSurfaceAdd(wxCommandEvent& event)
{
 				SurfaceWizard mySurfaceWizard( this, Protocols, pHelpController, true);

if (mySurfaceWizard.RunWizard( mySurfaceWizard.GetSurfacePage()))
{
	
	if (mySurfaceWizard.GetSurfacePage()->TransferDataFromWindow())
{
// Add the new Surface to the Surface array, if it is unique
	
SurfaceParameters myParameters = mySurfaceWizard.GetSurfaceParameters();	
	
if (IsSurfaceNameUnique(myParameters.GetSurfaceName()))
	{
				// Get the appropriate name strings for the AppConfig and Protocol:
		try
			{
				myParameters.SetAppConfigName( Apps->GetAppConfig( myParameters.GetAppConfigID().first, myParameters.GetAppConfigID().second).GetAppName());
		}
		catch( ...)
			{
myParameters.SetAppConfigName( wstrEmpty);		
		}
		
		try
			{
				myParameters.SetProtocolName( Protocols->GetProtocol( myParameters.GetProtocolID()).GetProtocolName());
		}
		catch( ...)
		{
			myParameters.SetProtocolName( wstrEmpty);
		}

					// Create a new surface and add it to the vector
				ActiveProduct myProduct;
DolphinProduct dpProduct;
										Speech->GetActiveProduct( myProduct, dpProduct);

SurfacePointer pSurface( new MIDISurface(this, myProduct, &myParameters, Protocols, Apps));
							SurfacePointers.push_back( pSurface);
					
// Update the surface list
lbxSurfaces->Append(SurfacePointers.back()->GetSurfaceName());

	// Make sure that the correct menu options are enabled
	UpdateMenuStates();
// Update the status bar for testing
UpdateStatusBar();

OpenSurfacePorts( SurfacePointers.back());
SaveSurfaces();
}  // if surface name is unique
	else
	{
wxMessageBox( wstrDuplicatedSurfaceNameError, wstrErrorTitle, wxOK | wxICON_ERROR);
			}
	}  // if valid surface
else
{
wxMessageBox( wstrIncompleteSurfaceError, wstrErrorTitle, wxOK | wxICON_ERROR );
}
			}  // OK button processing finished

// Tidy up
mySurfaceWizard.Destroy();
}   


void SurfaceFrame::OnSurfaceLoadAppConfig(wxCommandEvent& event)
{
// Find the protocol ID for the highlighted surface
	int nMySelection = lbxSurfaces->GetSelection();
	
if (nMySelection < 0)   // No selection in list box
{
	return;
}

	std::wstring wstrSurfaceName = lbxSurfaces->GetStringSelection().ToStdWstring();
	
	// CloseSurfacePorts( SurfacePointers.at( nMySelection));

	// Load files, in case new app configs have been copied into place
	LoadAppConfigs();

		std::string 	strProtocolID = SurfacePointers.at( nMySelection)->GetProtocolID();
AppConfigChooserDialog * myAppChooser = new AppConfigChooserDialog( wstrAppChooserTitle, strProtocolID, Apps); 

if (myAppChooser->ShowModal() == wxID_OK)
{
SurfacePointers.at( nMySelection)->SetAppConfigID( myAppChooser->GetAppConfigID());
		
try
	{
SurfacePointers.at( nMySelection)->SetAppConfigName( Apps->GetAppConfig( myAppChooser->GetAppConfigID().first, myAppChooser->GetAppConfigID().second).GetAppName());
	}
catch( ...)
{
		SurfacePointers.at( nMySelection)->SetAppConfigName( wstrEmpty);
	}

	SaveSurfaces();
UpdateStatusBar();
}  // end if Dialog returns OK

myAppChooser->Destroy();
// OpenSurfacePorts( SurfacePointers.at( nMySelection));
}


void SurfaceFrame::OnSurfaceDisplayProperties(wxCommandEvent& event)
{
int nMySelection = lbxSurfaces->GetSelection();
	
if (nMySelection < 0)   // No selection in list box
{
return;
}

CloseSurfacePorts( SurfacePointers.at( nMySelection));

			// Get current values
int nDIn = SurfacePointers.at( nMySelection)->GetDisplayInID();
int nDOut = SurfacePointers.at( nMySelection)->GetDisplayOutID();

// Display MIDI dialog to get new ports
#ifdef __WINDOWS__ 
MIDIDialog * dlgDisplaySetup = new MIDIDialog( wstrDisplaySetupTitle, nDIn, nDOut);
#else
MIDIDialog * dlgDisplaySetup = new MIDIDialog( wstrDisplaySetupTitle, true, nDOut);
#endif

if (dlgDisplaySetup->ShowModal() == wxID_OK) 
{
nDIn = dlgDisplaySetup->GetSelectedInput();
nDOut = dlgDisplaySetup->GetSelectedOutput();

if ((nDIn>= 0) && (nDOut>= 0))
{
	// Set new ports for the selected surface
SurfacePointers.at( nMySelection)->SetDisplayInID( nDIn);
	SurfacePointers.at( nMySelection)->SetDisplayInName( dlgDisplaySetup->GetSelectedInputName());
	SurfacePointers.at( nMySelection)->SetDisplayOutID( nDOut);
	SurfacePointers.at( nMySelection)->SetDisplayOutName( dlgDisplaySetup->GetSelectedOutputName());
	}  // end if valid port ID values
}  // if MIDIDialog OK

// Open the new ports 
OpenSurfacePorts( SurfacePointers.at( nMySelection));

// Tidy up
dlgDisplaySetup->Destroy();

SaveSurfaces();
}


void SurfaceFrame::OnSurfaceHardwareProperties(wxCommandEvent& event)
{
int nMySelection = lbxSurfaces->GetSelection();
	
if (nMySelection < 0)   // No selection in list box
{
return;
}

		CloseSurfacePorts( SurfacePointers.at( nMySelection));

			// Get current values
int nHIn = SurfacePointers.at( nMySelection)->GetHardwareInID();
int nHOut = SurfacePointers.at( nMySelection)->GetHardwareOutID();

// Display MIDI dialog to get new ports
#ifdef __WINDOWS__ 
MIDIDialog * dlgHardwareSetup = new MIDIDialog( wstrHardwareSetupTitle, nHIn, nHOut);
#else
MIDIDialog * dlgHardwareSetup = new MIDIDialog( wstrHardwareSetupTitle, false, nHIn);
#endif

if (dlgHardwareSetup->ShowModal() == wxID_OK) 
{
nHIn = dlgHardwareSetup->GetSelectedInput();
nHOut = dlgHardwareSetup->GetSelectedOutput();

if ((nHIn>= 0) && (nHOut>= 0))
{
	// Set new ports for the selected surface
SurfacePointers.at( nMySelection)->SetHardwareInID( nHIn);
	SurfacePointers.at( nMySelection)->SetHardwareInName( dlgHardwareSetup->GetSelectedInputName());
	SurfacePointers.at( nMySelection)->SetHardwareOutID( nHOut);
	SurfacePointers.at( nMySelection)->SetHardwareOutName( dlgHardwareSetup->GetSelectedOutputName());
	}  // end if valid port ID values
}  // if MIDIDialog OK

// Open the ports again
OpenSurfacePorts( SurfacePointers.at( nMySelection));

// Tidy up
dlgHardwareSetup->Destroy(); 

SaveSurfaces();
}


void SurfaceFrame::OnSurfaceProtocolProperties(wxCommandEvent& event)
{
	// Find the protocol ID for the highlighted surface
	int nMySelection = lbxSurfaces->GetSelection();
	
if (nMySelection < 0)   // No selection in list box
{
	return;
}

	std::wstring wstrSurfaceName = lbxSurfaces->GetStringSelection().ToStdWstring();

	// CloseSurfacePorts( SurfacePointers.at( nMySelection));

	std::string strProtocolID = SurfacePointers.at( nMySelection)->GetProtocolID();
	ProtocolChooserDialog * myProtocolChooser = new ProtocolChooserDialog( wstrProtocolChooserTitle, strProtocolID, Protocols); 

if (myProtocolChooser->ShowModal() == wxID_OK)
{
	SurfacePointers.at( nMySelection)->SetProtocolID( myProtocolChooser->GetProtocolID());
SaveSurfaces();
UpdateStatusBar();
}  // end if Dialog returns OK

myProtocolChooser->Destroy();
// OpenSurfacePorts( SurfacePointers.at( nMySelection));
}


void SurfaceFrame::OnSurfaceRename(wxCommandEvent& event)
{
int nMySelection = lbxSurfaces->GetSelection();
	
if (nMySelection < 0)
		{
			return;
		}
		
std::wstring wstrOldName = lbxSurfaces->GetStringSelection().ToStdWstring();
RBInputBox * RenameSurfaceDlg = new RBInputBox( wstrAppTitle, wstrRenameSurfacePrompt, wstrOldName);
		
		if (RenameSurfaceDlg ->ShowModal() == wxID_OK)
		{
			std::wstring wstrNewName = RenameSurfaceDlg ->GetText();

			// Tidy up
			RenameSurfaceDlg->Destroy(); 	
			
			if (wstrNewName.empty())
{
	wxMessageBox( wstrEmptySurfaceNameError, wstrErrorTitle, wxOK | wxICON_ERROR);
			return;
			}

			if (IsSurfaceNameUnique( wstrNewName) == false)
{
	wxMessageBox( wstrDuplicatedSurfaceNameError, wstrErrorTitle, wxOK | wxICON_ERROR);
			return;
			}

// Rename the surface with the new name
SurfacePointers.at( nMySelection)->SetSurfaceName( wstrNewName);
				lbxSurfaces->SetString( nMySelection, wstrNewName);
			SaveSurfaces();
}  // end if OK button
}


void SurfaceFrame::OnSurfaceDelayProperties( wxCommandEvent& event)
{
int nMySelection = lbxSurfaces->GetSelection();
	
	if (nMySelection < 0)   // No selection in list box
{
	return;
}

	std::wstring wstrSurfaceName = lbxSurfaces->GetStringSelection().ToStdWstring();
	RBInputBox * ReadDisplayDelayDlg = new RBInputBox( wstrReadDisplayDelayTitle, wstrReadDisplayDelayPrompt, boost::lexical_cast <std::wstring> (SurfacePointers.at( nMySelection)->GetReadDisplayDelay()));
		
		if (ReadDisplayDelayDlg->ShowModal() == wxID_OK)
		{
long lDelay = 0;
std::wstring wstrDelay = ReadDisplayDelayDlg->GetText();
lDelay = 0;

	try
	{
		lDelay = boost::lexical_cast <long> (wstrDelay); 
}
catch(bad_lexical_cast &)
{
wxMessageBox( wstrReadDisplayDelayError, wstrErrorTitle, wxOK | wxICON_ERROR);
}

if (	SurfacePointers.at( nMySelection)->SetReadDisplayDelay( lDelay) == false)
{
wxMessageBox( wstrReadDisplayDelayError, wstrErrorTitle, wxOK | wxICON_ERROR);
}  // if set value failed
			else
			{
SaveSurfaces();
			}  // Value set successfully
					}  // if input box closed with OK button
		ReadDisplayDelayDlg->Destroy(); 
}

		
		void SurfaceFrame::OnSurfaceRemove(wxCommandEvent& event)
{
	int nMySelection = lbxSurfaces->GetSelection();
	
if (nMySelection > -1)
		{
			std::wstring wstrMessage = wstrSurfaceRemovalCheck;
			wstrMessage.append( lbxSurfaces->GetStringSelection().ToStdWstring());
		
	if (		wxMessageBox( wstrMessage,                wstrAppTitle, wxYES_NO | wxNO_DEFAULT | wxICON_QUESTION) == wxYES) 
{
		if ( SurfacePointers.size() > 0)
    {
SurfaceVectorType::iterator it;
int i = 0;

for (it = SurfacePointers.begin(); it != SurfacePointers.end(); it++)
{
if (i == nMySelection)
{
				SurfacePointers.erase( it);
break;
}
i++;
}  // end for
					
				lbxSurfaces->Delete( nMySelection);
		SaveSurfaces();
		UpdateStatusBar();
		}  // if not empty
	}  // if user confirms
		}  // if nMySelection
		}


void SurfaceFrame::OnSurfaceModeLive( wxCommandEvent& event)
{
	int nMySelection = lbxSurfaces->GetSelection();
	
if (nMySelection < 0)   // No selection in list box
{
return;
}

SurfacePointers.at( nMySelection)->SetWidgetMode( ID_LIVE_MODE);
SaveSurfaces();
UpdateStatusBar();

DolphinReturnCode dpReturn;
	std::wstring wstrMessage = lbxSurfaces->GetStringSelection().ToStdWstring();
	wstrMessage.append( wstrSpace);
wstrMessage.append( wstrLiveMode);
Speech->Speak( wstrMessage, true, dpReturn);
}

		
void SurfaceFrame::OnSurfaceModeID( wxCommandEvent& event)
{
int nMySelection = lbxSurfaces->GetSelection();
	
if (nMySelection < 0)   // No selection in list box
{
return;
}

SurfacePointers.at( nMySelection)->SetWidgetMode( ID_ID_MODE);
SaveSurfaces();
UpdateStatusBar();

DolphinReturnCode dpReturn;
	std::wstring wstrMessage = lbxSurfaces->GetStringSelection().ToStdWstring();
	wstrMessage.append( wstrSpace);
wstrMessage.append( wstrIDMode);
Speech->Speak( wstrMessage, true, dpReturn);
}


void SurfaceFrame::OnSurfaceModeScreenReader( wxCommandEvent& event)
{
int nMySelection = lbxSurfaces->GetSelection();
	
if (nMySelection < 0)   // No selection in list box
{
return;
}

SurfacePointers.at( nMySelection)->SetWidgetMode( ID_SCREEN_READER_MODE);
SaveSurfaces();
UpdateStatusBar();

DolphinReturnCode dpReturn;
	std::wstring wstrMessage = lbxSurfaces->GetStringSelection().ToStdWstring();
wstrMessage.append( wstrSpace);
wstrMessage.append( wstrScreenReaderMode);
Speech->Speak( wstrMessage, true, dpReturn);
}


void SurfaceFrame::OnSurfaceModeConfig( wxCommandEvent& event)
{
	int nMySelection = lbxSurfaces->GetSelection();
	
if (nMySelection < 0)   // No selection in list box
{
return;
}

SurfacePointers.at( nMySelection)->SetWidgetMode( ID_CONFIG_MODE);
SaveSurfaces();
UpdateStatusBar();

DolphinReturnCode dpReturn;
	std::wstring wstrMessage = lbxSurfaces->GetStringSelection().ToStdWstring();
	wstrMessage.append( wstrSpace);
wstrMessage.append( wstrConfigMode);
Speech->Speak( wstrMessage, true, dpReturn);
}


void SurfaceFrame::OnVoiceMute(wxCommandEvent& event)
{
	HRESULT hr = Speech->IsMuted();
	
	if (hr == S_OK)
	{
		Speech->Mute( false);
	}
	else
	{
		Speech->Mute( true);
	}
}


void SurfaceFrame::OnVoiceSetup(wxCommandEvent& event)
{
#ifdef __WXOSX_COCOA__
SpeechDialog * VoiceSetupDlg = new SpeechDialog( wxT( "Setup Voice Parameters"), Speech);
	
	if (VoiceSetupDlg->ShowModal() == wxID_OK)
	{
	}  // end if OK button
	
	// Tidy up
	VoiceSetupDlg->Destroy();
#else
wxMessageBox( wxT( "Not available under Windows."), wstrErrorTitle, wxOK | wxICON_ERROR);
#endif
}


void SurfaceFrame::OnStartMinimised(wxCommandEvent& event)
{
	myOptions.SetStartSized( (! myOptions.GetStartSized()));
	myMenuBar->Check( ID_StartMinimised, (! myOptions.GetStartSized()));	
SaveOptions();
	}


void SurfaceFrame::OnEnableLogging(wxCommandEvent& event)
{
	blnLogging = (! blnLogging);
myMenuBar->Check( ID_StartMinimised, blnLogging);
}


void SurfaceFrame::OnViewLog(wxCommandEvent& event)
{
int nSurfaceNumber = lbxSurfaces->GetSelection();
	
	if (nSurfaceNumber < 0)
	{
		return;
	}

	if (Protocols->GetProtocol( SurfacePointers.at( nSurfaceNumber)->GetProtocolID()).IsEncrypted())
	{
		if (blnEncryptionMode == false)
	{
	wxMessageBox( wstrEncryptedProtocolError, wstrErrorTitle, wxOK | wxICON_ERROR);
return;
	}
		else
		{
			// Decrypt and display the log
boost::filesystem::path myPath = LogPath();
std::string strFileName = lbxSurfaces->GetStringSelection().ToStdString();
strFileName.append( strLogExtension);
myPath /= strFileName;
ShowLog( myPath, true);			
		}
	}
	else
	{
		// Display the log
		boost::filesystem::path myPath = LogPath();
		std::string strFileName = lbxSurfaces->GetStringSelection().ToStdString();
strFileName.append( strLogExtension);
myPath /= strFileName;
ShowLog( myPath, false);			
		}
}


void SurfaceFrame::OnHelpContents(wxCommandEvent& event)
{
	try
	{
		pHelpController->DisplayContents();
	}
	catch (const filesystem_error &error)
				{
					wxMessageBox(error.what(), wstrErrorTitle, wxOK | wxICON_ERROR);
									}
			}


void SurfaceFrame::OnHelpCheckForUpdates(wxCommandEvent& event)
{
UpdateChecker * myUpdates = new UpdateChecker( wstrAppTitle);

if (myUpdates->ShowModal() == wxOK)
{
	// Do updates here...
}

// Tidy up
myUpdates->Destroy();
}


void SurfaceFrame::OnHelpAbout(wxCommandEvent& event)
{
std::wstring wstrAboutText = wstrAboutMessage;

wstrAboutText.append( wstrHelpAboutCredits1);
wstrAboutText.append( wstrHelpAboutCredits2);
wstrAboutText.append( wstrHelpAboutCredits3);
wstrAboutText.append( wstrHelpAboutCredits4);
wstrAboutText.append( wstrHelpAboutCredits5);
wstrAboutText.append( wstrHelpAboutCredits6);
wstrAboutText.append( wstrHelpAboutCredits7);
wstrAboutText.append( wstrHelpAboutCredits8);
wstrAboutText.append( wstrHelpAboutCredits9);

	wxMessageBox( wstrAboutText,                wstrAppTitle, wxOK | wxICON_INFORMATION );
}

// End of menu bar event handlers


// Additional event handlers

void SurfaceFrame::OnListSelect(wxCommandEvent& event)
{
	UpdateMenuStates();
UpdateStatusBar();

// Set up the appropriate number of displays for the selected surface
int nMySelection = lbxSurfaces->GetSelection();
	
if (nMySelection < 0)
{
return;
}
else
{
// Get rid of any current displays
ClearDisplays();	
			
// Identify the selected surface and its protocol
std::wstring wstrSurfaceName = lbxSurfaces->GetStringSelection().ToStdWstring();
std::string strProtocolID = SurfacePointers.at( nMySelection)->GetProtocolID();
	boost::shared_ptr<SurfaceProtocol> pProtocol( new SurfaceProtocol (Protocols->GetProtocol( strProtocolID)));

for (unsigned int i = 0; i < pProtocol->GetDisplayCount(); i++)
{
	if (pProtocol->GetDisplayItem( i).IsLEDLamp() == false)
{
	// AddDisplay( pProtocol->GetDisplayItem( i).GetLabel());
		}  // end if not LEDLamp
}  // end for
} // end if valid selection
}  


void SurfaceFrame::OnListDoubleClick(wxCommandEvent& event)
{
	lbxSurfaces->ShowSurfaceReport();
}


void SurfaceFrame::OnSurfaceReport(wxCommandEvent& event)
{
int nSelection= lbxSurfaces->GetSelection();
	
if (nSelection> -1)
{
unsigned int nDisplayCount; 
std::wstring wstrOut;

nDisplayCount = Protocols->GetProtocol( SurfacePointers.at( nSelection)->GetProtocolID()).GetDisplayCount();

for (unsigned int nDisplayNumber = 0; nDisplayNumber < nDisplayCount; nDisplayNumber++)
	{
DisplayDefinition myDisplay; 
	std::wstring wstrDisplayName; 
int nLineCount = 0;
int nDisplayLength = 0;
int nLineLength = 0;
std::wstring wstrDisplayText; 

myDisplay = Protocols->GetProtocol( SurfacePointers.at( nSelection)->GetProtocolID()).GetDisplayItem( nDisplayNumber);		
wstrDisplayName = myDisplay.GetLabel();

		if (wstrDisplayName.empty() == false)
{
	wstrOut.append( wstrDisplayName);
	wstrOut.append( wstrSpacedColon);

	if (myDisplay.IsLEDLamp() == false)
		{
			wstrOut.append( wstrNewLine);
			nLineCount = myDisplay.GetLineCount();
nDisplayLength = myDisplay.GetDisplayLength();
nLineLength = nDisplayLength / nLineCount;
	}  // if not LED

wstrDisplayText = SurfacePointers.at( nSelection)->GetDisplayText( wstrDisplayName);

if (myDisplay.IsLEDLamp())
{
	wstrOut.append( SurfacePointers.at( nSelection)->GetCurrentState( wstrDisplayName));
}
else  // text display
	{
if (wstrDisplayText.empty() == false)
			{
				for (int nLineNumber= 1; nLineNumber<= nLineCount; nLineNumber++)
{
int nStart = (nLineNumber * nLineLength) -1;
std::wstring wstrTemp = wstrDisplayText.substr( nStart, nLineLength);
wstrTemp.append( wstrNewLine);
wstrOut.append( wstrTemp);
}  // end for line processing
		}  // end if display is empty
}  // end if LED

wstrOut.append( wstrNewLine);
}  // end if display name not empty
}  // end for looping through displays

// Display report
DisplayReportDlg * myReport = new DisplayReportDlg( lbxSurfaces->GetStringSelection().append( wstrSpace).append( wstrDisplayContentsTitle), wstrOut);
myReport->ShowModal();
myReport->Destroy();
}  // end if valid selection
}


bool SurfaceFrame::LoadOptions()
{
	    bool blnResult = false;
		boost::filesystem::path myPath( AppDataPath());
myPath /= wstrOptionsFileName;

if ((exists(myPath))
	&& (is_regular_file(myPath)))
{
	try
	{
		SurfaceReaderOptions myOptions{ LoadData <SurfaceReaderOptions>(myPath, false) };
		blnResult = true;
	}
	catch (RBException &myException)
	{
		wxMessageBox(myException.what(), wstrErrorTitle, wxOK | wxICON_ERROR);
	}  // end catch
}
else
{
	// Don't worry if the file doesn't exist, it will be created when needed
	blnResult = true;
}

return blnResult;
}


bool SurfaceFrame::SaveOptions()
{
	bool blnResult = false;
	path myPath( AppDataPath());
	
	try
	{
		if (! exists( myPath))    // does the path actually exist?
		{
			return blnResult;
		}  // if exists
		
		if (! is_directory( myPath))      // myPath must be a directory
		{
			return blnResult;
		}  // if directory
	}  // end try
	catch (const filesystem_error &error)
	{
		wxMessageBox( error.what(), wstrErrorTitle, wxOK | wxICON_ERROR);
		return blnResult;
	}
	
	// Directory exists so process the desired file
	myPath /= wstrOptionsFileName;
	
	 try
	 {
		 myOptions.SetStartSized( mySurfaceFrame->GetStartSized());	 
		 SaveData <SurfaceReaderOptions> ( myOptions, myPath, false);
		 		 	 return true;
	 }
	 catch( ...)
	 {
}
	return blnResult;	 
}


// Event handlers for handling surface updates

void SurfaceFrame::onSpeechUpdate(wxCommandEvent& event)
    {
        SpeechObject * pSpeechObj = new SpeechObject();
pSpeechObj = 		dynamic_cast <SpeechObject*> (event.GetEventObject());
std::wstring wstrOut = pSpeechObj->GetMsg();

if (wstrOut.empty())
	{
		return;
}

			DolphinReturnCode dpReturn;
			Speech->Speak( wstrOut, pSpeechObj->Interrupt(), dpReturn);
}
 

void SurfaceFrame::onScreenReaderCall(wxCommandEvent& event)
{
SRFunctionObject * pSRFunctionObj = new SRFunctionObject();
pSRFunctionObj = 		dynamic_cast <SRFunctionObject*> (event.GetEventObject());
	
HRESULT hr = Speech->ExecuteAction( pSRFunctionObj->GetFunctionName(), pSRFunctionObj->GetActionType());
}


void SurfaceFrame::onHotSpotCall(wxCommandEvent& event)
    {
HotSpotObject * pHSC = new HotSpotObject();
pHSC = 		dynamic_cast <HotSpotObject*> (event.GetEventObject());

HRESULT hr = Speech->ExecuteHotSpot( pHSC->GetHSCFileName(), pHSC->GetSpotName());
} 


void SurfaceFrame::OnDisplayUpdate(wxCommandEvent& event)
{
DisplayObject * pDisplay = dynamic_cast <DisplayObject*> (event.GetEventObject());

		if (pDisplay->IsLEDLamp())
		{
// delete pDisplay;
			return;
		}

	std::wstring wstrLabel = pDisplay->GetLabel();
std::wstring wstrContents = pDisplay->GetContents();

if ((wstrLabel.empty())
	|| (wstrContents.empty()))
{
	// Nothing worth processing
	return;
}

								// Search the map of surface displays
		// The index is the label for the static text, which then acts as an on-screen prompt for the text box
		std::map <std::wstring, DisplayElement>::iterator it;	
		it = myDisplays.find( wstrLabel);

	if (it == myDisplays.end())
	{
				// Create new display elements
DisplayElement myDisplayElement;

myDisplayElement.ElementSizer = new wxBoxSizer(wxHORIZONTAL);

std::wstring wstrElementLabel = wstrLabel;
myDisplayElement.DisplayLabel = new wxStaticText(this, wxID_ANY, wstrElementLabel, wxPoint( DISPLAY_LABEL_LEFT, -1), wxSize( NORMAL_WIDTH, -1));
myDisplayElement.DisplayContents = NULL;

		if (pDisplay->GetLineCount() > 1)
		{
			myDisplayElement.DisplayContents = new RBTextCtrl( this, wxID_ANY, wstrEmpty, wxPoint( DISPLAY_CONTENTS_LEFT, -1), wxSize( TEXT_WIDTH, -1), wxTE_READONLY | wxTE_MULTILINE | wxTE_NO_VSCROLL);
}
		else
{
myDisplayElement.DisplayContents = new RBTextCtrl( this, wxID_ANY, wstrEmpty, wxPoint( DISPLAY_CONTENTS_LEFT, -1), wxSize( TEXT_WIDTH, -1), wxTE_READONLY);
}
				
wxFont myFont = 		wxSystemSettings::GetFont(wxSYS_OEM_FIXED_FONT);
myDisplayElement.DisplayContents->SetFont( myFont);
myDisplayElement.DisplayContents->SetValue( wstrContents);
myDisplayElement.DisplayContents->SetInsertionPoint( 0);   // Always position the cursor at the start
// myDisplayElement.DisplayContents->Refresh();

// Add the new display components to their own sizer
myDisplayElement.ElementSizer->Add( myDisplayElement.DisplayLabel, wxEXPAND);
myDisplayElement.ElementSizer->Add( myDisplayElement.DisplayContents, wxEXPAND);
// Add the new display element to the overall display sizer
myDisplaySizer->Add( myDisplayElement.ElementSizer, wxEXPAND);
myDisplaySizer->AddSpacer( 25);

// Make sure that the new controls appear in the UI
myDisplayElement.DisplayLabel->Enable();
myDisplayElement.DisplayContents->Enable();
myDisplayElement.DisplayLabel->Show();
myDisplayElement.DisplayContents->Show();

// Add the new element to the map
std::pair <std::wstring, DisplayElement> myDisplayPair = std::make_pair( wstrLabel, myDisplayElement);
myDisplays.insert( myDisplayPair);

// Adjust tab order
if (myDisplays.size() == 1)
	{
		lbxSurfaces->MoveBeforeInTabOrder( myDisplayPair.second.DisplayLabel);
}
lbxSurfaces->MoveAfterInTabOrder( myDisplayPair.second.DisplayContents);
	}	
	else  // Display already exists, so update its contents
	{
		it->second.DisplayContents->SetInsertionPoint( 0);
		it->second.DisplayContents->SetValue( wstrContents);
		// it->second.DisplayContents->Refresh();
		it->second.DisplayContents->SetFocus();
	}

	// delete pDisplay;
}


void SurfaceFrame::onStatusUpdate( wxCommandEvent& event)
{
StatusObject * pStatus = dynamic_cast <StatusObject*> (event.GetEventObject());

if (pStatus->GetMode().empty())
		{
// delete pDisplay;
			return;
		}

UpdateStatusBar();
}

	
void SurfaceFrame::OnToggleDisplays( wxCommandEvent& event)
{
		// Set the focus to the first display, if present
		if (myDisplays.size() > 0)
		{
			if (this->FindFocus() != lbxSurfaces)
			{
			lbxSurfaces->SetFocus();
	}
			else
			{
			// Set focus to the first text box
			myDisplays.at( 0).DisplayContents->SetFocus();
}
				}
}


// Event table

	BEGIN_EVENT_TABLE(SurfaceFrame, wxFrame)
// Surface updates
// On-screen display update from a surface
EVT_COMMAND(SURFACE_UPDATE_ID, wxEVT_COMMAND_TEXT_UPDATED, SurfaceFrame::OnDisplayUpdate)
	// Speech update from a surface
EVT_COMMAND  (SPEECH_UPDATE_ID, wxEVT_COMMAND_TEXT_UPDATED, SurfaceFrame::onSpeechUpdate)
// Call a hot spot
	EVT_COMMAND(HOT_SPOT_CALL_ID, wxEVT_COMMAND_TEXT_UPDATED, SurfaceFrame::onHotSpotCall)
// Call a screen-reader function
	EVT_COMMAND(SCREEN_READER_FUNCTION_CALL_ID, wxEVT_COMMAND_TEXT_UPDATED, SurfaceFrame::onScreenReaderCall)
// Update the status bar
EVT_COMMAND(STATUS_UPDATE_ID, wxEVT_COMMAND_TEXT_UPDATED, SurfaceFrame::onStatusUpdate)	

// AppConfig wizard
	// EVT_WIZARD_FINISHED(wxID_ANY, SurfaceFrame::OnAppConfigWizardFinished)

		// List box handling
EVT_LISTBOX( ID_SURFACE_LIST_BOX, SurfaceFrame::OnListSelect)
EVT_LISTBOX_DCLICK( ID_SURFACE_LIST_BOX, SurfaceFrame::OnListDoubleClick)

// Menu events
EVT_MENU(ID_NewProtocol,  SurfaceFrame::OnNewProtocol)    
EVT_MENU(ID_NewAppConfig,  SurfaceFrame::OnNewAppConfig)
EVT_MENU(ID_ImportProtocol,  SurfaceFrame::OnImportProtocol)    
EVT_MENU(ID_ImportAppConfig,  SurfaceFrame::OnImportAppConfig)
EVT_MENU(ID_ExportProtocol,  SurfaceFrame::OnExportProtocol)    
EVT_MENU(ID_ExportAppConfig,  SurfaceFrame::OnExportAppConfig)
EVT_MENU(ID_Hide,  SurfaceFrame::OnHide)
EVT_MENU(ID_ToggleDisplays,  SurfaceFrame::OnToggleDisplays)
EVT_MENU(wxID_EXIT, SurfaceFrame::OnQuit)
EVT_CLOSE(SurfaceFrame::OnCloseWindow)


EVT_MENU(ID_EditAppConfig,  SurfaceFrame::OnEditAppConfig)
	EVT_MENU(ID_EditProtocol,  SurfaceFrame::OnEditProtocol)
		EVT_MENU(ID_EditCloneProtocol,  SurfaceFrame::OnEditCloneProtocol)
EVT_MENU(ID_EditCloneAppConfig,  SurfaceFrame::OnEditCloneAppConfig)

EVT_MENU( ID_SurfaceAdd, SurfaceFrame::OnSurfaceAdd)    
EVT_MENU( ID_SurfaceLoadAppConfig, SurfaceFrame::OnSurfaceLoadAppConfig)    
EVT_MENU( ID_SurfaceDisplayMIDIProperties,                 SurfaceFrame::OnSurfaceDisplayProperties)
		EVT_MENU(ID_SurfaceHardwareMIDIProperties,  SurfaceFrame::OnSurfaceHardwareProperties)
EVT_MENU(ID_SurfaceProtocolProperties,  SurfaceFrame::OnSurfaceProtocolProperties)
EVT_MENU(ID_SurfaceDelayProperties,  SurfaceFrame::OnSurfaceDelayProperties)
EVT_MENU( ID_SurfaceRename, SurfaceFrame::OnSurfaceRename)    	
EVT_MENU( ID_SurfaceRemove, SurfaceFrame::OnSurfaceRemove)    	
EVT_MENU( ID_SurfaceDisplayReport, SurfaceFrame::OnSurfaceReport)    	

EVT_MENU( ID_SurfaceModeLive,                 SurfaceFrame::OnSurfaceModeLive)
		EVT_MENU(ID_SurfaceModeID,  SurfaceFrame::OnSurfaceModeID)
EVT_MENU(ID_SurfaceModeScreenReader,  SurfaceFrame::OnSurfaceModeScreenReader)
EVT_MENU( ID_SurfaceModeConfig,                 SurfaceFrame::OnSurfaceModeConfig)
				
EVT_MENU( ID_VoiceMute,                 SurfaceFrame::OnVoiceMute)
EVT_MENU( ID_VoiceSetup,                 SurfaceFrame::OnVoiceSetup)
EVT_MENU( ID_StartMinimised,                 SurfaceFrame::OnStartMinimised)
EVT_MENU( ID_EnableLogging,                 SurfaceFrame::OnEnableLogging)
EVT_MENU( ID_ViewLog,                 SurfaceFrame::OnViewLog)
				
EVT_MENU(ID_HelpContents, SurfaceFrame::OnHelpContents)
				EVT_MENU(ID_Help_Update_Check, SurfaceFrame::OnHelpCheckForUpdates)
EVT_MENU(wxID_ABOUT, SurfaceFrame::OnHelpAbout)
END_EVENT_TABLE()


