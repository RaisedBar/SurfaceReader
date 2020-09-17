// AppConfigChooserDialog.h
// Custom dialog class definition for loading application configurations
// Author:  T A Burgess
// Raised Bar Ltd.
// http://www.raisedbar.net


#ifndef CHOOSE_APP_CONFIG_H
#define CHOOSE_APP_CONFIG_H

#pragma once

#include "SRConstants.h"
#include "RBEnums.h"
#include "AppCollection.h"

#include <boost/shared_ptr.hpp>

#include <wx/StatText.h>
#include <wx/sizer.h>
#include <wx/button.h>
#include <wx/listbox.h>
#include <wx/dialog.h>
#include <wx/msw/winundef.h> 


class AppConfigChooserDialog: public wxDialog
{
public:
AppConfigChooserDialog( const wxString& title, std::string strProtocolID, boost::shared_ptr <AppCollection> pApps);
~AppConfigChooserDialog();

std::pair <std::wstring, std::wstring> GetAppConfigID();

private:
	void ListDefinedApps();
	
	// Event handlers:
void OnListSelect( wxCommandEvent& event);
	void OnOK( wxCommandEvent& event);
void OnCancel( wxCommandEvent& event);

wxStaticText * lblDefinedAppsPrompt;
wxListBox * lbxApps;  
std::wstring wstrMyProtocolID;
boost::shared_ptr <AppCollection> pMyApps;

DECLARE_EVENT_TABLE()
};
#endif       // CHOOSE_APP_CONFIG_H
