// SpeechDialog.h
// Custom dialog class definition for the selection of Mac speech parameters
// Author:  T A Burgess
// Raised Bar Ltd.
// http://www.raisedbar.net

#ifndef SPEECH_DIALOG_H
#define SPEECH_DIALOG_H

#pragma once

#include <wx/platform.h>
#ifdef __WXOSX_COCOA__

#include <boost/shared_ptr.hpp>
#include <boost/lexical_cast.hpp>
using boost::lexical_cast;
using boost::bad_lexical_cast;

#include "RBEnums.h"
#include "SRConstants.h"
#include "RBSpeech.h"

#include <wx/msgdlg.h>
#include <wx/sizer.h>
#include <wx/utils.h>
#include <wx/dialog.h>
#include <wx/listbox.h>
#include <wx/StatText.h>
#include <wx/button.h>
#include <wx/StatText.h>
#include <wx/textctrl.h>
#include <wx/msw/winundef.h>


const int SPEECH_DIALOG_HEIGHT = 260;
const int SPEECH_DIALOG_WIDTH = 460;


class SpeechDialog: public wxDialog
{
public:
	SpeechDialog(const wxString& title, boost::shared_ptr <RBSpeech> myRBSpeech);
~SpeechDialog();

void VoiceName( std::wstring wstrVoice);
std::wstring VoiceName();
	void SpeechRate( float fRate);
	float SpeechRate();
	void SpeechPitch( float fPitch);
	float SpeechPitch();
	void SpeechVolume( float fVolume);
	float SpeechVolume();
	
private:
	void ListVoiceNames();

	// Event handlers
void OnTest(wxCommandEvent& event);
void OnOK(wxCommandEvent& event);
    void OnCancel(wxCommandEvent& event);

	wxListBox * lbxVoiceNames;
	wxTextCtrl * txtVoiceRate, * txtVoiceVolume;
	
	boost::shared_ptr <RBSpeech> pMySpeech;
	std::wstring wstrOldVoice;
	float fOldRate, fOldVolume;
	
	DECLARE_EVENT_TABLE()
};
#endif  // if cocoa
#endif       // SPEECH_DIALOG_H
