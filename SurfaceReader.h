//SurfaceReader.h

#ifndef SURFACE_READER_APP_H
#define SURFACE_READER_APP_H

#pragma once

#include "SurfaceFrame.h"
#include "SRConstants.h"

// For compilers that support precompilation, includes "wx/wx.h".
#include "wx/wxprec.h"

#ifdef __BORLANDC__
    #pragma hdrstop
#endif

// for all others, include the necessary headers (this file is usually all you
// need because it includes almost all "standard" wxWidgets headers)
#ifndef WX_PRECOMP
    #include "wx/wx.h"
#endif

// define this to 1 to use HTML help even under Windows (by default, Windows
// version will use WinHelp).
// Please also see samples/html/helpview.
#define USE_HTML_HELP 1

#ifdef __WINDOWS__ 
// define this to 1 to use external help controller (not used by default)
#define USE_EXT_HELP 0
// Define this to 0 to use the help controller as the help
// provider, or to 1 to use the 'simple help provider'
// (the one implemented with wxTipWindow).
#define USE_SIMPLE_HELP_PROVIDER 0
#else   // Mac
#define USE_EXT_HELP 1
#define USE_SIMPLE_HELP_PROVIDER 1
#endif

// Define this to 1 to handle exceptions:
#define wxUSE_EXCEPTIONS 1
	
#include <wx/app.h>
#include <wx/snglinst.h>
#include <wx/platform.h>
#include <wx/cmdline.h>
#include <wx/msw/winundef.h> 


class SurfaceReaderApp: public wxApp
{
public:
SurfaceReaderApp();
    virtual ~SurfaceReaderApp(){};

virtual bool OnInit();
    
// 2nd-level exception handling: we get all the exceptions occurring in any
    // event handler here
    // virtual bool OnExceptionInMainLoop();
// 3rd, and final, level exception handling: whenever an unhandled
    // exception is caught, this function is called
    // virtual void OnUnhandledException();

	// int OnExit();

virtual void OnInitCmdLine(wxCmdLineParser& parser);
    virtual bool OnCmdLineParsed(wxCmdLineParser& parser);

	bool AllowEncryption;

	// Check for multiple instances
	wxSingleInstanceChecker * m_checker;
};


//Command line arguments available.
static const wxCmdLineEntryDesc g_cmdLineDesc [] =
{
{wxCMD_LINE_SWITCH, "x" },
     { wxCMD_LINE_NONE }
};

// Create forward reference to allow other modules to use wxGetApp()
// wxDECLARE_APP ( SurfaceReaderApp);  
#endif

