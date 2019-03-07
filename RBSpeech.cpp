// RBSpeech.cpp

#include "RBSpeech.h"

#ifdef __WINDOWS__ 
#include <TlHelp32.h>
#include <Processthreadsapi.h> // on Windows 8 and Windows Server 2012
#include <comutil.h>
#include <procutil.h>
#include <osutil.h>
#include <vector>
#include <algorithm>

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

RBSpeech::RBSpeech(void)
{
}


RBSpeech::~RBSpeech(void)
{}
	

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
	bool result = false;
	//first initialize com for the current thread.
CoInitializeEx(0, COINIT_APARTMENTTHREADED); 
//Now obtain an instance of the required co class.
CComPtr<IDispatch> lpTDispatch;
HRESULT hr= lpTDispatch.CoCreateInstance(_bstr_t(L"FreedomSci.JawsApi"));
if (hr == S_OK)
{
	JawsAPI = lpTDispatch;
	result = true;
}
return result;
}

void RBSpeech::UnloadJAWSApi(void)
{
// Dolphin API always loaded, so must unload
	this->UnloadDolphinApi();
	//Release the JAWS API com object.
	JawsAPI.Release();
	//Now uninitialize com.
	CoUninitialize();	
	return;
}
	
HRESULT RBSpeech::JAWSSpeak(wstring strText, BOOL blnSilence)
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

LoadJAWSApi();
	hr 	= JawsAPI.Invoke2(_bstr_t(L"SayString"), &_variant_t(strText.c_str()), &_variant_t(blnSilence), &vFunctionResult);
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
	
HRESULT RBSpeech::JAWSBraille(wstring strText)
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
AvailableActionFieldsType Fields =boost::assign::map_list_of(L"Name", 0)(L"Synopsis", 1)(L"Description", 2)(L"Returns", 3)(L"Parameters", 4)(L"Category", 5)(L"Type", 6);
ActionInfoType CurrentAction;
std::vector<JawsFunction> AvailableJawsFunctions;
std::wstring UserScriptFolder; 
std::wstring SharedScriptFolder; 
std::wstring DefaultScriptFile; 
std::wstring ApplicationScriptFile; 
std::experimental::filesystem::path JSDFile;
wxVariant FunctionResult;
			boost::property_tree::ptree IniTree; //used to store environment information.
			std::wstring JAWSFunctionCallString =L"GetCurrentJAWSEnvironment(\"%s\")"; //used to hold the call to JAWS.
			std::experimental::filesystem::path IniFile; //file to store the hsc information.
			ActiveProduct CurrentProduct;
	DolphinProduct SpecificDolphinProduct;
	hReturnValue =GetActiveProduct(CurrentProduct, SpecificDolphinProduct);
	ExitOnFailure(hReturnValue, "No product is active.");
				switch(CurrentProduct)
			{
			case ID_JAWS:
				if (JsdFileToStartProcessing !=PROCESS_NO_FILE) 					
				{
//Process files based on the enum.
					int CurrentFileBeingProcessed;
					std::wstring TestDir;
		std::experimental::filesystem::path CurrentFile;
					int FreedomScientificDirectoryPosition =-1;
					std::vector<std::wstring> tokens;
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
 TestDir =wxStandardPaths::Get().GetConfigDir().Remove(wxStandardPaths::Get().GetConfigDir().find(L"SurfaceReader"));
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
	TestDir =wxStandardPaths::Get().GetUserConfigDir();
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
 TestDir =wxStandardPaths::Get().GetConfigDir().Remove(wxStandardPaths::Get().GetConfigDir().find(L"SurfaceReader"));
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
BOOST_FOREACH(JawsFunction j, AvailableJawsFunctions)
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

	bool RBSpeech::GetJAWSPath(std::experimental::filesystem::path& FileName)
	{
		bool blnReturnValue =false;
		std::experimental::filesystem::path InternalPath;
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
		std::experimental::filesystem::path JAWSPath;
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
		HRESULT hr = S_OK;
		BOOL bIs64BitProcess = false;
		OS_VERSION version;
		DWORD servicePack = 0;
		//Retrieve the OS version.
		OsGetVersion(&version, &servicePack);
		//Retrieve whether the process is 64-bit.
		hr = ProcWow64(::GetCurrentProcess(), &bIs64BitProcess);
		//first check whether an older version--pre 211 is installed.
		//the only reliable way to check this is using a registry key check.

		//conditionalise this on operating system, if the user is running xp we don't support 64-bit.
		if (version == OS_VERSION_WINXP && !bIs64BitProcess)
		{ //we are using xp.
			{ // Use a boost::scoped_ptr to ensure that raii is used to close the key when done.
    HKEY reg = NULL;
    DWORD dwErr =RegOpenKey(HKEY_LOCAL_MACHINE, L"SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Uninstall\\HotSpotClicker", &reg);
    boost::shared_ptr<void> reg_ptr(reg, ::CloseHandle);
if (dwErr ==ERROR_SUCCESS)
{ //Key is valid so hsc 211 is installed.
	eReturnValue =STATE_OLDER_VERSION_INSTALLED;
} //end key is valid block.
} //end raii scope for the key.
		} //end xp specific block.
		//check for at least vista.
		else if (version == OS_VERSION_VISTA)
		{ //we are running vista or above.
			//Query the wow6432node key.
				{ // Use a boost::scoped_ptr to ensure that raii is used to close the key when done.
    HKEY reg = NULL;
    DWORD dwErr =RegOpenKey(HKEY_LOCAL_MACHINE, L"SOFTWARE\\Wow6432Node\\Microsoft\\Windows\\CurrentVersion\\Uninstall\\HotSpotClicker", & reg);
    boost::shared_ptr<void> reg_ptr(reg, ::CloseHandle);
if (dwErr ==ERROR_SUCCESS)
{ //Key is valid so hsc 211 is installed.
	eReturnValue =STATE_OLDER_VERSION_INSTALLED;
} //end key is valid block.
} //end raii scope for the key.
		} //end vista or above block.
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
	BOOL bIsProcess64Bit = false;
	if (NvdaDllApi.is_loaded())
	{
		return true;
	}
	
	std::experimental::filesystem::path NVDADllFileName =wxStandardPaths::Get().GetExecutablePath().ToStdWstring(); //assign the executable directory.
	
	hr = ProcWow64(::GetCurrentProcess(), &bIsProcess64Bit);
	if (hr == S_FALSE)
	{
		return false;
	}

	if (bIsProcess64Bit)
		{ //We are running on a 64-bit operating system--or at least as a 64-bit process.
NVDADllFileName /=L"nvdaControllerClient64.dll";
	}
	else  //32-bit.
			{
			NVDADllFileName /=L"nvdaControllerClient32.dll";
	}

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

HRESULT RBSpeech::NVDASpeak(wstring strText, BOOL blnSilence)
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
	
HRESULT RBSpeech::NVDABraille(wstring strText)
{
HRESULT hReturnValue =S_OK;
//check to see that the message to be brailled actually contains some text.
ExitOnTrue(strText.empty(), hReturnValue, __HRESULT_FROM_WIN32(ERROR_BAD_ARGUMENTS), "No text has been specified.");
			ExitOnNull(BrailleMessage, hReturnValue, S_FALSE, "The braille message function is null, please call ActiveProduct.");
		ExitIfValueGreaterThanSuppliedValue(BrailleMessage(strText.c_str()), 0, hReturnValue, S_FALSE, "Brailleing through NVDA failed.");
		LExit:
		return hReturnValue;			
}

//Dolphin.
//All dolphin functions for dolphin requires us to be 32-bit.

bool RBSpeech::IsDolphinActive(DolphinProduct& SpecificProduct)
{
#ifndef _WIN64
	SpecificProduct =ID_NOPRODUCT;

	//API loaded, so now have a switch to determine what exact product from dolphin is active.
switch(DolAccess_GetSystem())
{
case 0:
	{
		SpecificProduct =ID_NOPRODUCT;
return false; //no product.
	}
	break;

case 1:
	{
		SpecificProduct =ID_HAL;
return true; 
	}
	break;

case 2:
	{
		SpecificProduct =ID_LUNAR;
return true; 
	}
	break;

case 4:
	{
		SpecificProduct =ID_SUPERNOVA;
return true; 
	}
	break;

case 8:
	{
		SpecificProduct =ID_LUNARPLUS;
return true; 
	}
	break;

default:
	{
		SpecificProduct =ID_NOPRODUCT;
return false; //no product.
	}
	break;
}  // end switch
#else 
	return false;
#endif
}

	
bool RBSpeech::LoadDolphinApi()
{
#ifndef _WIN64
	return true;
#else
	return false;
#endif
}

	
void RBSpeech::UnloadDolphinApi(void)
{
	return;
}
	
	HRESULT RBSpeech::DolphinSpeak(wstring strText, BOOL blnSilence, DolphinReturnCode& ReturnCode)
{
#ifndef _WIN64
	HRESULT hReturnValue =S_OK;
ExitOnTrue(strText.empty(), hReturnValue, S_FALSE, "No text was specified to speak through dolphin.");
const wchar_t *StrToSpeak =strText.c_str();
	DWORD len = (wcslen(StrToSpeak)+1)*sizeof(WCHAR);
// DWORD ReturnValue =DolAccess_Command((VOID *) StrToSpeak, len, DOLAPI_COMMAND_SPEAK);
	DWORD ReturnValue = 1;
		// ReturnValue = DolAccess_Command((VOID *)StrToSpeak, len, DOLAPI_COMMAND_SPEAK);
switch(ReturnValue)
{
case 0:
	ReturnCode =ID_SUCCESS;
break;
case 1:
	ReturnCode =ID_No_Server;
	hReturnValue =S_FALSE;
	break;
case 2:
	ReturnCode =		ID_INVALID_ID;
	hReturnValue =S_FALSE;
	break;
case 3:
	ReturnCode =ID_INVALID_LENGTH;
	hReturnValue =S_FALSE;
	break;
case 4:
	ReturnCode =ID_INVALID_ACTION;
	hReturnValue =S_FALSE;
	break;
case 5:
	ReturnCode =ID_ALREADY_IN_USE;
	hReturnValue =S_FALSE;
	break;
case 6: 
	ReturnCode =ID_SUPERNOVA_CONTROL_PANEL_IN_USE;
	hReturnValue =S_FALSE;
	break;
case 7:
	ReturnValue =ID_SUPERNOVA_NOT_RESPONDING;
	hReturnValue =S_FALSE;
	break;
case 8:
	ReturnCode =ID_SET_VAL_FAILED_PARAMETER_READONLY;
	hReturnValue =S_FALSE;
	break;
default:
	hReturnValue =S_FALSE;
	break;
};
LExit:
return hReturnValue;
#else
	return E_NOTIMPL;
#endif
	}
	
	HRESULT RBSpeech::DolphinSilence(DolphinReturnCode& ReturnCode)
	{
#ifndef _WIN64
		HRESULT hReturnValue = S_OK;
		ExitOnNull(ReturnCode, hReturnValue, S_FALSE, "No exact return code was supplied.");
		DWORD ReturnValue = DolAccess_Action(141);
		switch (ReturnValue)
		{
		case 0:
			ReturnCode = ID_SUCCESS;
			break;
		case 1:
			ReturnCode = ID_No_Server;
			hReturnValue = S_FALSE;
			break;
		case 2:
			ReturnCode = ID_INVALID_ID;
			hReturnValue = S_FALSE;
			break;
		case 3:
			ReturnCode = ID_INVALID_LENGTH;
			hReturnValue = S_FALSE;
			break;
		case 4:
			ReturnCode = ID_INVALID_ACTION;
			hReturnValue = S_FALSE;
			break;
		case 5:
			ReturnCode = ID_ALREADY_IN_USE;
			hReturnValue = S_FALSE;
			break;
		case 6:
			ReturnCode = ID_SUPERNOVA_CONTROL_PANEL_IN_USE;
			hReturnValue = S_FALSE;
			break;
		case 7:
			ReturnValue = ID_SUPERNOVA_NOT_RESPONDING;
			hReturnValue = S_FALSE;
			break;
		case 8:
			ReturnCode = ID_SET_VAL_FAILED_PARAMETER_READONLY;
			hReturnValue = S_FALSE;
			break;
		default:
			hReturnValue = S_FALSE;
			break;
		};
	LExit:
		return hReturnValue;
#else
		return E_NOTIMPL;
#endif
	}

	HRESULT RBSpeech::DolphinBraille(wstring strText)
{
	return E_NOTIMPL;
	}
	
	HRESULT RBSpeech::GetAvailableDolphinActions(AvailableActionsType& ActionInformation)
	{
#ifndef _WIN64
		//declare needed variables.
		AvailableActionFieldsType AvailableDolphinActionFields;
ActionCollectionType DolphinActionCollection; //store all action information for Dolphin.
//add the description, ID and type int/strings.
AvailableDolphinActionFields.clear();
// AvailableDolphinActionFields =boost::assign::map_list_of(L"ID", 0)(L"Description", 1)(L"Action Type", 2);
AvailableDolphinActionFields.insert(std::make_pair( L"ID", 0));
AvailableDolphinActionFields.insert(std::make_pair(L"Description", 1));
AvailableDolphinActionFields.insert(std::make_pair( L"Action Type", 2));
DolphinActionCollection.clear();
//now add the actions.
ActionInfoType CurrentAction;
CurrentAction.clear();
CurrentAction.insert(std::make_pair(0, boost::any(200)));
CurrentAction.insert(std::make_pair( 1, boost::any( wstrScreenRefresh)));
CurrentAction.insert(std::make_pair(2, boost::any(ID_ACTION)));
DolphinActionCollection.push_back(CurrentAction);
CurrentAction.clear();
CurrentAction.insert(std::make_pair(0, boost::any(201)));
CurrentAction.insert(std::make_pair(1, boost::any( wstrPassNextKey)));
CurrentAction.insert(std::make_pair(2, boost::any(ID_ACTION)));
DolphinActionCollection.push_back(CurrentAction);
CurrentAction.clear();
CurrentAction.insert(std::make_pair(0, boost::any(202)));
CurrentAction.insert(std::make_pair(1, boost::any( wstrToggleHotKeys)));
CurrentAction.insert(std::make_pair(2, boost::any(ID_ACTION)));
DolphinActionCollection.push_back(CurrentAction);
CurrentAction.clear();
CurrentAction.insert(std::make_pair(0, boost::any(2)));
CurrentAction.insert(std::make_pair(1, boost::any(wstrNavigationMode)));
CurrentAction.insert(std::make_pair(2, boost::any(ID_ACTION)));
DolphinActionCollection.push_back(CurrentAction);
CurrentAction.clear();
CurrentAction.insert(std::make_pair(0, boost::any(3)));
CurrentAction.insert(std::make_pair(1, boost::any(wstrDocumentReadMode)));
CurrentAction.insert(std::make_pair(2, boost::any(ID_ACTION)));
DolphinActionCollection.push_back(CurrentAction);
CurrentAction.clear();
CurrentAction.insert(std::make_pair(0, boost::any(4)));
CurrentAction.insert(std::make_pair(1, boost::any(wstrSwitchToPreviousLanguage)));
CurrentAction.insert(std::make_pair(2, boost::any(ID_ACTION)));
DolphinActionCollection.push_back(CurrentAction);
CurrentAction.clear();
CurrentAction.insert(std::make_pair(0, boost::any(5)));
CurrentAction.insert(std::make_pair(1, boost::any(wstrSwitchToNextLanguage)));
CurrentAction.insert(std::make_pair(2, boost::any(ID_ACTION)));
DolphinActionCollection.push_back(CurrentAction);
CurrentAction.clear();
CurrentAction.insert(std::make_pair(0, boost::any(35)));
CurrentAction.insert(std::make_pair(1, boost::any(wstrVolumeUp)));
CurrentAction.insert(std::make_pair(2, boost::any(ID_ACTION)));
DolphinActionCollection.push_back(CurrentAction);
CurrentAction.clear();
CurrentAction.insert(std::make_pair(0, boost::any(36)));
CurrentAction.insert(std::make_pair(1, boost::any(wstrVolumeDown)));
CurrentAction.insert(std::make_pair(2, boost::any(ID_ACTION)));
DolphinActionCollection.push_back(CurrentAction);
CurrentAction.clear();
CurrentAction.insert(std::make_pair(0, boost::any(37)));
CurrentAction.insert(std::make_pair(1, boost::any(wstrToggleSpeech)));
CurrentAction.insert(std::make_pair(2, boost::any(ID_ACTION)));
DolphinActionCollection.push_back(CurrentAction);
CurrentAction.clear();
CurrentAction.insert(std::make_pair(0, boost::any(38)));
CurrentAction.insert(std::make_pair(1, boost::any(wstrToggleRowColumnMode)));
CurrentAction.insert(std::make_pair(2, boost::any(ID_ACTION)));
DolphinActionCollection.push_back(CurrentAction);
CurrentAction.clear();
CurrentAction.insert(std::make_pair(0, boost::any(105)));
CurrentAction.insert(std::make_pair(1, boost::any(wstrReadFocus)));
CurrentAction.insert(std::make_pair(2, boost::any(ID_ACTION)));
DolphinActionCollection.push_back(CurrentAction);
CurrentAction.clear();
CurrentAction.insert(std::make_pair(0, boost::any(106)));
CurrentAction.insert(std::make_pair(1, boost::any(wstrSpellFocus)));
CurrentAction.insert(std::make_pair(2, boost::any(ID_ACTION)));
DolphinActionCollection.push_back(CurrentAction);
CurrentAction.clear();
CurrentAction.insert(std::make_pair(0, boost::any(107)));
CurrentAction.insert(std::make_pair(1, boost::any(wstrVerboseFocus)));
CurrentAction.insert(std::make_pair(2, boost::any(ID_ACTION)));
DolphinActionCollection.push_back(CurrentAction);
CurrentAction.clear();
CurrentAction.insert(std::make_pair(0, boost::any(108)));
CurrentAction.insert(std::make_pair(1, boost::any(wstrPhoneticFocus)));
CurrentAction.insert(std::make_pair(2, boost::any(ID_ACTION)));
DolphinActionCollection.push_back(CurrentAction);
CurrentAction.clear();
CurrentAction.insert(std::make_pair(0, boost::any(109)));
CurrentAction.insert(std::make_pair(1, boost::any(wstrReadTextToCursor)));
CurrentAction.insert(std::make_pair(2, boost::any(ID_ACTION)));
DolphinActionCollection.push_back(CurrentAction);
CurrentAction.clear();
CurrentAction.insert(std::make_pair(0, boost::any(110)));
CurrentAction.insert(std::make_pair(1, boost::any(wstrSpellTextToCursor)));
CurrentAction.insert(std::make_pair(2, boost::any(ID_ACTION)));
DolphinActionCollection.push_back(CurrentAction);
CurrentAction.clear();
CurrentAction.insert(std::make_pair(0, boost::any(111)));
CurrentAction.insert(std::make_pair(1, boost::any(wstrVerboseTextToCursor)));
CurrentAction.insert(std::make_pair(2, boost::any(ID_ACTION)));
DolphinActionCollection.push_back(CurrentAction);
CurrentAction.clear();
CurrentAction.insert(std::make_pair(0, boost::any(112)));
CurrentAction.insert(std::make_pair(1, boost::any(wstrPhoneticTextToCursor)));
CurrentAction.insert(std::make_pair(2, boost::any(ID_ACTION)));
DolphinActionCollection.push_back(CurrentAction);
CurrentAction.clear();
CurrentAction.insert(std::make_pair(0, boost::any(113)));
CurrentAction.insert(std::make_pair(1, boost::any(wstrReadTextFromCursor)));
CurrentAction.insert(std::make_pair(2, boost::any(ID_ACTION)));
DolphinActionCollection.push_back(CurrentAction);
CurrentAction.clear();
CurrentAction.insert(std::make_pair(0, boost::any(114)));
CurrentAction.insert(std::make_pair(1, boost::any(wstrSpellTextFromCursor)));
CurrentAction.insert(std::make_pair(2, boost::any(ID_ACTION)));
DolphinActionCollection.push_back(CurrentAction);
CurrentAction.clear();
CurrentAction.insert(std::make_pair(0, boost::any(115)));
CurrentAction.insert(std::make_pair(1, boost::any(wstrVerboseTextFromCursor)));
CurrentAction.insert(std::make_pair(2, boost::any(ID_ACTION)));
DolphinActionCollection.push_back(CurrentAction);
CurrentAction.clear();
CurrentAction.insert(std::make_pair(0, boost::any(116)));
CurrentAction.insert(std::make_pair(1, boost::any(wstrPhoneticTextFromCursor)));
CurrentAction.insert(std::make_pair(2, boost::any(ID_ACTION)));
DolphinActionCollection.push_back(CurrentAction);
CurrentAction.clear();
CurrentAction.insert(std::make_pair(0, boost::any(117)));
CurrentAction.insert(std::make_pair(1, boost::any(wstrReadSpreadsheetRowAndColumn)));
CurrentAction.insert(std::make_pair(2, boost::any(ID_ACTION)));
DolphinActionCollection.push_back(CurrentAction);
CurrentAction.clear();
CurrentAction.insert(std::make_pair(0, boost::any(118)));
CurrentAction.insert(std::make_pair(1, boost::any(wstrSpellSpreadsheetRowAndColumn)));
CurrentAction.insert(std::make_pair(2, boost::any(ID_ACTION)));
DolphinActionCollection.push_back(CurrentAction);
CurrentAction.clear();
CurrentAction.insert(std::make_pair(0, boost::any(119)));
CurrentAction.insert(std::make_pair(1, boost::any(wstrVerboseSpreadsheetRowAndColumn)));
CurrentAction.insert(std::make_pair(2, boost::any(ID_ACTION)));
DolphinActionCollection.push_back(CurrentAction);
CurrentAction.clear();
CurrentAction.insert(std::make_pair(0, boost::any(120)));
CurrentAction.insert(std::make_pair(1, boost::any(wstrPhoneticSpreadsheetRowAndColumn)));
CurrentAction.insert(std::make_pair(2, boost::any(ID_ACTION)));
DolphinActionCollection.push_back(CurrentAction);
CurrentAction.clear();
CurrentAction.insert(std::make_pair(0, boost::any(121)));
CurrentAction.insert(std::make_pair(1, boost::any(wstrReadCharacter)));
CurrentAction.insert(std::make_pair(2, boost::any(ID_ACTION)));
DolphinActionCollection.push_back(CurrentAction);
CurrentAction.clear();
CurrentAction.insert(std::make_pair(0, boost::any(122)));
CurrentAction.insert(std::make_pair(1, boost::any(wstrDescribeCharacterAttributes)));
CurrentAction.insert(std::make_pair(2, boost::any(ID_ACTION)));
DolphinActionCollection.push_back(CurrentAction);
CurrentAction.clear();
CurrentAction.insert(std::make_pair(0, boost::any(123)));
CurrentAction.insert(std::make_pair(1, boost::any(wstrDescribeCharacterAttributes)));
CurrentAction.insert(std::make_pair(2, boost::any(ID_ACTION)));
DolphinActionCollection.push_back(CurrentAction);
CurrentAction.clear();
CurrentAction.insert(std::make_pair(0, boost::any(124)));
CurrentAction.insert(std::make_pair(1, boost::any(wstrPhoneticCharacter)));
CurrentAction.insert(std::make_pair(2, boost::any(ID_ACTION)));
DolphinActionCollection.push_back(CurrentAction);
CurrentAction.clear();
CurrentAction.insert(std::make_pair(0, boost::any(125)));
CurrentAction.insert(std::make_pair(1, boost::any(wstrReadWord)));
CurrentAction.insert(std::make_pair(2, boost::any(ID_ACTION)));
DolphinActionCollection.push_back(CurrentAction);
CurrentAction.clear();
CurrentAction.insert(std::make_pair(0, boost::any(126)));
CurrentAction.insert(std::make_pair(1, boost::any(wstrSpellWord)));
CurrentAction.insert(std::make_pair(2, boost::any(ID_ACTION)));
DolphinActionCollection.push_back(CurrentAction);
CurrentAction.clear();
CurrentAction.insert(std::make_pair(0, boost::any(127)));
CurrentAction.insert(std::make_pair(1, boost::any(wstrVerboseWord)));
CurrentAction.insert(std::make_pair(2, boost::any(ID_ACTION)));
DolphinActionCollection.push_back(CurrentAction);
CurrentAction.clear();
CurrentAction.insert(std::make_pair(0, boost::any(128)));
CurrentAction.insert(std::make_pair(1, boost::any(wstrPhoneticWord)));
CurrentAction.insert(std::make_pair(2, boost::any(ID_ACTION)));
DolphinActionCollection.push_back(CurrentAction);
CurrentAction.clear();
CurrentAction.insert(std::make_pair(0, boost::any(129)));
CurrentAction.insert(std::make_pair(1, boost::any(wstrReadLine)));
CurrentAction.insert(std::make_pair(2, boost::any(ID_ACTION)));
DolphinActionCollection.push_back(CurrentAction);
CurrentAction.clear();
CurrentAction.insert(std::make_pair(0, boost::any(130)));
CurrentAction.insert(std::make_pair(1, boost::any(wstrSpellLine)));
CurrentAction.insert(std::make_pair(2, boost::any(ID_ACTION)));
DolphinActionCollection.push_back(CurrentAction);
CurrentAction.clear();
CurrentAction.insert(std::make_pair(0, boost::any(131)));
CurrentAction.insert(std::make_pair(1, boost::any(wstrVerboseLine)));
CurrentAction.insert(std::make_pair(2, boost::any(ID_ACTION)));
DolphinActionCollection.push_back(CurrentAction);
CurrentAction.clear();
CurrentAction.insert(std::make_pair(0, boost::any(132)));
CurrentAction.insert(std::make_pair(1, boost::any(wstrPhoneticLine)));
CurrentAction.insert(std::make_pair(2, boost::any(ID_ACTION)));
DolphinActionCollection.push_back(CurrentAction);
CurrentAction.clear();
CurrentAction.insert(std::make_pair(0, boost::any(134)));
CurrentAction.insert(std::make_pair(1, boost::any(wstrReadWindow)));
CurrentAction.insert(std::make_pair(2, boost::any(ID_ACTION)));
DolphinActionCollection.push_back(CurrentAction);
CurrentAction.clear();
CurrentAction.insert(std::make_pair(0, boost::any(135)));
CurrentAction.insert(std::make_pair(1, boost::any(wstrVerboseWindow)));
CurrentAction.insert(std::make_pair(2, boost::any(ID_ACTION)));
DolphinActionCollection.push_back(CurrentAction);
CurrentAction.clear();
CurrentAction.insert(std::make_pair(0, boost::any(136)));
CurrentAction.insert(std::make_pair(1, boost::any(wstrWhereAmI)));
CurrentAction.insert(std::make_pair(2, boost::any(ID_ACTION)));
DolphinActionCollection.push_back(CurrentAction);
CurrentAction.clear();
CurrentAction.insert(std::make_pair(0, boost::any(137)));
CurrentAction.insert(std::make_pair(1, boost::any(wstrReadShortcut)));
CurrentAction.insert(std::make_pair(2, boost::any(ID_ACTION)));
DolphinActionCollection.push_back(CurrentAction);
CurrentAction.clear();
CurrentAction.insert(std::make_pair(0, boost::any(139)));
CurrentAction.insert(std::make_pair(1, boost::any(wstrReadControl)));
CurrentAction.insert(std::make_pair(2, boost::any(ID_ACTION)));
DolphinActionCollection.push_back(CurrentAction);
CurrentAction.clear();
CurrentAction.insert(std::make_pair(0, boost::any(140)));
CurrentAction.insert(std::make_pair(1, boost::any(wstrVerboseControl)));
CurrentAction.insert(std::make_pair(2, boost::any(ID_ACTION)));
DolphinActionCollection.push_back(CurrentAction);
CurrentAction.clear();
CurrentAction.insert(std::make_pair(0, boost::any(14)));
CurrentAction.insert(std::make_pair(1, boost::any(wstr1Mute)));
CurrentAction.insert(std::make_pair(2, boost::any(ID_ACTION)));
DolphinActionCollection.push_back(CurrentAction);
CurrentAction.clear();
CurrentAction.insert(std::make_pair(0, boost::any(185)));
CurrentAction.insert(std::make_pair(1, boost::any(wstrReadStatusBar)));
CurrentAction.insert(std::make_pair(2, boost::any(ID_ACTION)));
DolphinActionCollection.push_back(CurrentAction);
CurrentAction.clear();
CurrentAction.insert(std::make_pair(0, boost::any(186)));
CurrentAction.insert(std::make_pair(1, boost::any(wstrSpellStatusBar)));
CurrentAction.insert(std::make_pair(2, boost::any(ID_ACTION)));
DolphinActionCollection.push_back(CurrentAction);
CurrentAction.clear();
CurrentAction.insert(std::make_pair(0, boost::any(187)));
CurrentAction.insert(std::make_pair(1, boost::any(wstrVerboseStatusBar)));
CurrentAction.insert(std::make_pair(2, boost::any(ID_ACTION)));
DolphinActionCollection.push_back(CurrentAction);
CurrentAction.clear();
CurrentAction.insert(std::make_pair(0, boost::any(188)));
CurrentAction.insert(std::make_pair(1, boost::any(wstrPhoneticStatusBar)));
CurrentAction.insert(std::make_pair(2, boost::any(ID_ACTION)));
DolphinActionCollection.push_back(CurrentAction);
CurrentAction.clear();
CurrentAction.insert(std::make_pair(0, boost::any(198)));
CurrentAction.insert(std::make_pair(1, boost::any(wstrSpeedUp)));
CurrentAction.insert(std::make_pair(2, boost::any(ID_ACTION)));
DolphinActionCollection.push_back(CurrentAction);
CurrentAction.clear();
CurrentAction.insert(std::make_pair(0, boost::any(199)));
CurrentAction.insert(std::make_pair(1, boost::any(wstrSpeedDown)));
CurrentAction.insert(std::make_pair(2, boost::any(ID_ACTION)));
DolphinActionCollection.push_back(CurrentAction);
CurrentAction.clear();
CurrentAction.insert(std::make_pair(0, boost::any(76)));
CurrentAction.insert(std::make_pair(1, boost::any(wstrBrailleBack)));
CurrentAction.insert(std::make_pair(2, boost::any(ID_ACTION)));
DolphinActionCollection.push_back(CurrentAction);
CurrentAction.clear();
CurrentAction.insert(std::make_pair(0, boost::any(77)));
CurrentAction.insert(std::make_pair(1, boost::any(wstrBrailleForward)));
CurrentAction.insert(std::make_pair(2, boost::any(ID_ACTION)));
DolphinActionCollection.push_back(CurrentAction);
CurrentAction.clear();
CurrentAction.insert(std::make_pair(0, boost::any(78)));
CurrentAction.insert(std::make_pair(1, boost::any(wstrBrailleLeft)));
CurrentAction.insert(std::make_pair(2, boost::any(ID_ACTION)));
DolphinActionCollection.push_back(CurrentAction);
CurrentAction.clear();
CurrentAction.insert(std::make_pair(0, boost::any(79)));
CurrentAction.insert(std::make_pair(1, boost::any(wstrBrailleRight)));
CurrentAction.insert(std::make_pair(2, boost::any(ID_ACTION)));
DolphinActionCollection.push_back(CurrentAction);
CurrentAction.clear();
CurrentAction.insert(std::make_pair(0, boost::any(80)));
CurrentAction.insert(std::make_pair(1, boost::any(wstrBrailleWidthLeft)));
CurrentAction.insert(std::make_pair(2, boost::any(ID_ACTION)));
DolphinActionCollection.push_back(CurrentAction);
CurrentAction.clear();
CurrentAction.insert(std::make_pair(0, boost::any(81)));
CurrentAction.insert(std::make_pair(1, boost::any(wstrBrailleWidthRight)));
CurrentAction.insert(std::make_pair(2, boost::any(ID_ACTION)));
DolphinActionCollection.push_back(CurrentAction);
CurrentAction.clear();
CurrentAction.insert(std::make_pair(0, boost::any(82)));
CurrentAction.insert(std::make_pair(1, boost::any(wstrBrailleHalfWidthLeft)));
CurrentAction.insert(std::make_pair(2, boost::any(ID_ACTION)));
DolphinActionCollection.push_back(CurrentAction);
CurrentAction.clear();
CurrentAction.insert(std::make_pair(0, boost::any(83)));
CurrentAction.insert(std::make_pair(1, boost::any(wstrBrailleHalfWidthRight)));
CurrentAction.insert(std::make_pair(2, boost::any(ID_ACTION)));
DolphinActionCollection.push_back(CurrentAction);
CurrentAction.clear();
CurrentAction.insert(std::make_pair(0, boost::any(84)));
CurrentAction.insert(std::make_pair(1, boost::any(wstrBrailleGoToFocus)));
CurrentAction.insert(std::make_pair(2, boost::any(ID_ACTION)));
DolphinActionCollection.push_back(CurrentAction);
CurrentAction.clear();
CurrentAction.insert(std::make_pair(0, boost::any(85)));
CurrentAction.insert(std::make_pair(1, boost::any(wstrBrailleHome)));
CurrentAction.insert(std::make_pair(2, boost::any(ID_ACTION)));
DolphinActionCollection.push_back(CurrentAction);
CurrentAction.clear();
CurrentAction.insert(std::make_pair(0, boost::any(86)));
CurrentAction.insert(std::make_pair(1, boost::any(wstrBrailleEnd)));
CurrentAction.insert(std::make_pair(2, boost::any(ID_ACTION)));
DolphinActionCollection.push_back(CurrentAction);
CurrentAction.clear();
CurrentAction.insert(std::make_pair(0, boost::any(175)));
CurrentAction.insert(std::make_pair(1, boost::any(wstrToggleBraille)));
CurrentAction.insert(std::make_pair(2, boost::any(ID_ACTION)));
DolphinActionCollection.push_back(CurrentAction);
CurrentAction.clear();
CurrentAction.insert(std::make_pair(0, boost::any(176)));
CurrentAction.insert(std::make_pair(1, boost::any(wstrDisplayCharacters)));
CurrentAction.insert(std::make_pair(2, boost::any(ID_ACTION)));
DolphinActionCollection.push_back(CurrentAction);
CurrentAction.clear();
CurrentAction.insert(std::make_pair(0, boost::any(177)));
CurrentAction.insert(std::make_pair(1, boost::any(wstrCycleAttributes)));
CurrentAction.insert(std::make_pair(2, boost::any(ID_ACTION)));
DolphinActionCollection.push_back(CurrentAction);
CurrentAction.clear();
CurrentAction.insert(std::make_pair(0, boost::any(178)));
CurrentAction.insert(std::make_pair(1, boost::any(wstrDisplayFontSize)));
CurrentAction.insert(std::make_pair(2, boost::any(ID_ACTION)));
DolphinActionCollection.push_back(CurrentAction);
CurrentAction.clear();
CurrentAction.insert(std::make_pair(0, boost::any(179)));
CurrentAction.insert(std::make_pair(1, boost::any(wstrDisplayFontStyle)));
CurrentAction.insert(std::make_pair(2, boost::any(ID_ACTION)));
DolphinActionCollection.push_back(CurrentAction);
CurrentAction.clear();
CurrentAction.insert(std::make_pair(0, boost::any(180)));
CurrentAction.insert(std::make_pair(1, boost::any(wstrDisplayFontColours)));
CurrentAction.insert(std::make_pair(2, boost::any(ID_ACTION)));
DolphinActionCollection.push_back(CurrentAction);
CurrentAction.clear();
CurrentAction.insert(std::make_pair(0, boost::any(181)));
CurrentAction.insert(std::make_pair(1, boost::any(wstrDisplayFontForegroundColour)));
CurrentAction.insert(std::make_pair(2, boost::any(ID_ACTION)));
DolphinActionCollection.push_back(CurrentAction);
CurrentAction.clear();
CurrentAction.insert(std::make_pair(0, boost::any(182)));
CurrentAction.insert(std::make_pair(1, boost::any(wstrDisplayFontName)));
CurrentAction.insert(std::make_pair(2, boost::any(ID_ACTION)));
DolphinActionCollection.push_back(CurrentAction);
CurrentAction.clear();
CurrentAction.insert(std::make_pair(0, boost::any(183)));
CurrentAction.insert(std::make_pair(1, boost::any(wstrDescribeCharacter)));
CurrentAction.insert(std::make_pair(2, boost::any(ID_ACTION)));
DolphinActionCollection.push_back(CurrentAction);
CurrentAction.clear();
CurrentAction.insert(std::make_pair(0, boost::any(184)));
CurrentAction.insert(std::make_pair(1, boost::any(wstrToggleCursor)));
CurrentAction.insert(std::make_pair(2, boost::any(ID_ACTION)));
DolphinActionCollection.push_back(CurrentAction);
CurrentAction.clear();
CurrentAction.insert(std::make_pair(0, boost::any(189)));
CurrentAction.insert(std::make_pair(1, boost::any(wstrToggleTracking)));
CurrentAction.insert(std::make_pair(2, boost::any(ID_ACTION)));
DolphinActionCollection.push_back(CurrentAction);
CurrentAction.clear();
CurrentAction.insert(std::make_pair(0, boost::any(190)));
CurrentAction.insert(std::make_pair(1, boost::any(wstrBrailleLineDown)));
CurrentAction.insert(std::make_pair(2, boost::any(ID_ACTION)));
DolphinActionCollection.push_back(CurrentAction);
CurrentAction.clear();
CurrentAction.insert(std::make_pair(0, boost::any(191)));
CurrentAction.insert(std::make_pair(1, boost::any(wstrBrailleLineUp)));
CurrentAction.insert(std::make_pair(2, boost::any(ID_ACTION)));
DolphinActionCollection.push_back(CurrentAction);
CurrentAction.clear();
CurrentAction.insert(std::make_pair(0, boost::any(192)));
CurrentAction.insert(std::make_pair(1, boost::any(wstrBrailleBottom)));
CurrentAction.insert(std::make_pair(2, boost::any(ID_ACTION)));
DolphinActionCollection.push_back(CurrentAction);
CurrentAction.clear();
CurrentAction.insert(std::make_pair(0, boost::any(193)));
CurrentAction.insert(std::make_pair(1, boost::any(wstrTBrailleTP)));
CurrentAction.insert(std::make_pair(2, boost::any(ID_ACTION)));
DolphinActionCollection.push_back(CurrentAction);
CurrentAction.clear();
CurrentAction.insert(std::make_pair(0, boost::any(197)));
CurrentAction.insert(std::make_pair(1, boost::any(wstrToggleLayoutMode)));
CurrentAction.insert(std::make_pair(2, boost::any(ID_ACTION)));
DolphinActionCollection.push_back(CurrentAction);
CurrentAction.clear();
CurrentAction.insert(std::make_pair(0, boost::any(217)));
CurrentAction.insert(std::make_pair(1, boost::any(wstrToggleLiteraryBraille)));
CurrentAction.insert(std::make_pair(2, boost::any(ID_ACTION)));
DolphinActionCollection.push_back(CurrentAction);
CurrentAction.clear();
CurrentAction.insert(std::make_pair(0, boost::any(218)));
CurrentAction.insert(std::make_pair(1, boost::any(wstrTogglePhysicalMode)));
CurrentAction.insert(std::make_pair(2, boost::any(ID_ACTION)));
DolphinActionCollection.push_back(CurrentAction);
CurrentAction.clear();
CurrentAction.insert(std::make_pair(0, boost::any(142)));
CurrentAction.insert(std::make_pair(1, boost::any(wstrIncreaseMag)));
CurrentAction.insert(std::make_pair(2, boost::any(ID_ACTION)));
DolphinActionCollection.push_back(CurrentAction);
CurrentAction.clear();
CurrentAction.insert(std::make_pair(0, boost::any(143)));
CurrentAction.insert(std::make_pair(1, boost::any(wstrDecreaseMag)));
CurrentAction.insert(std::make_pair(2, boost::any(ID_ACTION)));
DolphinActionCollection.push_back(CurrentAction);
CurrentAction.clear();
CurrentAction.insert(std::make_pair(0, boost::any(146)));
CurrentAction.insert(std::make_pair(1, boost::any(wstrIncrementXMag)));
CurrentAction.insert(std::make_pair(2, boost::any(ID_ACTION)));
DolphinActionCollection.push_back(CurrentAction);
CurrentAction.clear();
CurrentAction.insert(std::make_pair(0, boost::any(147)));
CurrentAction.insert(std::make_pair(1, boost::any(wstrDecrementXMag)));
CurrentAction.insert(std::make_pair(2, boost::any(ID_ACTION)));
DolphinActionCollection.push_back(CurrentAction);
CurrentAction.clear();
CurrentAction.insert(std::make_pair(0, boost::any(148)));
CurrentAction.insert(std::make_pair(1, boost::any(wstrIncrementYMag)));
CurrentAction.insert(std::make_pair(2, boost::any(ID_ACTION)));
DolphinActionCollection.push_back(CurrentAction);
CurrentAction.clear();
CurrentAction.insert(std::make_pair(0, boost::any(149)));
CurrentAction.insert(std::make_pair(1, boost::any(wstrDecrementYMag)));
CurrentAction.insert(std::make_pair(2, boost::any(ID_ACTION)));
DolphinActionCollection.push_back(CurrentAction);
CurrentAction.clear();
CurrentAction.insert(std::make_pair(0, boost::any(150)));
CurrentAction.insert(std::make_pair(1, boost::any(wstrToggleMag)));
CurrentAction.insert(std::make_pair(2, boost::any(ID_ACTION)));
DolphinActionCollection.push_back(CurrentAction);
CurrentAction.clear();
CurrentAction.insert(std::make_pair(0, boost::any(151)));
CurrentAction.insert(std::make_pair(1, boost::any(wstrToggleColourChanger)));
CurrentAction.insert(std::make_pair(2, boost::any(ID_ACTION)));
DolphinActionCollection.push_back(CurrentAction);
CurrentAction.clear();
CurrentAction.insert(std::make_pair(0, boost::any(152)));
CurrentAction.insert(std::make_pair(1, boost::any(wstrToggleOverviewMode)));
CurrentAction.insert(std::make_pair(2, boost::any(ID_ACTION)));
DolphinActionCollection.push_back(CurrentAction);
CurrentAction.clear();
CurrentAction.insert(std::make_pair(0, boost::any(153)));
CurrentAction.insert(std::make_pair(1, boost::any(wstrCyclePrimaryAreaType)));
CurrentAction.insert(std::make_pair(2, boost::any(ID_ACTION)));
DolphinActionCollection.push_back(CurrentAction);
CurrentAction.clear();
CurrentAction.insert(std::make_pair(0, boost::any(155)));
CurrentAction.insert(std::make_pair(1, boost::any(wstrCycleOrientation)));
CurrentAction.insert(std::make_pair(2, boost::any(ID_ACTION)));
DolphinActionCollection.push_back(CurrentAction);
CurrentAction.clear();
CurrentAction.insert(std::make_pair(0, boost::any(156)));
CurrentAction.insert(std::make_pair(1, boost::any(wstrImageSmoothing)));
CurrentAction.insert(std::make_pair(2, boost::any(ID_ACTION)));
DolphinActionCollection.push_back(CurrentAction);
CurrentAction.clear();
CurrentAction.insert(std::make_pair(0, boost::any(157)));
CurrentAction.insert(std::make_pair(1, boost::any(wstrToggleMagTracking)));
CurrentAction.insert(std::make_pair(2, boost::any(ID_ACTION)));
DolphinActionCollection.push_back(CurrentAction);
CurrentAction.clear();
CurrentAction.insert(std::make_pair(0, boost::any(158)));
CurrentAction.insert(std::make_pair(1, boost::any(wstrDisplayHookedAreas)));
CurrentAction.insert(std::make_pair(2, boost::any(ID_ACTION)));
DolphinActionCollection.push_back(CurrentAction);
CurrentAction.clear();
CurrentAction.insert(std::make_pair(0, boost::any(204)));
CurrentAction.insert(std::make_pair(1, boost::any(wstrLineViewWholeWindows)));
CurrentAction.insert(std::make_pair(2, boost::any(ID_ACTION)));
DolphinActionCollection.push_back(CurrentAction);
ActionInformation =std::make_pair(AvailableDolphinActionFields, DolphinActionCollection); //return what is required.
		return S_OK;
#else
return E_NOTIMPL;
#endif
}
	
HRESULT RBSpeech::ExecuteDolphinAction(boost::any& name, ScreenReaderActionType& type, DWORD& ReturnCode)
	{
#ifndef _WIN64
		HRESULT hReturnValue =S_OK;
		DWORD ReturnValue =-1;
		DWORD DolphinCommand =-1; //store the dolphin command.
		// wchar_t *ScriptStr;
	// DWORD ScriptStrLen;
ExitOnNull(ReturnCode, hReturnValue, S_FALSE, "No exact return code was supplied.");
//determine what action type we have.
switch(type)
{
case ID_ACTION:
	ExitOnFalse(IsInt( name), hReturnValue, S_FALSE, "the command ID supplied is not an int."); //see if the name parameter contains an int, if it doesn't exit.
	DolphinCommand =(DWORD) AsInt( name); //convert to a DWORD.
	ReturnValue =DolAccess_Action(DolphinCommand);
switch(ReturnValue)
{
case 0:
	ReturnCode =ID_SUCCESS;
break;
case 1:
	ReturnCode =ID_No_Server;
	hReturnValue =S_FALSE;
	break;
case 2:
	ReturnCode =		ID_INVALID_ID;
	hReturnValue =S_FALSE;
	break;
case 3:
	ReturnCode =ID_INVALID_LENGTH;
	hReturnValue =S_FALSE;
	break;
case 4:
	ReturnCode =ID_INVALID_ACTION;
	hReturnValue =S_FALSE;
	break;
case 5:
	ReturnCode =ID_ALREADY_IN_USE;
	hReturnValue =S_FALSE;
	break;
case 6: 
	ReturnCode =ID_SUPERNOVA_CONTROL_PANEL_IN_USE;
	hReturnValue =S_FALSE;
	break;
case 7:
	ReturnValue =ID_SUPERNOVA_NOT_RESPONDING;
	hReturnValue =S_FALSE;
	break;
case 8:
	ReturnCode =ID_SET_VAL_FAILED_PARAMETER_READONLY;
	hReturnValue =S_FALSE;
	break;
default:
	hReturnValue =S_FALSE;
	break;
};
default:
hReturnValue =S_FALSE;
	break;
};
LExit:
return hReturnValue;
#else
		return E_NOTIMPL;
#endif
}
	
//System Access.
/// <summary> Determine whether Dolphin is currently active in memory. </summary>
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
	BOOL bIsProcess64Bit = false;

	if (!SystemAccessDllApi.is_loaded())
{
		std::experimental::filesystem::path SystemAccessDllFileName(wxStandardPaths::Get().GetExecutablePath().ToStdWstring()); //assign the executable directory.
		SystemAccessDllFileName.remove_filename();
		
		hr = ProcWow64(::GetCurrentProcess(), &bIsProcess64Bit);
		if (hr == S_FALSE)
		{
			return false;
		}

		if (bIsProcess64Bit)
		{ //We are running on a 64-bit operating system--or at least as a 64-bit process.
			SystemAccessDllFileName /= L"SAAPI64.dll";
		}
		else { //We are running on a 32-bit o/s.
			SystemAccessDllFileName /= L"SAAPI32.dll";
		}
		
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
	
HRESULT RBSpeech::SystemAccessSpeak(wstring strText, BOOL blnSilence)
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
	
	HRESULT RBSpeech::SystemAccessBraille(wstring strText)
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

	
	HRESULT RBSpeech::GetActiveProduct(ActiveProduct& Product, DolphinProduct& SpecificDolphinProduct)
{
HRESULT hReturnValue =S_OK;
DolphinProduct ExactDolphinProduct =ID_NOPRODUCT; //we always obtain even if we can't output.
// tim test
// ExitOnNull(Product, hReturnValue, S_FALSE, "The variable passed in to recieve the product cannot be null.");

//check to see what is active.
if (IsJAWSActive())
{ //JAWS IS ACTIVE.
	Product = ID_JAWS;
} //End JAWS active block.
else if (IsDolphinActive(ExactDolphinProduct))
	{ //Dolphin is active.
		Product =ID_DOLPHIN;
		if (SpecificDolphinProduct ==NULL)
		{ //exit telling the user dolphin is active, but that we carn't tell them what screen reader is active.
			ExitFunctionWithMessageAndStatusCode(hReturnValue, S_OK, "Dolphin is active but due to the fact that the variable to contain the specific dolphin product was NULL the exact product couldn't be provided.");
		} //end unable to tell what screen reader.
		else 
		{ //output dolphin product.
			SpecificDolphinProduct =ExactDolphinProduct;
		} //end dolphin product output.
	} //Dolphin is active.
else if (IsNVDAActive())
	{ //NVDA is active.
		Product =ID_NVDA;
	} //End NVDA active block.
			
else if (IsSystemAccessActive())
	{ //System Access is active.
	Product =ID_SYSTEMACCESS;
	} //System Access is active.
	
	else
	{ //no product is active.
		Product =ID_NONE;
		hReturnValue =S_FALSE;
	} //No product is active.
LExit:
return hReturnValue;
}

bool RBSpeech::LoadAPI()
{
	ActiveProduct CurrentProduct;
	DolphinProduct SpecificDolphinProduct;

	if (GetActiveProduct(CurrentProduct, SpecificDolphinProduct))
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
		
	case ID_DOLPHIN:
					{
						return LoadDolphinApi();
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
	DolphinProduct SpecificDolphinProduct;
	GetActiveProduct(CurrentProduct, SpecificDolphinProduct);
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
		case ID_DOLPHIN:
				UnloadDolphinApi();
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

HRESULT RBSpeech::Speak(wstring strText, DolphinReturnCode& ReturnCode)
{
	return Speak(strText, false, ReturnCode);
}
	
HRESULT RBSpeech::Speak(wstring strText, BOOL blnSilence, DolphinReturnCode& ReturnCode)
{
	HRESULT hReturnValue =S_FALSE;
	ActiveProduct CurrentProduct;
	DolphinProduct SpecificDolphinProduct;
	hReturnValue =GetActiveProduct(CurrentProduct, SpecificDolphinProduct);
	ExitOnFailure(hReturnValue, "No product is active.");
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
		case ID_DOLPHIN:
					hReturnValue =DolphinSpeak(strText, blnSilence, ReturnCode);
			break;
		default:
		hReturnValue =E_NOTIMPL;
			break;
	}
	LExit:
	return hReturnValue;
}

HRESULT RBSpeech::Silence(DolphinReturnCode& ReturnCode)
{
	HRESULT hReturnValue =S_FALSE;
	ActiveProduct CurrentProduct;
	DolphinProduct SpecificDolphinProduct;
	hReturnValue =GetActiveProduct(CurrentProduct, SpecificDolphinProduct);
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
		case ID_DOLPHIN:
					hReturnValue =DolphinSilence(ReturnCode);
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
	DolphinProduct SpecificDolphinProduct;
	boost::property_tree::ptree IniTree; //used to store hsc information.
		boost::property_tree::ptree::assoc_iterator it;	 
	char SetNameStr[MAX_PATH] ="";
	char SpotNameStr[MAX_PATH] ="";
		wxMBConvStrictUTF8 ConvertString; //used to convert to ansi later on.
	ExitOnTrue(SetName.empty(), hReturnValue, S_FALSE, "No hot spot set has been provided.");
	ExitOnTrue(SpotName.empty(), hReturnValue, S_FALSE, "No hot spot name has been provided.");
	ExitOnFalse(std::experimental::filesystem::exists(SetName), hReturnValue, S_FALSE, "The hot spot set does not exist.");
		hReturnValue =GetActiveProduct(CurrentProduct, SpecificDolphinProduct);
	ExitOnFailure(hReturnValue, "No product is active.");
	switch(CurrentProduct)
	{
	case ID_JAWS:
		ConvertString.FromWChar(&SetNameStr[0], MAX_PATH, SetName.c_str(), SetName.size());
		boost::property_tree::ini_parser::read_ini(&SetNameStr[0], IniTree);		
		//try and find the key.
		ConvertString.FromWChar(&SpotNameStr[0], MAX_PATH, SpotName.c_str(), SpotName.size());
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
		DolphinProduct SpecificDolphinProduct;
		std::wstring CurrentSet;
		ExitOnTrue(SpotName.empty(), hReturnValue, S_FALSE, "No hot spot name has been provided.");
		hReturnValue =GetActiveProduct(CurrentProduct, SpecificDolphinProduct);	
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
		DolphinProduct SpecificDolphinProduct;
		boost::property_tree::ptree IniTree; //used to store hsc information.
		boost::property_tree::ptree::assoc_iterator it;	 
	char SetNameStr[MAX_PATH] ="";
		wxMBConvStrictUTF8 ConvertString; //used to convert to ansi later on.
	ExitOnTrue(SetName.empty(), hReturnValue, S_FALSE, "No hot spot set has been provided.");
	ExitOnFalse(std::experimental::filesystem::exists(SetName), hReturnValue, S_FALSE, "The hot spot set does not exist.");
	hReturnValue =GetActiveProduct(CurrentProduct, SpecificDolphinProduct);
ExitOnFailure(hReturnValue, "No product is active.");
	switch(CurrentProduct)
	{
	case ID_JAWS:
		ConvertString.FromWChar(&SetNameStr[0], MAX_PATH, SetName.c_str(), SetName.size());
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
			BOOST_FOREACH( boost::property_tree::ptree::value_type &v, IniTree)
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
	DolphinProduct SpecificDolphinProduct;
	std::wstring CurrentSet;
	hReturnValue =GetActiveProduct(CurrentProduct, SpecificDolphinProduct);
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
			wchar_t ConvertedSpotString[MAX_PATH];
			wxMBConvStrictUTF8 ConvertString; //used to convert to unicode later on.
			boost::property_tree::ptree IniTree; //used to store hsc information.
			std::wstring JAWSFunctionCallString =L"GetCurrentJAWSEnvironment(\"%s\")"; //used to hold the call to JAWS.
				std::experimental::filesystem::path IniFile; //file to store the hsc information.
			ActiveProduct CurrentProduct;
	DolphinProduct SpecificDolphinProduct;
	hr =GetActiveProduct(CurrentProduct, SpecificDolphinProduct);
	ExitOnFailure(hr, "No product is active.");
				switch(CurrentProduct)
			{
			case ID_JAWS:
				IniFile =AppDataPath().native();
//now, set the filename.
IniFile /=L"CurrentJawsEnvironment.ini";
if (exists(IniFile))
{ //the file exists remove it.
	ExitOnFalse(std::experimental::filesystem::remove(IniFile), hr, S_FALSE, "Unable to delete the old file.");
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
									ConvertString.ToWChar(&ConvertedSpotString[0], MAX_PATH, CurrentSpotStringOptional.get().c_str(), CurrentSpotStringOptional.get().size());
ActiveSet =&ConvertedSpotString[0]; //Actually output.
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
	DolphinProduct SpecificDolphinProduct;
		std::wstring JAWSFunctionCallString =L"HSCLookupKey( \"%s\")"; //used to hold the call to JAWS.
		CComVariant vFunctionResult;
		ExitOnTrue(Set.empty(), hr, S_FALSE, "No hot spot set has been provided.");
		ExitOnTrue(SpotName.empty(), hr, S_FALSE, "No hot spot name has been provided.");
			hr =GetActiveProduct(CurrentProduct, SpecificDolphinProduct);
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
	DolphinProduct SpecificDolphinProduct;
DWORD ReturnCode =-1;
	hReturnValue =GetActiveProduct(CurrentProduct, SpecificDolphinProduct);
	ExitOnFailure(hReturnValue, "No product is active.");
				
	switch(CurrentProduct)
			{
			case ID_JAWS:
hReturnValue =ExecuteJAWSAction(Action, Type);
				break;
case ID_DOLPHIN:
hReturnValue =ExecuteDolphinAction(boost::any(Action), Type, ReturnCode);
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
	DolphinProduct SpecificDolphinProduct;
	hReturnValue =GetActiveProduct(CurrentProduct, SpecificDolphinProduct);
	ExitOnFailure(hReturnValue, "No product is active.");
				switch(CurrentProduct)
			{
			case ID_JAWS:
hReturnValue =GetAvailableJAWSActions(AvailableActions);
				break;
case ID_DOLPHIN:
hReturnValue =GetAvailableDolphinActions(AvailableActions);
break;
default:
hReturnValue =E_NOTIMPL;
break;
				};
LExit:
ActionInformation =AvailableActions;
return hReturnValue;
}

std::vector<JawsFunction> RBSpeech::ProcessJSDFile(std::experimental::filesystem::path&File)
{
	std::vector<JawsFunction> AvailableFunctions;
	bool ParameterIsOptional =false;
	if (std::experimental::filesystem::exists(File))
	{ //existence check.
		std::wifstream fs(File);
		//Read the file in to a vector.
		std::vector<std::wstring> lines;
		std::wstring str;
		while (std::getline(fs, str))
		{
			lines.push_back(str);
		}
			fs.close();
		
			JawsFunction newfunction;
	BOOST_FOREACH(std::wstring line, lines)
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

std::wstring UserName =wxGetUserId().ToStdWstring();
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

		std::vector <JAWSParameter> AsJawsParameterVector( const boost::any  operand)
{
std::vector <JAWSParameter> myParameters;

try
	{
		myParameters = any_cast<std::vector <JAWSParameter> >( operand);
return myParameters;
}
catch( const boost::bad_any_cast &)
{
return myParameters;
}
}

		int AsInt( const boost::any  operand)
{
    try
	{
		int nOut = any_cast<int>( operand);
		return nOut;
}
catch( const boost::bad_any_cast &)
{
return -1;
}
}

std::wstring AsWString( const boost::any operand)
	{
try
	{
		std::wstring wstrOut = any_cast<std::wstring>( operand);
		return wstrOut;
}
catch( const boost::bad_any_cast &)
{
return wstrEmpty;
}
}

bool IsWString(const boost::any & operand)
{
    return operand.type() == typeid( std::wstring);
}

bool IsInt(const boost::any & operand)
{
    return operand.type() == typeid(int);
}

bool IsJawsParameterVector(const boost::any & operand)
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

#endif  // Windows
