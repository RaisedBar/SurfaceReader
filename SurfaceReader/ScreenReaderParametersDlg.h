// ScreenReaderParametersDlg.h
// Custom dialog class definition for the Application Configuration Wizard
// Author:  T A Burgess
// Raised Bar Ltd.
// http://www.raisedbar.net


// #define _SCL_SECURE_NO_WARNINGS

#ifndef SCREEN_READER_PARAMS_H
#define SCREEN_READER_PARAMS_H

#pragma once

#include <vector>

#include "SRConstants.h"
#include "RBSpeech.h"

#include <boost/shared_ptr.hpp>

#include <wx/msgdlg.h>
#include <wx/dialog.h>
#include <wx/sizer.h>
#include <wx/StatText.h>
#include <wx/TextCtrl.h>
#include <wx/checkbox.h>
#include <wx/button.h>
#include <wx/msw/winundef.h> 


typedef std::vector<JAWSParameter> JawsParametersType;

class ScreenReaderParametersDlg: public wxDialog
{
public:
ScreenReaderParametersDlg(const wxString& title, boost::shared_ptr <JawsParametersType> pSpeechParameters);
~ScreenReaderParametersDlg();

std::wstring GetParameterString();

private:
void AddParameter( std::wstring wstrPrompt);
	void SetParameter( std::wstring wstrLabel, std::wstring wstrValue);

	// Event handlers
void OnOK(wxCommandEvent& event);
	void OnCancel(wxCommandEvent& event);

	wxPanel *myPanel; 

	// Sizer for parameter controls
wxBoxSizer * vBoxParameters;
	
// Buttons
wxButton * btnOK, * btnCancel;

// Internal storage

// Map of parameter controls
		// The index is the label for the static text, which then acts as an on-screen prompt for the text box
		std::map <std::wstring, std::pair < wxStaticText *, wxTextCtrl * > > myParameters;
boost::shared_ptr <JawsParametersType> pMySpeechParameters; 
std::wstring wstrArguments;

DECLARE_EVENT_TABLE()
};

#endif       // SCREEN_READER_PARAMS_H