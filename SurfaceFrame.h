// SurfaceFrame.h

// #define _SCL_SECURE_NO_WARNINGS

#ifndef SURFACE_FRAME_H
#define SURFACE_FRAME_H

#pragma once

// https://embeddedartistry.com/blog/2017/2/1/c11-implementing-a-dispatch-queue-using-stdfunction
#include "RBSpeech.h"
#include "SurfaceReaderHelp.h"
#include "AppConfigChooserDialog.h"
#include "AppConfigWizard.h"
#include "EditProtocol.h"
#include "ProtocolChooserDialog.h"
#include "ProtocolWizard.h"
#include "RBTextCtrl.h"
#include "AppCollection.h"
#include "MIDISurface.h"
#include "ProtocolCollection.h"
#include "SROptions.h"
#include "SurfaceList.h"
#include "SurfaceWizard.h"
#include "UpdateChecker.h"

#ifdef __WXOSX_COCOA__
#include "SpeechDialog.h"
#endif

#define BOOST_FILESYSTEM_VERSION 3
using namespace boost::filesystem;

#include <wx/panel.h>
#include <wx/accel.h>
#include <wx/frame.h>
#include <wx/menu.h>
#include <wx/stattext.h>
#include <wx/sizer.h>
#include "wx/taskbar.h"
#include <wx/dirdlg.h>


#ifdef __WINDOWS__ 
#include <wx/msw/helpchm.h> // (MS HTML Help controller)
#else
#include <wx/generic/helpext.h>
#endif

#include <wx/msw/winundef.h> 


// Screen positions
const int LEFT_MARGIN = 60;
const int NORMAL_WIDTH = 150;
const int TEXT_WIDTH = 600;
const int TOP_MARGIN = 60;
const int SURFACE_LIST_LEFT = 210;
const int DISPLAY_LABEL_LEFT = 200;
const int DISPLAY_CONTENTS_LEFT = 470;

struct DisplayElement
{
wxBoxSizer * ElementSizer;
	wxStaticText * DisplayLabel;
RBTextCtrl * DisplayContents;
};


class myTaskBarIcon : public wxTaskBarIcon
{
public:
#if defined(__WXOSX__) && wxOSX_USE_COCOA
    myTaskBarIcon(wxTaskBarIconType iconType = wxTBI_DEFAULT_TYPE)
    :   wxTaskBarIcon(iconType)
#else
    myTaskBarIcon()
#endif
    {}

	wxMenu *CreatePopupMenu();
    void OnLeftButtonDClick(wxTaskBarIconEvent&);
    void OnMenuShowHide(wxCommandEvent&);
    void OnMenuQuit(wxCommandEvent&);
    
	DECLARE_EVENT_TABLE()

private:
wxMenu *menu; 
};


class SurfaceFrame : public wxFrame
{
public:
        SurfaceFrame(const wxString& title, bool EncryptionMode);
~SurfaceFrame();

bool GetStartSized();
void Shutdown();
    
private:
bool InitData();
void ClearDisplays();
void AddDisplay( std::wstring wstrLabel);
void SetDisplayText( std::wstring wstrLabel, std::string strDisplayText);

	void 	InitMenus();
void UpdateMenuStates();
	void UpdateStatusBar();
		void ListSurfaces();
bool OpenSurfacePorts( SurfacePointer pMySurface);
		void CloseSurfacePorts(SurfacePointer pMySurface);
std::string GetPortErrorReport(SurfacePointer pMySurface);
		void OpenAllSurfaces();
		void CloseAllSurfaces();

bool IsSurfaceNameUnique( std::wstring wstrName);
bool CopyDir( boost::filesystem::path const & source, boost::filesystem::path const & destination);

bool DeleteAppConfigFiles();
void SaveAppConfigs();
bool LoadAppConfigs();

bool DeleteProtocolFiles();
void SaveProtocols();
bool LoadProtocols();

bool DeleteSurfaceFiles();
void SaveSurfaces();
bool LoadSurfaces();

	bool LoadOptions();
	bool SaveOptions();

void ShowLog( boost::filesystem::path myPath, bool blnIsEncrypted);

// event handlers 

	void OnDisplayUpdate(wxCommandEvent& event);
	void onSpeechUpdate(wxCommandEvent& event);
void onHotSpotCall(wxCommandEvent& event);
void onScreenReaderCall(wxCommandEvent& event);
void onStatusUpdate(wxCommandEvent& event);
    
    void OnNewProtocol(wxCommandEvent& event);    
void OnNewAppConfig( wxCommandEvent& event);
void OnImportProtocol(wxCommandEvent& event);    
void OnImportAppConfig( wxCommandEvent& event);
void OnExportProtocol(wxCommandEvent& event);    
void OnExportAppConfig( wxCommandEvent& event);
void OnHide( wxCommandEvent& event);
void OnQuit(wxCommandEvent& event);

void OnEditAppConfig( wxCommandEvent& event);
	void OnEditProtocol( wxCommandEvent& event);
void OnEditCloneProtocol( wxCommandEvent& event);
void OnEditCloneAppConfig( wxCommandEvent& event);

void OnSurfaceAdd(wxCommandEvent& event);
void OnSurfaceLoadAppConfig(wxCommandEvent& event);
void OnSurfaceReport(wxCommandEvent& event);
void OnSurfaceRename(wxCommandEvent& event);
void OnSurfaceDisplayProperties( wxCommandEvent& event);
		void OnSurfaceHardwareProperties( wxCommandEvent& event);
void OnSurfaceProtocolProperties( wxCommandEvent& event);
	void OnSurfaceDelayProperties( wxCommandEvent& event);
				void OnSurfaceRemove(wxCommandEvent& event);
void OnSurfaceModeLive( wxCommandEvent& event);
		void OnSurfaceModeID( wxCommandEvent& event);
void OnSurfaceModeScreenReader( wxCommandEvent& event);
void OnSurfaceModeConfig( wxCommandEvent& event);
    
void OnVoiceMute(wxCommandEvent& event);
void OnVoiceSetup(wxCommandEvent& event);
void OnStartMinimised(wxCommandEvent& event);
void OnEnableLogging(wxCommandEvent& event);
void OnViewLog(wxCommandEvent& event);

	void OnHelpContents(wxCommandEvent& event);
	void OnHelpCheckForUpdates(wxCommandEvent& event);
		void OnHelpAbout(wxCommandEvent& event);

void OnListSelect( wxCommandEvent& event);
			void OnListDoubleClick( wxCommandEvent& event);
void OnToggleDisplays( wxCommandEvent& event);

		// internal storage
    					wxPanel *myPanel; 
wxMenuBar *myMenuBar;
						
// List of defined control surfaces
		SurfaceList * lbxSurfaces;

		// Sizer to hold displays
		wxBoxSizer * myDisplaySizer;

		// UI options, etc.
			SurfaceReaderOptions myOptions;

			// Map of surface displays
					// The index is the label for the static text, which then acts as an on-screen prompt for the text box
		std::map <std::wstring, DisplayElement> myDisplays;

		#ifdef __WXOSX_COCOA__   
// Flag to indicate if this is the first idle event processed
		bool blnFirstIdle;
#endif

// Do we allow encryption of protocols and/or viewing of already encrypted data?
		bool blnEncryptionMode;

		// Are we logging events?
		bool blnLogging;
		
		// Menus
	wxMenu * FileMenu, * FileNewSubMenu, * FileImportSubMenu, * FileExportSubMenu; 
	wxMenu * EditMenu;
	wxMenu * SurfaceMenu, * SurfacePropertiesSubMenu, * SurfaceModeSubMenu;
	wxMenu * OptionsMenu;
				     wxMenu * HelpMenu;
    
std::wstring wstrHelpFileName;

// Vector of MIDISurface objects
	    SurfaceVectorType SurfacePointers;

// Storage for application configurations
		boost::shared_ptr<AppCollection> Apps;

// Storage for all loaded protocols
boost::shared_ptr <ProtocolCollection> Protocols;

		// Speech output
		boost::shared_ptr<RBSpeech> Speech;
// Allow faster retrieval of Jaws parameters, etc.
boost::shared_ptr<JawsCacheType> JawsCache;

// Cope with mid-stream changes of access technology
ActiveProduct myActiveProduct, myOldActiveProduct;

// Platform-dependent help systems
#ifdef __WINDOWS__ 
wxCHMHelpController * pHelpController;
#else
wxExtHelpController * pHelpController;
#endif

// Taskbar handling
wxIcon myIcon;    
myTaskBarIcon * m_taskBarIcon;

#if defined(__WXOSX__) && wxOSX_USE_COCOA
    myTaskBarIcon   *m_dockIcon;
#endif
	
	DECLARE_EVENT_TABLE()
};
#endif

