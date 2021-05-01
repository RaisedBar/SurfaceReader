// ParseIniFile.h

#ifndef PARSEINIFILE_H
#define PARSEINIFILE_H
//parse ini file. allows us to parse both ConfigNames.ini and any hsc file.
	//includes:
#pragma once
#include <stdlib.h>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/ini_parser.hpp>
#include <boost/property_tree/exceptions.hpp>
#include <wx/filename.h>
#include <wx/string.h>
#include <vector>
#include <map>
#include "WIX Include.h"
#include <wx/msw/winundef.h> 


class IniFileUtils {
private:
	boost::property_tree::ptree IniTree;
public:
IniFileUtils()
{
	IniTree.clear();
}
IniFileUtils(wxFileName Filename)
{
	if (Filename.FileExists())
	{
		IniTree.clear();
		boost::property_tree::ini_parser::read_ini(Filename.GetFullPath().ToStdString(), IniTree);
	}
}
//Load file after construction.
bool LoadFile(wxFileName Filename)
{
	if (Filename.FileExists())
	{
		IniTree.clear();
		boost::property_tree::ini_parser::read_ini(Filename.GetFullPath().ToStdString(), IniTree);
		if (IniTree.empty())
		return false;
		else
			return true;
	}
}

//list sections.
bool ListSections(std::vector<wxString>* Sections)
{
	bool FunctionResult =false;
	if (!IniTree.empty())
	{
		std::vector<wxString> IniSections;
	IniSections.reserve(IniTree.size());
	BOOST_FOREACH(boost::property_tree::ptree::value_type &v, IniTree)
		IniSections.push_back(v.first);
FunctionResult =true;
	}
	return FunctionResult;
}
//does section exist.
bool DoesSectionExist(wxString section)
{
	bool FunctionResult =true;
	boost::property_tree::ptree::assoc_iterator it =IniTree.find(section.ToStdString());
	if (it !=IniTree.not_found())
FunctionResult =false;
	return FunctionResult;
}
//Retrieve all keys from a section.
bool RetrieveAllKeysFromSection(wxString SectionName, std::map<wxString, wxString>* SectionProperties)
{
bool FunctionResult =false;
if (!SectionName.empty())
{ //there is something in the section name.
if (DoesSectionExist(SectionName))
{ //the section exists.
	//edit.
}
} //end section name checking.
	return FunctionResult; 
}
//Read a string.
HRESULT ReadStringValue(wxString Section, wxString Key, wxString* Value)
{
	HRESULT FunctionResult =S_FALSE;
		wxString WantedValue;
		wxString ExceptionMessage ="An exception has occured when trying to obtain a value. The exception text is:";
	WantedValue.Clear();
	ExitOnNull(Section, FunctionResult, S_FALSE, "You must pass in a section from which you want ot retrieve a value.");
		ExitOnNull(Key, FunctionResult, S_FALSE, "You must pass in a key who's value you wish to retrieve.");
	ExitOnNull(Value, FunctionResult, S_FALSE, "You must pass in a value varialbe in which to place the keys value.");
	try
	{ //see if an exception is thrown when we try and obtain a value.
		WantedValue =IniTree.get<std::string>(wxString::Format("%s.%s", Section, Key).ToStdString());
		Value =&WantedValue; //return in a pointer.
	} //end try block.
	catch(boost::property_tree::ptree_error &e)
	{ //an exception was thrown.
		ExceptionMessage.append(e.what());
ExitFunctionWithMessageAndStatusCode(FunctionResult, S_FALSE, ExceptionMessage); //exit the function as an exception has occured.
	} //End exception catching.
LExit:
	return FunctionResult;
}
};
#endif