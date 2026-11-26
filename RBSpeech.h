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
#include <any>
#include <memory>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/ini_parser.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/optional/optional.hpp>
#include <boost/dll/shared_library.hpp>
#include <boost/dll/import.hpp>


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
/// <remarks> this type contains an integer on the left corresponding to the integer in the AvailableActionFieldsType, and a std::any on the right allowing the storing of any data type supported. This doesn't tie us to returning strings where they are not the most efficient way to represent the data. </remarks> 
typedef std::map<int, std::any> ActionInfoType; 
typedef std::map<int, std::any>::iterator ActionInfoTypeIterator;
/// <summary> A vector of <see cref="ActionInfoType"/> containing all available actions for the currently active access technology. </summary>
typedef std::vector<ActionInfoType > ActionCollectionType; 
typedef std::vector<ActionInfoType >::iterator ActionCollectionTypeIterator; 
typedef std::map < std::pair< std::wstring, std::string>, ActionCollectionType> JawsCacheType;

// <summary> the available action fields. </summary>
/// <remarks> These fields are unique to the access technology that is currently active. They define the fields available in the <see cref="ActionInfoType"/>
/// Further documentation can be found in the respective Get*Actions methods. </remarks>
typedef std::map<std::wstring, int> AvailableActionFieldsType;
typedef std::map<std::wstring, int>::iterator AvailableActionFieldsIterator;
typedef std::pair <AvailableActionFieldsType, ActionCollectionType > AvailableActionsType; //Available actions pair.

/// <summary> 
///ActiveProduct enumeration determining which supported product is currently active.
/// </summery>
enum ActiveProduct 
{
	/// <summary>No access technology is active.</summary>
	ID_NONE = 0,
	/// <summary>JAWS for windows is active.</summary>
	ID_JAWS = 1,
	/// <summary>System Access is active.</summary>
	ID_SYSTEMACCESS = 2,
	/// <summary>Non-Visual Desktop Access is active.</summary>
	ID_NVDA = 3,
	// 4 was Dolphin, which is no longer supported. The values are stored in protocol files, so the
	// others keep their numbers; MessageDefinition drops any actions still stored for 4 when it loads.
	// Apple NSSpeechSynthesizer on Mac
	ID_MAC = 5,
	/// <summary>Windows Narrator is active; it is spoken to through UI Automation notifications.</summary>
	ID_NARRATOR = 6
};

/// <summary>The products that have a branch of their own in the action trees, in display order.
/// Narrator is left out until actions can be defined for it.</summary>
const ActiveProduct arrActionProducts[] = { ID_JAWS, ID_SYSTEMACCESS, ID_NVDA, ID_MAC };


//Action types enum.
enum ScreenReaderActionType 
{
	// Stored in protocol files, so the values are fixed; 3 and 4 were Dolphin actions and scripts
	ID_HOTKEY = 0, //Window-Eyes hotkeys.
	ID_REGISTERED_HOTKEY = 1, //Window-Eyes hotkeys.
	ID_CURSORKEY = 2, //Window-Eyes cursor key.
	ID_SCRIPT = 5, //Jaws scripts.
	ID_FUNCTION = 6 //jaws void functions.
};

//Hot spot clicker install state enum.
enum HSCInstallState 
{
	STATE_NOT_INSTALLED,
	STATE_OLDER_VERSION_INSTALLED,
	STATE_INSTALLED
};

// Variant-handling

		std::vector <JAWSParameter> AsJawsParameterVector( const std::any  operand);
		int AsInt( const std::any  operand);
std::wstring AsWString( const std::any operand);
bool IsWString(const std::any & operand);
bool IsInt(const std::any & operand);
bool IsJawsParameterVector(const std::any & operand);

#ifdef __WINDOWS__ 
#include "RBSpeechWin.h"
#endif  // if windows

#ifdef __WXOSX_COCOA__   
#include "RBSpeechCocoa.h"
#endif    // if Mac
#endif
