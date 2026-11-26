// RBSpeech.cpp

#include "RBSpeech.h"

#ifdef __WINDOWS__ 
#include <TlHelp32.h>
#include <Processthreadsapi.h> // on Windows 8 and Windows Server 2012
#include <comutil.h>
#include <shlobj.h>
#include <vector>
#include <algorithm>
#include <locale>

#include "UiaAnnouncer.h"

bool FindProcessByName(const wchar_t * wstrProcessName)
{
	PROCESSENTRY32 entry;
	entry.dwSize = sizeof(PROCESSENTRY32);
		HANDLE snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, NULL);

	if (Process32First(snapshot, &entry) == TRUE)
	{
		while (Process32Next(snapshot, &entry) == TRUE)
		{
			if (_tcsicmp(entry.szExeFile, wstrProcessName) == 0)
			{
				CloseHandle(snapshot);
				return true;
			}
		}
	}

	CloseHandle(snapshot);
	return false;
}

// How long a screen reader check is trusted before Speak checks again.
// Long enough that a burst of surface updates doesn't rescan, short enough that
// starting or switching screen reader is picked up almost at once.
const ULONGLONG nActiveProductCacheMs = 3000;


RBSpeech::RBSpeech(void)
	: blnHaveActiveProduct( false),
	nActiveProductTime( 0),
	hrActiveProduct( S_FALSE),
	CachedProduct( ID_NONE),
	pAnnouncer( new UiaAnnouncer()),
	blnComInitialised( false),
	blnComInitAttempted( false),
	FShouldSpeak( S_OK),
	JsdFileToStartProcessing( PROCESS_NO_FILE),
	blnMuted( false)
{
}


RBSpeech::~RBSpeech(void)
{
	// Release the JAWS object before COM is uninitialised
	JawsAPI.Release();

	if (blnComInitialised)
	{
		CoUninitialize();
		blnComInitialised = false;
	}
}


void RBSpeech::SetAnnounceWindow( HWND hWindow)
{
	pAnnouncer->SetWindow( hWindow);
}
	

//JAWS specific private functions.
bool RBSpeech::IsJAWSActive()
{
	// if (FindWindow(L"JFWUI2", L"JAWS"))
	if (FindProcessByName(_T("jfw.exe")))
	{
		if (LoadJAWSApi())
		{ //JAWS active.
			return true;
		} //end JAWS active block.
		else
		{
			return false;
		}
	}
		else
{
	return false;
}
	}

bool RBSpeech::LoadJAWSApi()
{
	// Initialise COM once for this object, not on every call: each successful
	// CoInitializeEx needs its own CoUninitialize, and only one was ever made.
	if (blnComInitAttempted == false)
	{
		HRESULT hrInit = CoInitializeEx(0, COINIT_APARTMENTTHREADED);
		// S_OK and S_FALSE both need a matching CoUninitialize. RPC_E_CHANGED_MODE means COM
		// is already initialised differently on this thread: usable, but not ours to uninitialise.
		blnComInitialised = SUCCEEDED( hrInit);
		blnComInitAttempted = true;
	}

	// Reuse the JAWS object rather than creating a new one for every utterance
	if (JawsAPI.p != NULL)
	{
		return true;
	}

//Now obtain an instance of the required co class.
CComPtr<IDispatch> lpTDispatch;
HRESULT hr= lpTDispatch.CoCreateInstance(_bstr_t(L"FreedomSci.JawsApi"));
if (hr == S_OK)
{
	JawsAPI = lpTDispatch;
	return true;
}
return false;
}

void RBSpeech::UnloadJAWSApi(void)
{

	//Release the JAWS API com object.
	JawsAPI.Release();
	//Now uninitialize com, if this object initialised it.
	if (blnComInitialised)
	{
		CoUninitialize();
		blnComInitialised = false;
	}
	blnComInitAttempted = false;
	return;
}


// True when a COM call failed because the server went away (for example JAWS was restarted)
static bool IsComServerGone( HRESULT hr)
{
	return ((hr == RPC_E_DISCONNECTED)
		|| (hr == CO_E_OBJNOTCONNECTED)
		|| (hr == RPC_E_SERVER_DIED)
		|| (hr == RPC_E_SERVER_DIED_DNE)
		|| (hr == HRESULT_FROM_WIN32( RPC_S_SERVER_UNAVAILABLE))
		|| (hr == HRESULT_FROM_WIN32( RPC_S_CALL_FAILED)));
}
	
HRESULT RBSpeech::JAWSSpeak(std::wstring strText, BOOL blnSilence)
{
	HRESULT hr =S_OK;
	CComVariant vFunctionResult;
/*
std::wstring wstrFunctionCall = L"Say( ";
wstrFunctionCall.append( wstrDoubleQuotes);
wstrFunctionCall.append( strText);
wstrFunctionCall.append( wstrDoubleQuotes);
wstrFunctionCall.append( L", 1)");
// wstrFunctionCall.append( L", OT_CONTROL_TYPE)");
*/

	//check to see that the message to be spoken actually contains some text.
ExitOnTrue(strText.empty(), hr, __HRESULT_FROM_WIN32(ERROR_BAD_ARGUMENTS), "No text has been specified.");

{
	CComVariant vText( strText.c_str());
	CComVariant vSilence( blnSilence);
LoadJAWSApi();
	hr 	= JawsAPI.Invoke2(_bstr_t(L"SayString"), &vText, &vSilence, &vFunctionResult);

	if (IsComServerGone( hr))
	{
		// JAWS has been restarted since the object was created: make a new one and try once more
		JawsAPI.Release();
		if (LoadJAWSApi())
		{
			vFunctionResult.Clear();
			hr = JawsAPI.Invoke2(_bstr_t(L"SayString"), &vText, &vSilence, &vFunctionResult);
		}
	}
}
	ExitOnFailure(hr, "Executing the JAWS SayString function returned a failure.");
	ExitOnFalse(vFunctionResult.vt == VT_BOOL, hr, S_FALSE, "The JAWS SayString function should return a boolean.");
	ExitOnFalse(vFunctionResult.boolVal, hr, S_FALSE, "The JAWS SayString function could not schedule the speech.");
	LExit:	
return hr;
}
	
HRESULT RBSpeech::JAWSSilence(void)
	{
HRESULT hr =S_OK;
	CComVariant vFunctionResult;
	//Call jaws.
	hr =JawsAPI.Invoke0(_bstr_t("StopString"), &vFunctionResult);
	ExitOnFailure(hr, "Executing the JAWS StopString function returned a failure.");
	ExitOnFalse(vFunctionResult.vt == VT_BOOL, hr, S_FALSE, "The JAWS StopString function should return a boolean.");
	ExitOnFalse(vFunctionResult.boolVal, hr, S_FALSE, "The JAWS StopString function could not schedule the speech.");
LExit:
	return hr;
	}
	
HRESULT RBSpeech::JAWSBraille(std::wstring strText)
{ 
HRESULT hr =S_OK;
	CComVariant vFunctionResult;
	std::wstring FunctionStr =L"BrailleMessage(";
//Check to see that the message to be spoken actually contains some text.
ExitOnTrue(strText.empty(), hr, __HRESULT_FROM_WIN32(ERROR_BAD_ARGUMENTS), "No text has been specified.");
//now, append to the FunctionStr.
	FunctionStr.append(strText);
	FunctionStr.append(L", 0, 2000)");
	LoadJAWSApi();
	hr = JawsAPI.Invoke1(_bstr_t(L"RunFunction"), &_variant_t(FunctionStr.c_str()), &vFunctionResult);
	ExitOnFailure(hr, "Executing the JAWS RunFunction function returned a failure.");
	ExitOnFalse(vFunctionResult.vt == VT_BOOL, hr, S_FALSE, "The JAWS RunFunction function should return a boolean.");
	ExitOnFalse(vFunctionResult.boolVal, hr, S_FALSE, "The JAWS RunFunction function could not schedule the speech.");
LExit:
	return hr;
}
	
HRESULT RBSpeech::GetAvailableJAWSActions(AvailableActionsType& ActionInformation)
{
HRESULT hReturnValue =S_OK;
AvailableActionsType JawsActions;
ActionCollectionType Actions;
ActionCollectionTypeIterator NewEnd =Actions.begin();
AvailableActionFieldsType Fields = { {L"Name", 0}, {L"Synopsis", 1}, {L"Description", 2}, {L"Returns", 3}, {L"Parameters", 4}, {L"Category", 5}, {L"Type", 6} };
ActionInfoType CurrentAction;
std::vector<JawsFunction> AvailableJawsFunctions;
std::wstring UserScriptFolder; 
std::wstring SharedScriptFolder; 
std::wstring DefaultScriptFile; 
std::wstring ApplicationScriptFile; 
fs::path JSDFile;
			boost::property_tree::ptree IniTree; //used to store environment information.
			std::wstring JAWSFunctionCallString =L"GetCurrentJAWSEnvironment(\"%s\")"; //used to hold the call to JAWS.
			fs::path IniFile; //file to store the hsc information.
			ActiveProduct CurrentProduct;
	hReturnValue =GetActiveProduct( CurrentProduct);
	ExitOnFailure(hReturnValue, "No product is active.");
				switch(CurrentProduct)
			{
			case ID_JAWS:
				if (JsdFileToStartProcessing !=PROCESS_NO_FILE) 					
				{
//Process files based on the enum.
					int CurrentFileBeingProcessed;
					std::wstring TestDir;
		fs::path CurrentFile;
					int FreedomScientificDirectoryPosition =-1;
					std::vector<std::wstring> tokens;
					fs::path applicationConfigDir;
					hReturnValue = GetCommonAppDataPath(applicationConfigDir);
					for(CurrentFileBeingProcessed =JsdFileToStartProcessing; CurrentFileBeingProcessed <=PROCESS_DEFAULT_SYSTEM_DEFAULT_FILE; CurrentFileBeingProcessed++)
					{
						std::vector<JawsFunction> LocalFunctions;
						switch(CurrentFileBeingProcessed)
{
case PROCESS_NO_FILE:
	//should never execute.
	break;
case PROCESS_USER_APP_FILE:
	if (JsdFileToStartProcessing ==CurrentFileBeingProcessed)
	{ //we don't need to do anything as we already have the jsd file.
		CurrentFile =JsdFile;
	}
	OutputDebugString(JsdFile.c_str());
	if (exists(CurrentFile))
{
LocalFunctions =ProcessJSDFile(CurrentFile);
//Merge with the already available functions.
AvailableJawsFunctions.insert(AvailableJawsFunctions.end(), LocalFunctions.begin(), LocalFunctions.end());
	} //end parsing jsd file.
	break;
case PROCESS_DEFAULT_APP_FILE:
if (JsdFileToStartProcessing ==CurrentFileBeingProcessed)
	{ //we don't need to do anything as we already have the jsd file.
		CurrentFile =JsdFile;
}
else { //specify the file ourselves.
FreedomScientificDirectoryPosition =JsdFile.find(L"Freedom Scientific");	
TestDir = applicationConfigDir;
TestDir.append(JsdFile.substr(JsdFile.length()-FreedomScientificDirectoryPosition));
OutputDebugString(TestDir.c_str());
CurrentFile = TestDir;
} //end file processing.
if (exists(CurrentFile))
{
LocalFunctions =ProcessJSDFile(CurrentFile);
//Merge with the already available functions.
AvailableJawsFunctions.insert(AvailableJawsFunctions.end(), LocalFunctions.begin(), LocalFunctions.end());
} //end parsing jsd file.
break;
case PROCESS_USER_SYSTEM_DEFAULT_FILE:
if (JsdFileToStartProcessing ==CurrentFileBeingProcessed)
	{ //we don't need to do anything as we already have the jsd file.
		CurrentFile =JsdFile;
	}
else { //specifically set the file/path.
	//obtain the current user path.
	fs::path userConfigDir;
	hReturnValue = GetCurrentUsersAppDataPath(userConfigDir);
	TestDir = userConfigDir;
	TestDir.append(L"\\");
	FreedomScientificDirectoryPosition =JsdFile.find(L"Freedom Scientific");	
 TestDir.append(JsdFile.substr(JsdFile.length()-FreedomScientificDirectoryPosition));
 tokens.clear();
 boost::split(tokens, JsdFile, boost::is_any_of("\\"));
 TestDir.erase(TestDir.find(*end(tokens), FreedomScientificDirectoryPosition));
 TestDir.append(L"default.jsd");
 OutputDebugString(TestDir.c_str());
 CurrentFile=TestDir;
} //end specifically setting the filename.
if (exists(CurrentFile))
{
LocalFunctions =ProcessJSDFile(CurrentFile);
//Merge with the already available functions.
AvailableJawsFunctions.insert(AvailableJawsFunctions.end(), LocalFunctions.begin(), LocalFunctions.end());
} //end parsing jsd file.
break;
case PROCESS_DEFAULT_SYSTEM_DEFAULT_FILE:
	if (JsdFileToStartProcessing ==CurrentFileBeingProcessed)
	{ //we don't need to do anything as we already have the jsd file.
		CurrentFile =JsdFile;
	}
else { //specify the file ourselves.
FreedomScientificDirectoryPosition =JsdFile.find(L"Freedom Scientific");	
TestDir = applicationConfigDir;
TestDir.append(JsdFile.substr(JsdFile.length()-FreedomScientificDirectoryPosition));
tokens.clear();
boost::split(tokens, JsdFile, boost::is_any_of("\\"));
TestDir.erase(TestDir.find(*end(tokens), FreedomScientificDirectoryPosition));
 TestDir.append(L"default.jsd");
OutputDebugString(TestDir.c_str());
CurrentFile =TestDir;
} //end specifying file.
if (exists(CurrentFile))
{
LocalFunctions =ProcessJSDFile(CurrentFile);
//Merge with the already available functions.
AvailableJawsFunctions.insert(AvailableJawsFunctions.end(), LocalFunctions.begin(), LocalFunctions.end());
} //end parsing jsd file.
	break;
default:
				break;
					};
					} //end for loop
				} //end jsd file processing.
else { //use hsc method.
} //end hsc method.
//now sort the function vector.
//std::sort(AvailableJawsFunctions.begin(), AvailableJawsFunctions.end(), CompareJawsFunctions);
//now iterate through and add to the vector.
for (JawsFunction j : AvailableJawsFunctions)
{
if (j.Type ==ID_TYPE_SCRIPT)
{
	CurrentAction.clear();
	// CurrentAction = boost::assign::map_list_of(0, j.Name)(1, j.Synopsis)(2, j.Description)(5, j.Category);
	CurrentAction.insert(std::make_pair(0, j.Name));
	CurrentAction.insert(std::make_pair(1, j.Synopsis));
	CurrentAction.insert(std::make_pair(2, j.Description));
	CurrentAction.insert(std::make_pair(3, j.Returns));	
	CurrentAction.insert(std::make_pair(4, j.Parameters));
	CurrentAction.insert( std::make_pair( 5, j.Category));
	CurrentAction.insert(std::make_pair(6, ID_SCRIPT));
	Actions.push_back(CurrentAction);
}
	else if ((j.Type ==ID_TYPE_FUNCTION) && (j.Returns.DataType.compare(L"void") ==0))
{
		CurrentAction.clear();
		//  CurrentAction = boost::assign::map_list_of(0, j.Name)(1, j.Synopsis)(2, j.Description)(5, j.Category);

		CurrentAction.insert( std::make_pair( 0, j.Name));

	CurrentAction.insert(std::make_pair(3, j.Returns));	
	CurrentAction.insert(std::make_pair(4, j.Parameters));
	CurrentAction.insert(std::make_pair(6, ID_FUNCTION));
	Actions.push_back(CurrentAction);
}
} //end foreach.
JawsActions =std::make_pair(Fields, Actions);
ActionInformation =JawsActions;
break;
default:
hReturnValue =E_NOTIMPL;
break;
				};
LExit:
return hReturnValue;
}

HRESULT RBSpeech::ExecuteJAWSAction(std::wstring Action, ScreenReaderActionType Type)
{
HRESULT hr =S_FALSE;
CComVariant vFunctionResult;
LoadJAWSApi();
if (Type == ID_SCRIPT)
{
	hr = JawsAPI.Invoke1(_bstr_t(L"RunScript"), &_variant_t(Action.c_str()), &vFunctionResult);
	ExitOnFailure(hr, "Executing the JAWS RunScript function returned a failure.");
}
else if (Type == ID_FUNCTION)
{
	hr = JawsAPI.Invoke1(_bstr_t(L"RunFunction"), &_variant_t(Action.c_str()), &vFunctionResult);
	ExitOnFailure(hr, "Executing the JAWS RunFunction function returned a failure.");
}
ExitOnFalse(vFunctionResult.vt == VT_BOOL, hr, S_FALSE, "Executing a JAWS action should return a boolean.");
ExitOnFalse(vFunctionResult.boolVal, hr, S_FALSE, "The requested action execution could not be scheduled.");
LExit:
return hr;
}

	bool RBSpeech::GetJAWSPath(fs::path& FileName)
	{
		bool blnReturnValue =false;
		fs::path InternalPath;
		if (IsJAWSActive())
		{ //JAWS is active.
			//now obtain the JAWS window.
			HWND JawsWindow =NULL;
	JawsWindow =FindWindow(L"JFW",NULL);
	//now get the process id.
		DWORD ProcessId, WindowThread;
		WindowThread =GetWindowThreadProcessId(JawsWindow, &ProcessId);
		//now we need to open the process.
	HANDLE Process =NULL;
	Process =OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, false, ProcessId);
if (Process !=NULL)
{ //the process was opened successfully.
	            TCHAR szModName[MAX_PATH];
            // Get the full path to the module's file.
DWORD ProcessResult =GetModuleFileNameEx( Process, 0, szModName, sizeof(szModName) / sizeof(TCHAR));
if (ProcessResult !=0)
{ //the process was obtained successfully.
	InternalPath =szModName;
	blnReturnValue =true;
} //the process path was obtained successfully.
} //end process opened successfully block.
		} //End JAWS active block.
		FileName =InternalPath;
		return blnReturnValue;
	}
	
bool RBSpeech::IsJAWSRoaming()
	{
		bool blnReturnValue =false;
		fs::path JAWSPath;
		if (GetJAWSPath(JAWSPath) ==true)
		{ //we have the jaws path.
			JAWSPath.remove_filename();
			JAWSPath /=L"settings";
			JAWSPath /=L"enu";
			if (exists(JAWSPath))
			{ //JAWS is roaming.
blnReturnValue =true;
			} //end JAWS is roaming block.
		} //end we have jaws path block.
		return blnReturnValue;
	}
	
	HSCInstallState RBSpeech::IsHSCInstalled()
	{
		HSCInstallState eReturnValue =STATE_NOT_INSTALLED;
		//first check whether an older version--pre 211 is installed.
		//the only reliable way to check this is using a registry key check.
		// Hot Spot Clicker 211 and earlier were 32-bit, so read the 32-bit registry view:
		// KEY_WOW64_32KEY finds the key under Wow6432Node on 64-bit Windows, from both
		// 32-bit and 64-bit builds, and is ignored on 32-bit Windows.
		// (This used to compare the OS version with Windows XP and with Vista exactly,
		// so on Windows 7 and later neither check ran.)
		{
			HKEY reg = NULL;
			LONG nErr = RegOpenKeyExW(HKEY_LOCAL_MACHINE, L"SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Uninstall\\HotSpotClicker", 0, KEY_READ | KEY_WOW64_32KEY, &reg);
			if (nErr == ERROR_SUCCESS)
			{ //Key is valid so hsc 211 is installed.
				eReturnValue =STATE_OLDER_VERSION_INSTALLED;
				// Registry keys must be closed with RegCloseKey; CloseHandle left them open
				RegCloseKey( reg);
			} //end key is valid block.
		}
//now that we have checked for hsc 211 and below, we can look for 212 and above.
//to do this we use the msi (windows installer) functions and the hsc product code--you can find this in the hsc installer itsself.
if (MsiQueryProductStateW(L"c1093f38-9866-431e-a942-f786a1b8e8c0") ==INSTALLSTATE_DEFAULT)
{ //v212 or later is insstalled.
	eReturnValue =STATE_INSTALLED;
} //end v212 installed block.
			return eReturnValue;
	}
		
	BOOL RBSpeech::InstallHSC()
		{
			BOOL blnReturnValue =false;
		
			return blnReturnValue;
		}

bool RBSpeech::LoadNVDAApi()
{
	HRESULT hr = S_OK;
	if (NvdaDllApi.is_loaded())
	{
		return true;
	}

	fs::path NVDADllFileName;
	 hr =GetExecutablePath(NVDADllFileName);
	if (hr != S_OK)
	{
		return false;
	}
	// The DLL sits next to the executable. Without this the DLL name was appended to the
	// executable's own path (...\SurfaceReader.exe\nvdaControllerClient64.dll), so NVDA never loaded.
	NVDADllFileName.remove_filename();

	NVDADllFileName /= L"nvdaControllerClient64.dll";

			if (exists(NVDADllFileName))
{
	//file exists, so try to load it.
				NvdaDllApi.load(NVDADllFileName.generic_wstring());
				if (NvdaDllApi.is_loaded()) 
	{ //successfully loaded.
					TestIfRunning = NvdaDllApi.get<nvdaControllerTestIfRunningFunc>("nvdaController_testIfRunning");
					SpeakText = NvdaDllApi.get<nvdaControllerSpeakTextFunc>("nvdaController_speakText");
					BrailleMessage = NvdaDllApi.get<nvdaControllerBrailleMessageFunc>("nvdaController_brailleMessage");
					CancelSpeech = NvdaDllApi.get<nvdaControllerCancelSpeechFunc>("nvdaController_cancelSpeech");
return true;
	}    // end if loaded
}  // end if file not OK, or doesn't exist

//problem with initialization.
return false;
			 }

void RBSpeech::UnloadNVDAApi(void)
{
	//unload the nvda dll and free all functions.
	NvdaDllApi.unload();
return;
}

HRESULT RBSpeech::NVDASpeak(std::wstring strText, BOOL blnSilence)
{
	HRESULT hReturnValue =S_OK;
	//check to see that the message to be spoken actually contains some text.
ExitOnTrue(strText.empty(), hReturnValue, __HRESULT_FROM_WIN32(ERROR_BAD_ARGUMENTS), "No text has been specified.");
	//Check to see if we should silence.
if (blnSilence)
				{ //Silence speech.
					hReturnValue =NVDASilence();
					ExitOnFailure(hReturnValue, "Silencing NVDA failed.");
} //end silence check.
			ExitOnNull(SpeakText, hReturnValue, S_FALSE, "The speech text function is null, please call ActiveProduct.");
		ExitIfValueGreaterThanSuppliedValue(SpeakText(strText.c_str()), 0, hReturnValue, S_FALSE, "Speaking through NVDA failed.");
		LExit:
		return hReturnValue;
}

HRESULT RBSpeech::NVDASilence(void)
{
	HRESULT hReturnValue =S_OK;
	ExitOnNull(CancelSpeech, hReturnValue, S_FALSE, "The cancel speech function is null, please call ActiveProduct.");
	ExitIfValueGreaterThanSuppliedValue(CancelSpeech(), 0, hReturnValue, S_FALSE, "Silencing NVDA failed.");	
LExit:
return hReturnValue;
}
	
HRESULT RBSpeech::NVDABraille(std::wstring strText)
{
HRESULT hReturnValue =S_OK;
//check to see that the message to be brailled actually contains some text.
ExitOnTrue(strText.empty(), hReturnValue, __HRESULT_FROM_WIN32(ERROR_BAD_ARGUMENTS), "No text has been specified.");
			ExitOnNull(BrailleMessage, hReturnValue, S_FALSE, "The braille message function is null, please call ActiveProduct.");
		ExitIfValueGreaterThanSuppliedValue(BrailleMessage(strText.c_str()), 0, hReturnValue, S_FALSE, "Brailleing through NVDA failed.");
		LExit:
		return hReturnValue;			
}

//System Access.
/// <summary> Determine whether System Access is currently active in memory. </summary>
	/// <returns> returns true if System Access is active, false otherwise. </returns>
bool  RBSpeech::IsSystemAccessActive()
{
	HRESULT hReturnValue =S_OK;
	try
	{
		//first try and load the system access api.
		hReturnValue = LoadSystemAccessApi();
		// ExitOnFailure(hReturnValue, "Unable to load the system access api.");
		// ExitOnFalse(SAIsRunning(), hReturnValue, S_FALSE, "System Access is not running.");
	// LExit:
		if (hReturnValue == S_OK)
		{
			return true;
		}
		else
		{
			return false;
		}
	}
	catch (...)
	{
		return false;
	}
	}

bool RBSpeech::LoadSystemAccessApi()
{
	bool result = false;
	HRESULT hr = S_OK;

	if (!SystemAccessDllApi.is_loaded())
{
		fs::path SystemAccessDllFileName;
hr =GetExecutablePath(SystemAccessDllFileName); //assign the executable directory.
		if (hr != S_OK)
		{
			return false;
		}
		SystemAccessDllFileName.remove_filename();

		SystemAccessDllFileName /= L"SAAPI64.dll";
		
		if (exists(SystemAccessDllFileName))
		{ 
			SystemAccessDllApi.load(SystemAccessDllFileName.generic_string());
			if (SystemAccessDllApi.is_loaded())
			{
				SAIsRunning =SystemAccessDllApi.get<SAIsRunningFunc>("SA_IsRunning");
				SASpeak = SystemAccessDllApi.get<SASpeakFunc>("SA_SayW");
				SABraille = SystemAccessDllApi.get<SABrailleFunc>("SA_BrlShowTextW");
				SAStopAudio = SystemAccessDllApi.get<SAStopAudioFunc>("SA_StopAudio");
				result = true;
			}
		}
} //Dll not loaded.
	
	return result;
}


	void RBSpeech::UnloadSystemAccessApi(void)
{
	SystemAccessDllApi.unload();
	return;
}
	
HRESULT RBSpeech::SystemAccessSpeak(std::wstring strText, BOOL blnSilence)
{
HRESULT hReturnValue =S_OK;
	//check to see that the message to be spoken actually contains some text.	
ExitOnTrue(strText.empty(), hReturnValue, S_FALSE, "No text is available to speak through System Access.");
if (blnSilence)
{ //Try and silence.
	hReturnValue =SystemAccessSilence();
	ExitOnFailure(hReturnValue, "Unable to Silence System Access.");
} //end silence block.
ExitOnNull(SASpeak, hReturnValue, S_FALSE, "The system access speak function is not available. Please call Active Product.");
ExitOnFalse(SASpeak(strText.c_str()), hReturnValue, S_FALSE, "System Access speech failed.");
LExit:	
	return hReturnValue;
}
	
	HRESULT RBSpeech::SystemAccessSilence(void)
{
	HRESULT hReturnValue =S_OK;
	ExitOnNull(SAStopAudio, hReturnValue, S_FALSE, "Stopping speech via System Access is not available. Please call Active Product.");
	ExitOnFalse(SAStopAudio(), hReturnValue, S_FALSE, "Speaking through System Access failed.");
LExit:
	return hReturnValue;
}
	
	HRESULT RBSpeech::SystemAccessBraille(std::wstring strText)
{
HRESULT hReturnValue =S_OK;
	//check to see that the message to be spoken actually contains some text.	
ExitOnTrue(strText.empty(), hReturnValue, S_FALSE, "No text is available to speak through System Access.");
ExitOnNull(SABraille, hReturnValue, S_FALSE, "The system access braille function is not available. Please call Active Product.");
ExitOnFalse(SABraille(strText.c_str()), hReturnValue, S_FALSE, "System Access speech failed.");
LExit:	
	return hReturnValue;
	}

	
	//public functions.

	
	HRESULT RBSpeech::GetActiveProduct(ActiveProduct& Product)
{
	// Checking means scanning every running process and loading each screen reader's API,
	// which is far too slow to repeat for every display update, so reuse a recent answer.
	ULONGLONG nNow = GetTickCount64();

	if ((blnHaveActiveProduct == false)
		|| ((nNow - nActiveProductTime) >= nActiveProductCacheMs))
	{
		hrActiveProduct = DetectActiveProduct( CachedProduct);
		nActiveProductTime = nNow;
		blnHaveActiveProduct = true;

		// Drop the JAWS object once JAWS has gone, so a later JAWS gets a fresh one
		if (CachedProduct != ID_JAWS)
		{
			JawsAPI.Release();
		}
	}

	Product = CachedProduct;
	return hrActiveProduct;
}


void RBSpeech::RefreshActiveProduct()
{
	blnHaveActiveProduct = false;
}


	bool RBSpeech::IsNarratorActive()
{
	return FindProcessByName( L"Narrator.exe");
}


	HRESULT RBSpeech::DetectActiveProduct(ActiveProduct& Product)
{
HRESULT hReturnValue =S_OK;
//check to see what is active, in order of preference: JAWS, NVDA, System Access, then Narrator.
if (IsJAWSActive())
{ //JAWS IS ACTIVE.
	Product = ID_JAWS;
} //End JAWS active block.
else if (IsNVDAActive())
	{ //NVDA is active.
		Product =ID_NVDA;
	} //End NVDA active block.

else if (IsSystemAccessActive())
	{ //System Access is active.
	Product =ID_SYSTEMACCESS;
	} //System Access is active.
else if (IsNarratorActive())
	{ //Narrator is active.
	Product =ID_NARRATOR;
	} //Narrator is active.

	else
	{ //no product is active.
		Product =ID_NONE;
		hReturnValue =S_FALSE;
	} //No product is active.
return hReturnValue;
}

bool RBSpeech::LoadAPI()
{
	ActiveProduct CurrentProduct;

	if (GetActiveProduct( CurrentProduct))
	{
	switch(CurrentProduct)
	{
	case ID_JAWS:
		{
			return LoadJAWSApi();
		}
		break;
	case ID_NVDA:
				{
					return LoadNVDAApi();
				}
				break;
		
	case ID_SYSTEMACCESS:
				{
					return LoadSystemAccessApi();
				}
				break;
		
	default:
			{
return false;
			}
			break;
	}  // end switch
	} 
	else   // Could not determine product
	{
		return false;
	}
	}

void RBSpeech::UnloadAPI()
{
	ActiveProduct CurrentProduct;
	GetActiveProduct( CurrentProduct);
	switch(CurrentProduct)
	{
	case ID_JAWS:
		UnloadJAWSApi();
		break;
	case ID_NVDA:
		UnloadNVDAApi();
		break;
		case ID_SYSTEMACCESS:
			UnloadSystemAccessApi();
				break;
		default:
		break;
	}
	return;
}

HRESULT RBSpeech::IsMuted(void)
{
if (blnMuted)
{
	return S_OK;
}
else
{
return S_FALSE;
}
}

void RBSpeech::Mute( bool blnNewMute)
{
blnMuted = blnNewMute;
}

HRESULT RBSpeech::Speak(std::wstring strText)
{
	return Speak(strText, false);
}
	
HRESULT RBSpeech::Speak(std::wstring strText, BOOL blnSilence)
{
	// The Mac version always honoured mute; the Windows one ignored it
	if (IsMuted() == S_OK)
	{
		return S_OK;
	}

	if (strText.empty())
	{
		return S_FALSE;
	}

	HRESULT hReturnValue =S_FALSE;
	ActiveProduct CurrentProduct;
	GetActiveProduct( CurrentProduct);

	// A screen reader's own API works while SurfaceReader is in the background,
	// which UI Automation notifications may not, so use it whenever there is one.
	switch(CurrentProduct)
	{
	case ID_JAWS:
		hReturnValue =JAWSSpeak(strText, blnSilence);
		break;
	case ID_NVDA:
				hReturnValue =NVDASpeak(strText, blnSilence);
		break;
		case ID_SYSTEMACCESS:
				hReturnValue =SystemAccessSpeak(strText, blnSilence);
				break;
		case ID_NARRATOR:
			// Narrator has no API of its own; it reads UI Automation notifications
			hReturnValue = pAnnouncer->Announce( strText, (blnSilence != FALSE));
			break;
		default:
		// No screen reader recognised; one we don't know may still be listening to UI Automation
		hReturnValue =E_NOTIMPL;
			break;
	}

	if (hReturnValue != S_OK)
	{
		if (CurrentProduct != ID_NONE)
		{
			// The screen reader may have closed or changed: check again on the next call
			RefreshActiveProduct();
		}

		// Let Windows pass the text to whichever screen reader is listening
		// (already tried for Narrator, so not again)
		if (CurrentProduct != ID_NARRATOR)
		{
			HRESULT hrAnnounce = pAnnouncer->Announce( strText, (blnSilence != FALSE));
			if (hrAnnounce == S_OK)
			{
				hReturnValue = S_OK;
			}
		}
	}

	return hReturnValue;
}

HRESULT RBSpeech::Silence()
{
	HRESULT hReturnValue =S_FALSE;
	ActiveProduct CurrentProduct;
	hReturnValue =GetActiveProduct( CurrentProduct);
	ExitOnFailure(hReturnValue, "No product is active.");
	switch(CurrentProduct)
	{
	case ID_JAWS:
		hReturnValue =JAWSSilence();
		break;
	case ID_NVDA:
				hReturnValue =NVDASilence();
		break;
		case ID_SYSTEMACCESS:
				hReturnValue =SystemAccessSilence();
				break;
		default:
		hReturnValue =E_NOTIMPL;
			break;
	}
	LExit:
	return hReturnValue;
}
//Hotspot specific functions.
HRESULT RBSpeech::IsHotSpotInSet(std::wstring SetName, std::wstring SpotName)
{
	HRESULT hReturnValue =S_OK;
	ActiveProduct CurrentProduct;
	boost::property_tree::ptree IniTree; //used to store hsc information.
		boost::property_tree::ptree::assoc_iterator it;	 
		std::string SetNameStr;
	std::string SpotNameStr;
	ExitOnTrue(SetName.empty(), hReturnValue, S_FALSE, "No hot spot set has been provided.");
	ExitOnTrue(SpotName.empty(), hReturnValue, S_FALSE, "No hot spot name has been provided.");
	ExitOnFalse(fs::exists(SetName), hReturnValue, S_FALSE, "The hot spot set does not exist.");
		hReturnValue =GetActiveProduct( CurrentProduct);
	ExitOnFailure(hReturnValue, "No product is active.");
	switch(CurrentProduct)
	{
	case ID_JAWS:
		SetNameStr = WideStringToNarrowString(SetName);
		boost::property_tree::ini_parser::read_ini(&SetNameStr[0], IniTree);		
		//try and find the key.
		SpotNameStr = WideStringToNarrowString(SpotName);
		it =IniTree.find(&SpotNameStr[0]);
		ExitOnSpecificValue(it, IniTree.not_found(), hReturnValue, S_FALSE, "The spot is not available in the active set.");
		//now check to see if the spot is hidden, hence shouldn't be available.
			ExitOnSpecificValue(it->second.get(std::string(&SpotNameStr[0]).append(".Hidden"), 0), HOTSPOT_HIDDEN, hReturnValue, S_FALSE, "The spot is hidden.");
		break;
	default:
		hReturnValue =E_NOTIMPL;
		break;
	};
	LExit:
return hReturnValue;
}
	
HRESULT RBSpeech::IsHotSpotInCurrentSet(std::wstring SpotName)
	{
		HRESULT hReturnValue =S_OK;
		ActiveProduct CurrentProduct;
		std::wstring CurrentSet;
		ExitOnTrue(SpotName.empty(), hReturnValue, S_FALSE, "No hot spot name has been provided.");
		hReturnValue =GetActiveProduct( CurrentProduct);	
		ExitOnFailure(hReturnValue, "No product is active.");
	switch(CurrentProduct)
		{
		case ID_JAWS:
		hReturnValue =GetActiveHotSpotSet(CurrentSet);			
				ExitOnFailure(hReturnValue, "Unable to obtain the currently active hot spot set.");		
		hReturnValue =IsHotSpotInSet(CurrentSet, SpotName);	
				ExitOnFailure(hReturnValue, "Unable to obtain the currently active hot spot set.");		
		break;
		default:
			hReturnValue =E_NOTIMPL;
			break;
		};
LExit:
		return hReturnValue;
	}
	
	HRESULT RBSpeech::ListHotSpotsInSet(std::wstring SetName, std::vector<std::string>& Spots)
	{
		HRESULT hReturnValue =S_OK;
		ActiveProduct CurrentProduct;
		boost::property_tree::ptree IniTree; //used to store hsc information.
		boost::property_tree::ptree::assoc_iterator it;	 
		std::string SetNameStr;
	ExitOnTrue(SetName.empty(), hReturnValue, S_FALSE, "No hot spot set has been provided.");
	ExitOnFalse(fs::exists(SetName), hReturnValue, S_FALSE, "The hot spot set does not exist.");
	hReturnValue =GetActiveProduct( CurrentProduct);
ExitOnFailure(hReturnValue, "No product is active.");
	switch(CurrentProduct)
	{
	case ID_JAWS:
		SetNameStr = WideStringToNarrowString(SetName);
		try
			{
				boost::property_tree::ini_parser::read_ini(SetNameStr, IniTree);		
		}
		catch( ...)
			{
		return S_FALSE;
		}
		
		ExitOnTrue(IniTree.empty(), hReturnValue, S_FALSE, "No hot spots exist in the supplied set.")
			Spots.clear();
		Spots.reserve(IniTree.size()); //reserve memory equal to the number of spots in the current set.
		//now iterate the spots.
			for (boost::property_tree::ptree::value_type &v : IniTree)
			{
				if (!v.first.empty())
				{ //the key has some text in.
					if (!boost::algorithm::iequals(v.first, "main")) { //section is not main
						std::string Path =v.first;
						Path.append(".hidden");
						int IsHidden =IniTree.get(Path, 0);
						if (IsHidden ==0)
						{ //spot is not hidden.
							Path =v.first;
							Path.append(".label");
							std::string SpotLabel =IniTree.get(Path, v.first);
						Spots.push_back(SpotLabel);
						} //end hidden.
						} //end is not main.
					} //end key has text.
			}
		break;
	default:
		hReturnValue =E_NOTIMPL;
		break;
	};
LExit:
		return hReturnValue;
	}

	HRESULT RBSpeech::ListHotSpotsInCurrentSet(std::vector<std::string>& Spots)
{
	HRESULT hReturnValue =S_OK;
ActiveProduct CurrentProduct;
	std::wstring CurrentSet;
	hReturnValue =GetActiveProduct( CurrentProduct);
ExitOnFailure(hReturnValue, "No product is active.");	
	switch(CurrentProduct)
	{
	case ID_JAWS:
	hReturnValue =GetActiveHotSpotSet(CurrentSet);
	ExitOnFailure(hReturnValue, "The active hot spot set could not be obtained.")
	hReturnValue =ListHotSpotsInSet(CurrentSet, Spots);
	ExitOnFailure(hReturnValue, "Unable to list the hot spots.");
break;
	default:
		hReturnValue =E_NOTIMPL;
break;
	}
LExit:
	return hReturnValue;
}

	HRESULT RBSpeech::GetActiveHotSpotSet(std::wstring& ActiveSet)
{
HRESULT hr =S_OK;
boost::optional<std::string> CurrentSpotStringOptional;			
CComVariant vFunctionResult;
std::wstring ConvertedSpotString;
			boost::property_tree::ptree IniTree; //used to store hsc information.
			std::wstring JAWSFunctionCallString =L"GetCurrentJAWSEnvironment(\"%s\")"; //used to hold the call to JAWS.
				fs::path IniFile; //file to store the hsc information.
			ActiveProduct CurrentProduct;
	hr =GetActiveProduct( CurrentProduct);
	ExitOnFailure(hr, "No product is active.");
				switch(CurrentProduct)
			{
			case ID_JAWS:
				IniFile =AppDataPath().native();
//now, set the filename.
IniFile /=L"CurrentJawsEnvironment.ini";
if (exists(IniFile))
{ //the file exists remove it.
	ExitOnFalse(fs::remove(IniFile), hr, S_FALSE, "Unable to delete the old file.");
} //end file removal.
boost::replace_first(JAWSFunctionCallString, L"%s", AppDataPath().native());
//now actually call the function.
hr = JawsAPI.Invoke1(_bstr_t(L"RunFunction"), &_variant_t(JAWSFunctionCallString.c_str()), &vFunctionResult);
ExitOnFailure(hr, "Executing the JAWS RunFunction function returned a failure.");
ExitOnFalse(vFunctionResult.vt == VT_BOOL, hr, S_FALSE, "Running a JAWS function should return a boolean.");
ExitOnFalse(vFunctionResult.boolVal, hr, S_FALSE, "Running the requested JAWS function could not be scheduled.");

									//now, load the ini giving us hsc information, then grab the path to the current spot file.
try
	{
		boost::property_tree::ini_parser::read_ini(IniFile.generic_string(), IniTree);		
}
catch( ...)
	{
return S_FALSE;
			}

//obtain the current spot set filename.
									CurrentSpotStringOptional =IniTree.get_optional<std::string>("Environment.ActiveSpotFile");
									ConvertedSpotString = NarrowStringToWideString(CurrentSpotStringOptional.get());
ActiveSet =ConvertedSpotString; //Actually output.
									break;
			default:
				hr =E_NOTIMPL;
				break;
			};
LExit:
return hr;
}

	HRESULT RBSpeech::ExecuteHotSpot(std::wstring Set, std::wstring SpotName)
	{
		HRESULT hr =S_OK;
		ActiveProduct CurrentProduct;
		std::wstring JAWSFunctionCallString =L"HSCLookupKey( \"%s\")"; //used to hold the call to JAWS.
		CComVariant vFunctionResult;
		ExitOnTrue(Set.empty(), hr, S_FALSE, "No hot spot set has been provided.");
		ExitOnTrue(SpotName.empty(), hr, S_FALSE, "No hot spot name has been provided.");
			hr =GetActiveProduct( CurrentProduct);
	ExitOnFailure(hr, "No product is active.");
		switch(CurrentProduct)
			{
			case ID_JAWS:
				LoadJAWSApi();
				//Check to see if the hotspot is in the provided set.
				hr =IsHotSpotInSet(Set, SpotName);
				ExitOnFailure(hr, "The provided hot spot doesn't exist in the provided hot spot set.");
				//now create the requisite information to call a jaws function.
				boost::replace_first(JAWSFunctionCallString, L"%s", SpotName);
				//now call the function.
				hr = JawsAPI.Invoke1(_bstr_t(L"RunFunction"), &_variant_t(JAWSFunctionCallString.c_str()), &vFunctionResult);
				ExitOnFailure(hr, "Executing the JAWS RunFunction function returned a failure.");
				ExitOnFalse(vFunctionResult.vt == VT_BOOL, hr, S_FALSE, "Executing a JAWS hot spot should return a boolean.");
				ExitOnFalse(vFunctionResult.boolVal, hr, S_FALSE, "The requested hot spot execution could not be scheduled.");
				break;
			default:
				hr =E_NOTIMPL;
				break;
		};
LExit:
		return hr;
	}

HRESULT RBSpeech::ExecuteAction(std::wstring Action, ScreenReaderActionType Type)
{
HRESULT hReturnValue =S_OK;
ExitOnTrue(Action.empty(), hReturnValue, S_FALSE, "no action was supplied.");
			ActiveProduct CurrentProduct;
	hReturnValue =GetActiveProduct( CurrentProduct);
	ExitOnFailure(hReturnValue, "No product is active.");
				
	switch(CurrentProduct)
			{
			case ID_JAWS:
hReturnValue =ExecuteJAWSAction(Action, Type);
				break;
default:
hReturnValue =E_NOTIMPL;
break;
				};
LExit:
return hReturnValue;
}

HRESULT RBSpeech::GetAvailableActions(AvailableActionsType& ActionInformation)
{
HRESULT hReturnValue =S_OK;
AvailableActionsType AvailableActions;
			ActiveProduct CurrentProduct;
	hReturnValue =GetActiveProduct( CurrentProduct);
	ExitOnFailure(hReturnValue, "No product is active.");
				switch(CurrentProduct)
			{
			case ID_JAWS:
hReturnValue =GetAvailableJAWSActions(AvailableActions);
				break;
default:
hReturnValue =E_NOTIMPL;
break;
				};
LExit:
ActionInformation =AvailableActions;
return hReturnValue;
}

std::vector<JawsFunction> RBSpeech::ProcessJSDFile(fs::path&File)
{
	std::vector<JawsFunction> AvailableFunctions;
	bool ParameterIsOptional =false;
	if (fs::exists(File))
	{ //existence check.
		std::wifstream JsdStream(File);
		//Read the file in to a vector.
		std::vector<std::wstring> lines;
		std::wstring str;
		while (std::getline(JsdStream, str))
		{
			lines.push_back(str);
		}
			JsdStream.close();
		
			JawsFunction newfunction;
	for (std::wstring line : lines)
{
	std::vector<std::wstring> tokens;
	
	boost::trim(line);
	
	if (line.substr(0, 3).compare(L":sc"))
	{ //script and name
//Clear all variables as we're starting a new script.
		tokens.clear();
		ParameterIsOptional =false;
		newfunction.Type =ID_TYPE_NONE;
		newfunction.Category.clear();
		newfunction.Description.clear();
		newfunction.Name.clear();
		newfunction.Synopsis.clear();
		//start setting variables.
		newfunction.Type =ID_TYPE_SCRIPT;
		//retrieve the name.
		boost::split(tokens, line, boost::is_any_of(" "));
		
if (tokens.size() ==2)
{ //access the name.
newfunction.Name =*end(tokens);
} //access the name.
} //end script and name.
		else if (line.substr(0, 3).compare(L":fu"))
	{ //function and name
//Clear all variables as we're starting a new script.
		tokens.clear();
		newfunction.Type =ID_TYPE_NONE;
		ParameterIsOptional =false;
		newfunction.Category.clear();
		newfunction.Description.clear();
		newfunction.Name.clear();
		newfunction.Synopsis.clear();
		//start setting variables.
		newfunction.Type =ID_TYPE_FUNCTION;
		//retrieve the name.
		boost::split(tokens, line, boost::is_any_of(" "));
if (tokens.size() ==2)
{ //access the name.
newfunction.Name =*end(tokens);
} //access the name.
} //end function and name.
	else if (line.substr(0, 3).compare(L":sy"))
{ //synopsis.
		newfunction.Synopsis =line.substr(line.find(L" "));
	} //end synopsis.
else if (line.substr(0, 3).compare(L":de"))
{ //Description.
	newfunction.Description = line.substr(line.find(L" "));
	} //end description.
else if (line.substr(0, 3).compare(L":op"))
{ //optional parameter.
	ParameterIsOptional =true;
} //Optional parameter.
else if (line.substr(0, 3).compare(L":pa"))
{ //Parameter.
JAWSParameter CurrentParam;
CurrentParam.Optional =ParameterIsOptional;
if (line.find(L"/"))
{ //parameter with a name.
	tokens.clear();
	boost::split(tokens, line, boost::is_any_of(" "));
	if (tokens.size() ==2)
	{ //process.
		CurrentParam.DataType =*begin(tokens);
		std::wstring tokenToSplit = *end(tokens);
		CurrentParam.Name = tokenToSplit.substr(0, tokenToSplit.find_first_of(L" "));
		CurrentParam.Description = tokenToSplit.substr(tokenToSplit.find_first_of(L" "));
	} //end parameter processing.
} //end param with a name.
else { //parameter without a name.
	tokens.clear();
	boost::split(tokens, line, boost::is_any_of(" "));
	if (tokens.size() >= 2)
	{ //process.
		CurrentParam.DataType = *begin(tokens);
		CurrentParam.Description = line.substr(CurrentParam.DataType.length());
	} //end parameter processing.
} //end parameter without a name.
newfunction.Parameters.push_back(CurrentParam);
ParameterIsOptional =false;
} //end parameter
else if (line.substr(0, 3).compare(L":re")) 
{ //return value.
		tokens.clear();
		std::wstring stringToSplit = line.substr(line.find_first_of(L" "));
		boost::split(tokens, stringToSplit, boost::is_any_of(" "));
		newfunction.Returns.DataType = *begin(tokens);
		newfunction.Returns.Description = line.substr(newfunction.Returns.DataType.length());
	} //return value.
else if (line.substr(0, 3).compare(L":ca"))
	{ //Category.
		newfunction.Category =line.substr(line.find(L" "));
} //end category.
else if (line.length() ==0)
{ //blank line, ass to vector.
	//now add this to the vector.
		AvailableFunctions.push_back(newfunction);
} //end vector adition.
}
	} //end existence check.		
return AvailableFunctions;
}

void RBSpeech::SetFirstJsdFile(std::wstring file)
{
	JsdFile =file;
JsdFileToStartProcessing =PROCESS_NO_FILE;
std::vector<std::wstring> JsdFileTokens;
boost::split(JsdFileTokens, JsdFile, boost::is_any_of(L"\\"));

std::wstring UserName;
HRESULT hr = GetCurrentUserName(UserName);
if (std::find(begin(JsdFileTokens), end(JsdFileTokens), UserName) !=end(JsdFileTokens))
{ //user file is the first one.
//determine whether it is default/application.
	if (JsdFileTokens.back().compare(L"default.jsd"))
		JsdFileToStartProcessing =PROCESS_USER_SYSTEM_DEFAULT_FILE;
	else
JsdFileToStartProcessing =PROCESS_USER_APP_FILE;
} //end user file.
else { //global file.
	//determine whether it is the default/application.
	if (JsdFileTokens.back().compare(L"default.jsd"))
		JsdFileToStartProcessing =PROCESS_DEFAULT_SYSTEM_DEFAULT_FILE;
	else
JsdFileToStartProcessing =PROCESS_DEFAULT_APP_FILE;
} //end global file.
return;
}

std::wstring RBSpeech::GetFirstJsdFile(void)
{
	return JsdFile;
}
void RBSpeech::ClearJsdFile()
{
	JsdFile =L"";
JsdFileToStartProcessing =PROCESS_NO_FILE;
}
void RBSpeech::SetHscFile(std::wstring File)
{
HscFile =File;
return;
}

std::wstring RBSpeech::GetHscFile(void)
{
return HscFile;
}

void RBSpeech::ClearHscFile()
{
HscFile =L"";
return;
}


// Variant-handling

		std::vector <JAWSParameter> AsJawsParameterVector( const std::any  operand)
{
std::vector <JAWSParameter> myParameters;

try
	{
		myParameters = std::any_cast<std::vector <JAWSParameter> >( operand);
return myParameters;
}
catch( const std::bad_any_cast &)
{
return myParameters;
}
}

		int AsInt( const std::any  operand)
{
    try
	{
		int nOut = std::any_cast<int>( operand);
		return nOut;
}
catch( const std::bad_any_cast &)
{
return -1;
}
}

std::wstring AsWString( const std::any operand)
	{
try
	{
		std::wstring wstrOut = std::any_cast<std::wstring>( operand);
		return wstrOut;
}
catch( const std::bad_any_cast &)
{
return wstrEmpty;
}
}

bool IsWString(const std::any & operand)
{
    return operand.type() == typeid( std::wstring);
}

bool IsInt(const std::any & operand)
{
    return operand.type() == typeid(int);
}

bool IsJawsParameterVector(const std::any & operand)
{
    return operand.type() == typeid( std::vector <JAWSParameter>);
}

HRESULT RBSpeech::IsNVDAActive()
{
	if (LoadNVDAApi())
	{
		if (TestIfRunning() == 0)
		{
			return true;
		}
		else
		{
			return false;
		}
	}
	else
	{
		return false;
	}
}

//file path functions.
HRESULT RBSpeech::GetExecutablePath(fs::path& path)
{
	// This used to declare an array of string pointers and pass an uninitialised one to
	// GetModuleFileName, so the path was written to random memory, then freed with ReleaseStr.
	wchar_t szFileName[ MAX_PATH + 1] = L"";
	DWORD result = GetModuleFileNameW(NULL, szFileName, MAX_PATH + 1);
	if ((result == 0)
		|| (result > MAX_PATH))
	{
		return S_FALSE;  // failed, or the path was truncated
	}
	path = szFileName;
	return S_OK;
}
HRESULT RBSpeech::GetCommonAppDataPath(fs::path &path)
{
	wchar_t szPath[ MAX_PATH] = L"";
	HRESULT hr = SHGetFolderPathW(NULL, CSIDL_COMMON_APPDATA, NULL, SHGFP_TYPE_CURRENT, szPath);
	if (SUCCEEDED( hr))
	{
		path = szPath;
	}
	return hr;
}

HRESULT RBSpeech::GetCurrentUsersAppDataPath(fs::path &path)
{
	wchar_t szPath[ MAX_PATH] = L"";
	HRESULT hr = SHGetFolderPathW(NULL, CSIDL_APPDATA, NULL, SHGFP_TYPE_CURRENT, szPath);
	if (SUCCEEDED( hr))
	{
		path = szPath;
	}
	return hr;
}
//string conversion.
// UTF-8 to UTF-16 (std::wstring_convert, used before, is deprecated in C++17)
std::wstring RBSpeech::NarrowStringToWideString(const std::string& stringToConvert)
{
	if (stringToConvert.empty())
	{
		return std::wstring();
	}
	int nLength = MultiByteToWideChar(CP_UTF8, 0, stringToConvert.data(), (int) stringToConvert.size(), NULL, 0);
	std::wstring wstrOut( nLength, L'\0');
	MultiByteToWideChar(CP_UTF8, 0, stringToConvert.data(), (int) stringToConvert.size(), &wstrOut[0], nLength);
	return wstrOut;
}

// UTF-16 to UTF-8
std::string RBSpeech::WideStringToNarrowString(std::wstring& stringToConvert)
{
	if (stringToConvert.empty())
	{
		return std::string();
	}
	int nLength = WideCharToMultiByte(CP_UTF8, 0, stringToConvert.data(), (int) stringToConvert.size(), NULL, 0, NULL, NULL);
	std::string strOut( nLength, '\0');
	WideCharToMultiByte(CP_UTF8, 0, stringToConvert.data(), (int) stringToConvert.size(), &strOut[0], nLength, NULL, NULL);
	return strOut;
}
//Function to get the windows user name.
HRESULT RBSpeech::GetCurrentUserName(std::wstring& userName)
{
	HRESULT hr = S_OK;
#define buffCharCount 32767
	LPWSTR lpBuffer[buffCharCount];
	DWORD CoppiedBytes = 0;
	bool result = GetUserName(lpBuffer[0], &CoppiedBytes);
	ExitOnFalse(result, hr, S_FALSE, "Unable to obtain the username.");
	userName = lpBuffer[0];
LExit:
	return hr;
}
#endif  // Windows
