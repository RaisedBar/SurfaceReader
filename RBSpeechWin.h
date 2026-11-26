//RBSpeechWin.h
// speech utilities class header file.
//Copyright (�) 2012 Raised Bar Ltd.

#ifndef RB_SPEECH_WIN_H
#define RB_SPEECH_WIN_H

#pragma once

#include <memory>
#include <windows.h>
// atlcore.h casts FARPROC to function pointers internally (warning C4191)
#pragma warning( push)
#pragma warning( disable: 4191)
#include <atlbase.h>
#pragma warning( pop)
#include <Psapi.h>

//msi/wix includes.
#include <Msi.h>
#include "WIX Include.h"

// The screen reader DLLs shipped with SurfaceReader (NVDA, System Access) are the 64-bit versions
#ifndef _WIN64
#error SurfaceReader for Windows is built for x64 only
#endif

//typedefs.
//Nvda.
typedef error_status_t (WINAPI *nvdaControllerTestIfRunningFunc)(void);
typedef error_status_t (WINAPI *nvdaControllerCancelSpeechFunc)(void);
typedef error_status_t (WINAPI *nvdaControllerSpeakTextFunc)(const wchar_t *text);
typedef error_status_t (WINAPI *nvdaControllerBrailleMessageFunc)(const wchar_t *message);

//System access.
typedef BOOL (WINAPI *SAIsRunningFunc)(void);
typedef BOOL (WINAPI *SABrailleFunc)(const WCHAR *textW);
typedef BOOL (WINAPI *SASpeakFunc)(const WCHAR *textW);
typedef BOOL (WINAPI *SAStopAudioFunc)(void);



/* Speech utilities class allowing other applications to output speech to any supported access technology. 
The following access technologies arre supported:
JAWS For Windows (version 8 or later).
System Access.
Non-Visual Desktop Access.
*/

class UiaAnnouncer;

class RBSpeech
{
public:
	RBSpeech(void);
	~RBSpeech(void);

	/// <summary> The window UI Automation notifications are raised from; call once the main frame exists. </summary>
	/// <remarks> Used to speak through Narrator and any other screen reader without a direct API here, and as a fallback when a direct API fails. </remarks>
	void SetAnnounceWindow( HWND hWindow);

	/// <summary> Forget the cached screen reader, so the next call checks which one is running. </summary>
	void RefreshActiveProduct();

private:
	// Owns a COM object and possibly a COM initialisation, so it must not be copied
	RBSpeech( const RBSpeech &) = delete;
	RBSpeech & operator=( const RBSpeech &) = delete;

	// Which screen reader is running, cached so that speaking doesn't scan every process
	// and reload each screen reader's API every time; see GetActiveProduct.
	HRESULT DetectActiveProduct( ActiveProduct & Product);
	/// <summary> Determine whether Windows Narrator is running. </summary>
	bool IsNarratorActive();
	bool blnHaveActiveProduct;
	ULONGLONG nActiveProductTime;
	HRESULT hrActiveProduct;
	ActiveProduct CachedProduct;

	// UI Automation notifications
	std::unique_ptr <UiaAnnouncer> pAnnouncer;

	//Dll variables and function pointers.
	//jaws.
	CComDispatchDriver JawsAPI;
	// True when this object's CoInitializeEx succeeded and needs a matching CoUninitialize
	bool blnComInitialised;
	bool blnComInitAttempted;
	
	//NVDA.
		boost::dll::shared_library NvdaDllApi;
		nvdaControllerTestIfRunningFunc TestIfRunning;
nvdaControllerCancelSpeechFunc CancelSpeech;
nvdaControllerSpeakTextFunc SpeakText;
nvdaControllerBrailleMessageFunc BrailleMessage;

//System access dll and function pointers.
boost::dll::shared_library SystemAccessDllApi;
SAIsRunningFunc SAIsRunning;
SABrailleFunc SABraille;
SASpeakFunc SASpeak;
SAStopAudioFunc SAStopAudio;

	/// <summary> 
	/// FShouldSpeak: variable to determine whether the application should speak.
	///</summary>
	///<remarks> 
	///This variable doesn't tell us whether the speech is muted according to the underlying access technology. If this is required this can be added at a later date.
	/// </remarks>
	HRESULT FShouldSpeak;
std::wstring JsdFile;
std::wstring HscFile;
	ProcessJsdFileType JsdFileToStartProcessing;
	
	//Functions to retrieve directories.
	HRESULT GetExecutablePath(fs::path& path);
	HRESULT GetCommonAppDataPath(fs::path &path);
	HRESULT GetCurrentUsersAppDataPath(fs::path &path);
	
	//Functions to perform string conversion.
	std::wstring NarrowStringToWideString(const std::string& stringToConvert);
	std::string WideStringToNarrowString(std::wstring& stringToConvert);
		
	//Function to get the windows user name.
	HRESULT GetCurrentUserName(std::wstring& userName);
	/// <summary> Determine whether JAWS For Windows is currently active in memory. </summary>
	/// <returns> returns S_OK if JAWS is active, S_FALSE otherwise. </returns>
bool IsJAWSActive();

	/// <summary>Loads the JAWS com api.</summery>
/// <remarks> the clsid we need to use is fixed in this case. it is FreedomSci.JawsApi. We use a CDispatchDriver class to help manage the IDispatch pointer and call methods apropriately--saves us dealing with the intricacies of things like GetIdsOfNames.</remarks>
bool LoadJAWSApi();

/// <summary> Unloads the JAWS api.</summery>
void UnloadJAWSApi(void);

/// <summary> Speaks a string of text from JAWS. </summery>
/// <param name="strText">The text to speak.</param>
	   /// <param name="blnSilence">Whether or not to silence the existing speech. True indicates the existing speech will be silenced, false indicates the speech will be queued as normal.</param>
	/// <returns> Returns true if the speech has been queued for speaking, false otherwise.</returns>
	HRESULT JAWSSpeak(std::wstring strText, BOOL blnSilence);

	/// <summary> Silences JAWS.</summary>
	/// <returns> Returns S_OK if the speech has been silenced, S_FALSE otherwise.</returns>
	HRESULT JAWSSilence(void);

	/// <summary> Brailles a string of text from JAWS. </summery>
/// <param name="strText">The text to braille.</param>
	/// <returns> Returns S_OK if the speech has been queued for brailleing, S_FALSE otherwise.</returns>
	HRESULT JAWSBraille(std::wstring strText);
/// <summary> list the available actions for JAWS. </summary>
	/// <remarks> This procedure lists the available actions for the JAWS screen reader. The following are the available action fields:
	/// 0 =Name,
	/// 1 =Description
	/// 2 =Synopsis
///3 =type <see cref="ScreenReaderActionType"/> Specific values of note here are ID_SCRIPT, and ID_FUNCTION.
	/// The right-hand side of the map is then populated with a Action info vector. </remarks>
	/// <param>The variable in which to place the actions and there field inforamtion. this should be passed by reference by the caller.</param>
	/// <returns>return t if the map is populated, fase otherwise. </returns>
HRESULT GetAvailableJAWSActions(AvailableActionsType& ActionInformation);
HRESULT ExecuteJAWSAction(std::wstring Action, ScreenReaderActionType Type);
inline ActionInfoType ProcessJSDElement(JawsFunction Element)	
{
	ActionInfoType NewAction;
	NewAction.insert(std::make_pair(0, Element.Name));
NewAction.insert(std::make_pair(1, Element.Synopsis));
NewAction.insert(std::make_pair(2, Element.Description));
NewAction.insert(std::make_pair(3, Element.Category));
NewAction.insert(std::make_pair(4, Element.Returns));
NewAction.insert(std::make_pair(5, Element.Parameters));
//Action.insert(std::make_pair(6, Element.OriginatingFolder));
	return NewAction;
}
/// <summary> Obtain the path to the current jaws executable.
	bool GetJAWSPath(fs::path& FileName);
	/// <summary> IsJAWSRoaming
	/// <remarks> Check to see whether jaws is roaming. To do this we check to see whether a settings folder exists in the folder from which jaws is running. </remarks>
	bool IsJAWSRoaming();
	/// <summary> Determine whether Hot Spot Clicker is installed.
	HSCInstallState IsHSCInstalled();
	/// <summary> Installs hotspot clicker 212.
	BOOL InstallHSC();
	/// <summary> Determine whether Non-Visual Desktop Access is currently active in memory. </summary>
	/// <returns> returns S_OK if Non-Visual Desktop Access is active, S_FALSE otherwise. </returns>
HRESULT IsNVDAActive();
/// <summary>Loads the NVDA dll.</summery>
/// <remarks> the dll that is required is dependent on the type of architecture (x86/x64) windows is running on. We use the IsProcess64Bit function from the WiX toolset for this.</remarks>
bool LoadNVDAApi();
/// <summary> Unloads the NVDA dll.</summery>
void UnloadNVDAApi(void);
/// <summary> Speaks a string of text from NVDA. </summery>
/// <param name="strText">The text to speak.</param>
	   /// <param name="blnSilence">Whether or not to silence the existing speech. True indicates the existing speech will be silenced, false indicates the speech will be queued as normal.</param>
	/// <returns> Returns S_OK if the speech has been queued for speaking, S_FALSE otherwise.</returns>
	HRESULT NVDASpeak(std::wstring strText, BOOL blnSilence);
/// <summary> Silences NVDA.</summary>
	/// <returns> Returns S_OK if the speech has been silenced, S_FALSE otherwise.</returns>
	HRESULT NVDASilence(void);
/// <summary> Brailles a string of text from NVDA. </summery>
/// <param name="strText">The text to braille.</param>
	/// <returns> Returns S_OK if the speech has been queued for brailleing, S_FALSE otherwise.</returns>
	HRESULT NVDABraille(std::wstring strText);
/// <summary> Determine whether System Access is currently active in memory. </summary>
	/// <returns> returns S_OK if System Access is active, S_FALSE otherwise. </returns>
bool IsSystemAccessActive();
/// <summary>Loads the System Access dll.</summery>
bool LoadSystemAccessApi();
/// <summary> Unloads the SystemAccess dll.</summery>
void UnloadSystemAccessApi(void);
/// <summary> Speaks a string of text from Sysetm Access. </summery>
/// <param name="strText">The text to speak.</param>
	   /// <param name="blnSilence">Whether or not to silence the existing speech. True indicates the existing speech will be silenced, false indicates the speech will be queued as normal.</param>
	/// <returns> Returns S_OK if the speech has been queued for speaking, S_FALSE otherwise.</returns>
	HRESULT SystemAccessSpeak(std::wstring strText, BOOL blnSilence);
/// <summary> Silences System Access.</summary>
	/// <returns> Returns S_OK if the speech has been silenced, S_FALSE otherwise.</returns>
	HRESULT SystemAccessSilence(void);
/// <summary> Brailles a string of text from System Access. </summery>
/// <param name="strText">The text to braille.</param>
	/// <returns> Returns S_OK if the string has been queued for brailleing, S_FALSE otherwise.</returns>
	HRESULT SystemAccessBraille(std::wstring strText);	
public:
	std::vector<JawsFunction> ProcessJSDFile(fs::path&File);
	/// <summary> loads the application programming interface for the active screen reader.</summery>
	/// <returns> S_OK if the API has loaded successfully and an error code otherwise.</returns>
	bool LoadAPI();
void UnloadAPI();
	/// <summary> 
	HRESULT IsHotSpotInSet(std::wstring SetName, std::wstring SpotName);
	HRESULT IsHotSpotInCurrentSet(std::wstring SpotName);
HRESULT ListHotSpotsInSet(std::wstring SetName, std::vector<std::string>& Spots);
HRESULT ListHotSpotsInCurrentSet(std::vector<std::string>& Spots);
HRESULT GetActiveHotSpotSet(std::wstring& ActiveSet);
HRESULT ExecuteHotSpot(std::wstring Set, std::wstring SpotName);
void SetFirstJsdFile(std::wstring );
std::wstring GetFirstJsdFile(void);
void ClearJsdFile();
void SetHscFile(std::wstring File);
std::wstring GetHscFile(void);
void ClearHscFile();
	
	/// <summary> Obtain the currently active speech product. </summary>
	/// <returns> return S_OK if a product was active S_FALSE otherwise. </returns>
	HRESULT GetActiveProduct(ActiveProduct& Product);
	
	/// Determine whether the application should speak anything.
	/// </summary>
	/// <returns> returns S_OK if the application should speak, S_FALSE otherwise. </returns>
	HRESULT IsMuted(void);
void Mute( bool blnNewMute);
		
	/// <summary> 
	///speak a string of text without silencing the currently speaking text if any is already speaking.
	/// </summary>
	   /// <param name="strText">The text to speak.</param>
	/// <returns> Returns S_OK if the speech has been queued for speaking, S_FALSE otherwise. </returns>
	HRESULT Speak(std::wstring strText);

	/// <summary> 
	///speak a string of text with an option to silence the currently speaking text if any is alreaky speaking.
	/// </summary>
	   /// <param name="strText">The text to speak.</param>
	   /// <param name="blnSilence">Whether or not to silence the existing speech. True indicates the existing speech will be silenced, false indicates the speech will be queued as normal.</param>
	/// <returns> Returns S_OK if the speech has been queued for speaking, S_FALSE otherwise. </returns>
	HRESULT Speak(std::wstring strText, BOOL blnSilence);
HRESULT Silence();
HRESULT GetAvailableActions(AvailableActionsType& ActionInformation);
HRESULT ExecuteAction(std::wstring Action, ScreenReaderActionType Type);

// Data members
bool blnMuted;
};


//Comparison functions.
inline bool CompareJawsFunctions(JawsFunction f1, JawsFunction f2)
{
	return f1.Name.compare(f2.Name) ==0;
}
inline bool IsNotVoid(JawsFunction f1)
{
	if (f1.Type ==ID_TYPE_SCRIPT)
		return true;
	else {
		return f1.Returns.DataType.compare(L"void") ==0;
	}
}

#endif  
