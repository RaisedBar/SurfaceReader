// ProtocolChooserDialog.h
// Custom dialog class definition for changing the active protocol for a surface
// Author:  T A Burgess
// Raised Bar Ltd.
// http://www.raisedbar.net

// #define _SCL_SECURE_NO_WARNINGS

#ifndef CHOOSE_PROTOCOL_H
#define CHOOSE_PROTOCOL_H

#pragma once

#include "SRConstants.h"
#include "RBEnums.h"
#include "ProtocolCollection.h"

#include <boost/shared_ptr.hpp>

#include <wx/StatText.h>
#include <wx/sizer.h>
#include <wx/button.h>
#include <wx/listbox.h>
#include <wx/dialog.h>
#include <wx/msw/winundef.h> 


class ProtocolChooserDialog: public wxDialog
{
public:
ProtocolChooserDialog( const wxString& title, std::string strProtocolID, boost::shared_ptr <ProtocolCollection> pProtocols);
~ProtocolChooserDialog();

std::string GetProtocolID();

private:
	void ListProtocols( std::string strMyProtocolID);
	
	// Event handlers:
void OnListSelect( wxCommandEvent& event);
	void OnOK( wxCommandEvent& event);
void OnCancel( wxCommandEvent& event);

wxStaticText * lblProtocolsPrompt;
wxListBox * lbxProtocols;  
boost::shared_ptr <ProtocolCollection> pMyProtocols;

DECLARE_EVENT_TABLE()
};
#endif       // CHOOSE_PROTOCOL_H
