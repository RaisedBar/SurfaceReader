//RBSpeechWin.h
// speech utilities class header file.
//Copyright (�) 2012 Raised Bar Ltd.

#ifndef RB_SPEECH_WIN_H
#define RB_SPEECH_WIN_H

#pragma once

#include <windows.h>
#include <atlbase.h>
#include <Psapi.h>

//msi/wix includes.
#include <Msi.h>
#include "WIX Include.h"

// Dolphin includes
#ifndef _WIN64
#include "apiproto.h"
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

// Required constants for Dolphin products
const std::wstring wstrScreenRefresh = L"Screen Refresh";
const std::wstring wstrPassNextKey = L"Pass next key to application";
const std::wstring wstrToggleHotKeys = L"Toggle Hotkeys off and on";
const std::wstring wstrNavigationMode = L"Navigation Mode";
const std::wstring wstrDocumentReadMode = L"Document Read Mode";
const std::wstring wstrSwitchToPreviousLanguage = L"Switch to Previous Language";
const std::wstring wstrSwitchToNextLanguage = L"Switch to Next Language";
const std::wstring wstrVolumeUp = L"Volume Up";
const std::wstring wstrVolumeDown = L"Volume Down";
const std::wstring wstrToggleSpeech = L"Toggle Speech on off";
const std::wstring wstrToggleRowColumnMode = L"Toggle Row or Column Mode";
const std::wstring wstrReadFocus = L"Read Focus";
const std::wstring wstrSpellFocus = L"Spell Focus";
const std::wstring wstrVerboseFocus = L"Verbose Focus";
const std::wstring wstrPhoneticFocus = L"Phonetic Focus";
const std::wstring wstrReadTextToCursor = L"Read text To Cursor";
const std::wstring wstrSpellTextToCursor = L"Spell Text To Cursor";
const std::wstring wstrVerboseTextToCursor = L"Verbose Text To Cursor";
const std::wstring wstrPhoneticTextToCursor = L"Phonetic Text To Cursor";
const std::wstring wstrReadTextFromCursor = L"Read Text From Cursor";
const std::wstring wstrSpellTextFromCursor = L"Spell Text From Cursor";
const std::wstring wstrVerboseTextFromCursor = L"Verbose Text From Cursor";
const std::wstring wstrPhoneticTextFromCursor = L"Phonetic Text From Cursor";
const std::wstring wstrReadSpreadsheetRowAndColumn = L"Read Spreadsheet Row and Column";
const std::wstring wstrSpellSpreadsheetRowAndColumn = L"Spell Spreadsheet Row and Column";
const std::wstring wstrVerboseSpreadsheetRowAndColumn = L"Verbose Spreadsheet Row and Column";
const std::wstring wstrPhoneticSpreadsheetRowAndColumn = L"Phonetic Spreadsheet Row and Column";
const std::wstring wstrReadCharacter = L"Read Character";
const std::wstring wstrDescribeCharacterAttributes = L"Describe Character Attributes";
const std::wstring wstrPhoneticCharacter = L"Phonetic Character";
const std::wstring wstrReadWord = L"Read Word";
const std::wstring wstrSpellWord = L"Spell Word";
const std::wstring wstrVerboseWord = L"Verbose Word";
const std::wstring wstrPhoneticWord = L"Phonetic Word";
const std::wstring wstrReadLine = L"Read Line";
const std::wstring wstrSpellLine = L"Spell Line";
const std::wstring wstrVerboseLine = L"Verbose Line";
const std::wstring wstrPhoneticLine = L"Phonetic Line";
const std::wstring wstrReadWindow = L"Read Window";
const std::wstring wstrVerboseWindow = L"Verbose Window";
const std::wstring wstrWhereAmI = L"Where Am I";
const std::wstring wstrReadShortcut = L"Read Shortcut";
const std::wstring wstrReadControl = L"Read Control";
const std::wstring wstrVerboseControl = L"Verbose Control";
const std::wstring wstr1Mute = L"1	Mute";
const std::wstring wstrReadStatusBar = L"Read Status Bar";
const std::wstring wstrSpellStatusBar = L"Spell Status Bar";
const std::wstring wstrVerboseStatusBar = L"Verbose Status Bar";
const std::wstring wstrPhoneticStatusBar = L"Phonetic Status Bar";
const std::wstring wstrSpeedUp = L"Speed Up";
const std::wstring wstrSpeedDown = L"Speed Down";
const std::wstring wstrBrailleBack = L"Braille back";
const std::wstring wstrBrailleForward = L"Braille forward";
const std::wstring wstrBrailleLeft = L"Braille left";
const std::wstring wstrBrailleRight = L"Braille right";
const std::wstring wstrBrailleWidthLeft = L"Braille width Left";
const std::wstring wstrBrailleWidthRight = L"Braille width Right";
const std::wstring wstrBrailleHalfWidthLeft = L"Braille half Width Left";
const std::wstring wstrBrailleHalfWidthRight = L"Braille half Width Right";
const std::wstring wstrBrailleGoToFocus = L"Braille go To Focus";
const std::wstring wstrBrailleHome = L"Braille home";
const std::wstring wstrBrailleEnd = L"Braille end";
const std::wstring wstrToggleBraille = L"Toggle Braille on off";
const std::wstring wstrDisplayCharacters = L"Display Characters";
const std::wstring wstrCycleAttributes = L"Cycle Attributes";
const std::wstring wstrDisplayFontSize = L"Display Font Size";
const std::wstring wstrDisplayFontStyle = L"Display Font Style";
const std::wstring wstrDisplayFontColours = L"Display Font Colours";
const std::wstring wstrDisplayFontForegroundColour = L"Display Font Foreground Colour";
const std::wstring wstrDisplayFontName = L"Display Font Name";
const std::wstring wstrDescribeCharacter = L"Toggle Describe Character on off";
const std::wstring wstrToggleCursor = L"Toggle Cursor on off";
const std::wstring wstrToggleTracking = L"Toggle tracking on off";
const std::wstring wstrBrailleLineDown = L"Braille line down";
const std::wstring wstrBrailleLineUp = L"Braille line up";
const std::wstring wstrBrailleBottom = L"Braille bottom";
const std::wstring wstrTBrailleTP = L"TBraille tp";
const std::wstring wstrToggleLayoutMode = L"Toggle Layout Mode on off";
const std::wstring wstrToggleLiteraryBraille = L"Toggle Literary Braille on off";
const std::wstring wstrTogglePhysicalMode = L"Toggle Physical Mode on off";
const std::wstring wstrIncreaseMag = L"Increase Magnification";
const std::wstring wstrDecreaseMag = L"Decrease Magnification";
const std::wstring wstrIncrementXMag = L"Increment X Magnification";
const std::wstring wstrDecrementXMag = L"Decrement X Magnification";
const std::wstring wstrIncrementYMag = L"Increment Y Magnification";
const std::wstring wstrDecrementYMag = L"Decrement Y Magnification";
const std::wstring wstrToggleMag = L"Toggle Magnification";
const std::wstring wstrToggleColourChanger = L"Toggle Colour changer";
const std::wstring wstrToggleOverviewMode = L"Toggle Overview mode";
const std::wstring wstrCyclePrimaryAreaType = L"Cycle Primary Area type";
const std::wstring wstrCycleOrientation = L"Cycle Orientation";
const std::wstring wstrImageSmoothing = L"Image Smoothing";
const std::wstring wstrToggleMagTracking = L"Toggle Tracking";
const std::wstring wstrDisplayHookedAreas = L"Display Hooked Areas";
const std::wstring wstrLineViewWholeWindows = L"Line View Whole Windows";


/* Speech utilities class allowing other applications to output speech to any supported access technology. 
The following access technologies arre supported:
JAWS For Windows (version 8 or later).
Any product from Dolphin Computer Access.
System Access.
Non-Visual Desktop Access.
*/

class RBSpeech
{
public:
	RBSpeech(void);
	~RBSpeech(void);

private:
	//Dll variables and function pointers.
	//jaws.
	CComDispatchDriver JawsAPI;
	
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
	HRESULT GetExecutablePath(std::filesystem::path& path);
	HRESULT GetCommonAppDataPath(std::filesystem::path &path);
	HRESULT GetCurrentUsersAppDataPath(std::filesystem::path &path);
	
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
	HRESULT JAWSSpeak(wstring strText, BOOL blnSilence);

	/// <summary> Silences JAWS.</summary>
	/// <returns> Returns S_OK if the speech has been silenced, S_FALSE otherwise.</returns>
	HRESULT JAWSSilence(void);

	/// <summary> Brailles a string of text from JAWS. </summery>
/// <param name="strText">The text to braille.</param>
	/// <returns> Returns S_OK if the speech has been queued for brailleing, S_FALSE otherwise.</returns>
	HRESULT JAWSBraille(wstring strText);
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
	bool GetJAWSPath(std::filesystem::path& FileName);
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
	HRESULT NVDASpeak(wstring strText, BOOL blnSilence);
/// <summary> Silences NVDA.</summary>
	/// <returns> Returns S_OK if the speech has been silenced, S_FALSE otherwise.</returns>
	HRESULT NVDASilence(void);
/// <summary> Brailles a string of text from NVDA. </summery>
/// <param name="strText">The text to braille.</param>
	/// <returns> Returns S_OK if the speech has been queued for brailleing, S_FALSE otherwise.</returns>
	HRESULT NVDABraille(wstring strText);
//All dolphin functions for dolphin requires us to be 32-bit.
/// <summary> Determine whether Dolphin is currently active in memory. </summary>
	/// <returns> returns S_OK if Dolphin is active, S_FALSE otherwise. </returns>
bool IsDolphinActive(DolphinProduct& SpecificProduct);
/// <summary>Loads the Dolphin dll.</summery>
bool LoadDolphinApi();
/// <summary> Unloads the Dolphin dll.</summery>
void UnloadDolphinApi(void);
/// <summary> Speaks a string of text from Dolphin. </summery>
/// <param name="strText">The text to speak.</param>
	   /// <param name="blnSilence">Whether or not to silence the existing speech. True indicates the existing speech will be silenced, false indicates the speech will be queued as normal.</param>
	/// <returns> Returns S_OK if the speech has been queued for speaking, E_NOTIMPL if the function is not supported and S_FALSE otherwise.</returns>
	HRESULT DolphinSpeak(wstring strText, BOOL blnSilence, DolphinReturnCode& ReturnCode);
/// <summary> Silences Dolphin.</summary>
	/// <returns> Returns S_OK if the speech has been silenced, S_FALSE otherwise.</returns>
	HRESULT DolphinSilence(DolphinReturnCode& ReturnCode);
/// <summary> Brailles a string of text from Dolphin. </summery>
/// <param name="strText">The text to braille.</param>
	/// <returns> Returns S_OK if the speech has been queued for brailleing, E_NOTIMPL if the function is not available and S_FALSE otherwise.</returns>
	HRESULT DolphinBraille(wstring strText);	
	/// <Summary> Obtain a list of dolphin actions. </summary>
	HRESULT GetAvailableDolphinActions(AvailableActionsType& ActionInformation);
	HRESULT ExecuteDolphinAction(const boost::any& name, const ScreenReaderActionType& type, DWORD& ReturnCode);
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
	HRESULT SystemAccessSpeak(wstring strText, BOOL blnSilence);
/// <summary> Silences System Access.</summary>
	/// <returns> Returns S_OK if the speech has been silenced, S_FALSE otherwise.</returns>
	HRESULT SystemAccessSilence(void);
/// <summary> Brailles a string of text from System Access. </summery>
/// <param name="strText">The text to braille.</param>
	/// <returns> Returns S_OK if the string has been queued for brailleing, S_FALSE otherwise.</returns>
	HRESULT SystemAccessBraille(wstring strText);	
public:
	std::vector<JawsFunction> ProcessJSDFile(std::filesystem::path&File);
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
	HRESULT GetActiveProduct(ActiveProduct& Product, DolphinProduct& SpecificDolphinProduct);
	
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
	HRESULT Speak(wstring strText, DolphinReturnCode& ReturnCode);

	/// <summary> 
	///speak a string of text with an option to silence the currently speaking text if any is alreaky speaking.
	/// </summary>
	   /// <param name="strText">The text to speak.</param>
	   /// <param name="blnSilence">Whether or not to silence the existing speech. True indicates the existing speech will be silenced, false indicates the speech will be queued as normal.</param>
	/// <returns> Returns S_OK if the speech has been queued for speaking, S_FALSE otherwise. </returns>
	HRESULT Speak(wstring strText, BOOL blnSilence, DolphinReturnCode& ReturnCode);
HRESULT Silence(DolphinReturnCode& ReturnCode);
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
