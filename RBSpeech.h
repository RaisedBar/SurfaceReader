//RBSpeech.h
// speech utilities class header file
//Copyright (�) 2012 Raised Bar Ltd.

#ifndef RB_SPEECH_H
#define RB_SPEECH_H

#pragma once

#include <string>
#include <map>
#include <vector>
#include <algorithm>

#ifndef __WINDOWS__
#ifdef check
#undef check
#endif
#endif

#include "RBStringFuncs.h"
#include "Serializer.h"
#include "SRConstants.h"

//boost includes.
#include <boost/any.hpp>
#include <boost/assign/list_of.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/ini_parser.hpp>
#include <boost/foreach.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/optional/optional.hpp>
#include <boost/dll/shared_library.hpp>
#include <boost/dll/import.hpp>
using boost::any_cast;
using namespace std;
// using namespace boost; //used for assignment and EBNF parsing amongst others.
#include <wx/dynlib.h>
#include <wx/ffile.h>
#include <wx/stdpaths.h>
#include <wx/tokenzr.h>
#include <wx/msw/winundef.h> 


//defines.
#define HOTSPOT_HIDDEN 1 //A hot spot is hidden.

struct JAWSParameter 
{
	JAWSParameter()
	{
		Optional =false;
	}
// 	BOOL Optional;
	bool Optional;
std::wstring DataType;
std::wstring Description;
std::wstring Name;
};

struct JawsReturnInfo
{
	std::wstring DataType;
std::wstring Description;
};

//enum defining the jsd file to start processing.
enum ProcessJsdFileType 
{
	PROCESS_NO_FILE,
	PROCESS_USER_APP_FILE,
	PROCESS_DEFAULT_APP_FILE,
	PROCESS_USER_SYSTEM_DEFAULT_FILE,
	PROCESS_DEFAULT_SYSTEM_DEFAULT_FILE
};


enum FunctionType 
{ 
	ID_TYPE_NONE, //place holder for resetting.
	ID_TYPE_FUNCTION, //function.
	ID_TYPE_SCRIPT //script.
};

struct JawsFunction 
{
	std::wstring Name; //function/script name.
	std::wstring Synopsis; //Synopsis of the script/function.
	std::wstring Description; //the function/scripts  description.
	std::wstring Category; //the function/script category.
	FunctionType Type;
std::vector<JAWSParameter> Parameters;
JawsReturnInfo Returns;
};

/// <summary> The type containing a single entry for an access technology action. </summary>
/// <remarks> this type contains an integer on the left corresponding to the integer in the AvailableActionFieldsType, and a boost::any on the right allowing the storing of any data type supported. This doesn't tie us to returning strings where they are not the most efficient way to represent the data. </remarks> 
typedef std::map<int, boost::any> ActionInfoType; 
typedef std::map<int, boost::any>::iterator ActionInfoTypeIterator;
/// <summary> A vector of <see cref="ActionInfoType"/> containing all available actions for the currently active access technology. </summary>
typedef std::vector<ActionInfoType > ActionCollectionType; 
typedef std::vector<ActionInfoType >::iterator ActionCollectionTypeIterator; 
typedef std::map < std::pair< std::wstring, std::string>, ActionCollectionType> JawsCacheType;

// <summary> the available action fields. </summary>
/// <remarks> These fields are unique to the access technology that is currently active. They define the fields available in the <see cref="ActionInfoType"/>
/// Further documentation can be found in the respective Get*Actions methods. </remarks>
typedef std::map<std::wstring, int> AvailableActionFieldsType;
typedef std::map<std::wstring, int>::iterator AvailableActionFieldsIterator;
typedef pair<AvailableActionFieldsType, ActionCollectionType > AvailableActionsType; //Available actions pair.

/// <summary> 
///ActiveProduct enumeration determining which supported product is currently active.
/// </summery>
enum ActiveProduct 
{
	/// <summary>No access technology is active.</summary>
	ID_NONE,
/// <summary>JAWS for windows is active.</summary>
		ID_JAWS, 
	/// <summary>System Access is active.</summary>
		ID_SYSTEMACCESS, 
	/// <summary>Non-Visual Desktop Access is active.</summary>
		ID_NVDA, 
	/// <summary>A Dolphin product is active.</summary>
		ID_DOLPHIN,
		// Apple NSSpeechSynthesizer on Mac
		ID_MAC
};

/// <summary>
/// DolphinProduct enumeration determining which Dolphin product is currently active.
/// </summery>
enum DolphinProduct 
{
	/// <summary>No Dolphin product is active.</summary>
	ID_NOPRODUCT =0,
/// <summary>Hal is active.</summary>
		ID_HAL =1, 
	/// <summary>Lunar isactive.</summary>
		ID_LUNAR =2, 
	/// <summary>SuperNova is active.</summary>
		ID_SUPERNOVA =4, 
	/// <summary>LunarPlusis active.</summary>
		ID_LUNARPLUS =8, 
};

/// <summary>
/// DolphinReturnCode enumeration determining whether the procedure call succeeded and if it didn't what the specific error was.
/// </summery>
enum DolphinReturnCode 
{
	/// <summary>Function Succeeded.</summary>
	ID_SUCCESS= 0,
/// <summary>no dolphin product running.</summary>
		ID_No_Server =1, 
	/// <summary>An invalid id was specified.</summary>
		ID_INVALID_ID =2, 
	/// <summary>Invalid length.</summary>
		ID_INVALID_LENGTH =3, 
	/// <summary>Invalid action.</summary>
		ID_INVALID_ACTION =4, 
	/// <summary>In use by another application, try again.</summary>
		ID_ALREADY_IN_USE =5, 
	/// <summary>SuperNova control panel in use.</summary>
		ID_SUPERNOVA_CONTROL_PANEL_IN_USE =6, 
	/// <summary>SuperNova not responding.</summary>
ID_SUPERNOVA_NOT_RESPONDING =7, 
	/// <summary>Set val failed, parameter read-only.</summary>
		ID_SET_VAL_FAILED_PARAMETER_READONLY =8, 
};

//Action types enum.
enum ScreenReaderActionType 
{
ID_HOTKEY, //Window-Eyes hotkeys.
ID_REGISTERED_HOTKEY, //Window-Eyes hotkeys.
ID_CURSORKEY, //Window-Eyes cursor key.
ID_ACTION, //DolphinAction.
ID_DOLPHIN_SCRIPT, //Dolphin script.
ID_SCRIPT, //Jaws scripts.
ID_FUNCTION //jaws void functions.
};

//Hot spot clicker install state enum.
enum HSCInstallState 
{
	STATE_NOT_INSTALLED,
	STATE_OLDER_VERSION_INSTALLED,
	STATE_INSTALLED
};

// Variant-handling

		std::vector <JAWSParameter> AsJawsParameterVector( const boost::any  operand);
		int AsInt( const boost::any  operand);
std::wstring AsWString( const boost::any operand);
bool IsWString(const boost::any & operand);
bool IsInt(const boost::any & operand);
bool IsJawsParameterVector(const boost::any & operand);

#ifdef __WINDOWS__ 
#include "RBSpeechWin.h"
#endif  // if windows

#ifdef __WXOSX_COCOA__   
#include "RBSpeechCocoa.h"
#endif    // if Mac
#endif
