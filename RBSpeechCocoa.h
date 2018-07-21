//RBSpeechCocoa.h: speech utilities class header file.
//Copyright (�) 2012 Raised Bar Ltd.

#ifndef RB_SPEECH_COCOA_H
#define RB_SPEECH_COCOA_H

#pragma once

#ifdef __WXOSX_COCOA__   
typedef long HRESULT;
// typedef bool BOOL;
struct SpeechSynthesizerImpl;

/* Pre-defined success HRESULTS */
#define S_OK ((HRESULT)0x00000000L)
#define S_FALSE ((HRESULT)0x00000001L)
#define SUCCEEDED(Status) ((HRESULT)(Status) >= 0)
#define FAILED(Status) ((HRESULT)(Status)<0)
#include <wx/osx/core/cfstring.h>
#include <wx/msgdlg.h>

#include "SpeechParameters.h"
#include "Serializer.h"

class RBSpeech
{
	SpeechSynthesizerImpl* impl;

public:
	explicit RBSpeech();
	explicit RBSpeech( const wxString voice_identifier);
	~RBSpeech();
	
	float GetRate() const;
	void SetRate( float myRate);
	float GetVolume() const;
	void SetVolume( float myVolume);
	unsigned long GetVoiceCount();
	wxString GetVoice( unsigned int nVoiceIndex);
	wxString GetVoice() const;
	int GetVoiceIndex( wxString wxstrVoiceName);
	void SetVoice( wxString wxstrVoice);

bool LoadSpeechParameters();
bool SaveSpeechParameters();

	HRESULT GetActiveProduct(ActiveProduct& Product, DolphinProduct& SpecificDolphinProduct);
	HRESULT IsMuted(void);
	void Mute( bool blnNewMute);
	
	HRESULT Speak(wstring strText, DolphinReturnCode& ReturnCode);
	HRESULT Speak(wstring strText, bool  blnSilence, DolphinReturnCode& ReturnCode);
HRESULT Silence(DolphinReturnCode& ReturnCode);

HRESULT GetAvailableActions(AvailableActionsType& ActionInformation);
HRESULT ExecuteAction(std::wstring Action, ScreenReaderActionType Type);
HRESULT ExecuteHotSpot(std::wstring Set, std::wstring SpotName);
void ClearHscFile();
void SetHscFile(wxString File);
HRESULT ListHotSpotsInSet(std::wstring SetName, std::vector<std::string>& Spots);
void SetFirstJsdFile(wxString File);
wxString GetFirstJsdFile(void);
void ClearJsdFile();

private:
bool blnMutedSpeech;
wxString wxstrVoiceName;
float fRate, fVolume;
};
#endif  // Mac
#endif  // if RB_SPEECH_COCOA_H
