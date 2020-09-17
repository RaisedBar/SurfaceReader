// DisplayParamsDlg.h
// Author:  T A Burgess
// Raised Bar Ltd.
// http://www.raisedbar.net

#ifndef DISPLAY_PARAMS_DLG_H
#define DISPLAY_PARAMS_DLG_H

#pragma once

#include "RBEnums.h"
#include "SRConstants.h"
#include "AppConfig.h"
#include "SurfaceAction.h"

#include <boost/shared_ptr.hpp>
#include <boost/lexical_cast.hpp>
using boost::lexical_cast;
using boost::bad_lexical_cast;

#include <wx/sizer.h>
#include <wx/StatText.h>
#include <wx/TextCtrl.h>
#include <wx/listbox.h>
#include <wx/button.h>
#include <wx/msgdlg.h> 
#include <wx/msw/winundef.h> 


class DisplayParamsDlg: public wxDialog
{
public:
DisplayParamsDlg(const wxString & title, SurfaceActionType mySAType, boost::shared_ptr<AppConfig> pAppConfig);
~DisplayParamsDlg();

bool IsValidParameterSet();
std::string GetDisplayHash();
int GetDisplayLine();
int GetDisplayStrip();
int GetDisplayStartPosition();
int GetDisplaySubstringLength();

private:
void ListDisplays();
void ListParameter1Values( std::string strHash);
void ListLengths();

// Event handlers:
void OnDisplaySelect( wxCommandEvent& event);
void OnParameter1Select( wxCommandEvent& event);
void OnOK( wxCommandEvent& event);
void OnCancel( wxCommandEvent& event);

// Internal storage
// List box of available display names
wxListBox * lbxDisplayNames; 
wxListBox * lbxDisplayParam1; 
wxListBox * lbxDisplayParam2; 

// Standard dialog buttons
wxButton * btnOK, * btnCancel;

// The action type we're dealing with:
SurfaceActionType mySurfaceActionType;

// Pointer to the protocol being referenced
boost::shared_ptr<AppConfig> pMyAppConfig;

DECLARE_EVENT_TABLE()
};

#endif       // DISPLAY_PARAMS_DLG_H


