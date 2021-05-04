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
#include <vector>
#include <map>
#include <filesystem>
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
IniFileUtils(std::filesystem::path Filename)
{
	if (std::filesystem::exists(Filename))
	{
		IniTree.clear();
		boost::property_tree::ini_parser::read_ini(Filename.GetFullPath().ToStdString(), IniTree);
	}
}
//Load file after construction.
bool LoadFile(std::filesystem::path Filename)
{
	if (std::filesystem::exists(Filename))
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
bool ListSections(std::vector<std::wstring>* Sections)
{
	bool FunctionResult =false;
	if (!IniTree.empty())
	{
		std::vector<std::wstring> IniSections;
	IniSections.reserve(IniTree.size());
	BOOST_FOREACH(boost::property_tree::ptree::value_type &v, IniTree)
		IniSections.push_back(v.first);
FunctionResult =true;
	}
	return FunctionResult;
}
//does section exist.
bool DoesSectionExist(std::wstring section)
{
	bool FunctionResult =true;
	boost::property_tree::ptree::assoc_iterator it =IniTree.find(section.ToStdString());
	if (it !=IniTree.not_found())
FunctionResult =false;
	return FunctionResult;
}
//Retrieve all keys from a section.
bool RetrieveAllKeysFromSection(std::wstring SectionName, std::map<std::wstring, std::wstring>* SectionProperties)
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
HRESULT ReadStringValue(std::wstring Section, std::wstring Key, std::wstring* Value)
{
	HRESULT FunctionResult =S_FALSE;
		std::string WantedValue;
		std::wstring ExceptionMessage ="An exception has occured when trying to obtain a value. The exception text is:";
	WantedValue.Clear();
	ExitOnNull(Section, FunctionResult, S_FALSE, "You must pass in a section from which you want ot retrieve a value.");
		ExitOnNull(Key, FunctionResult, S_FALSE, "You must pass in a key who's value you wish to retrieve.");
	ExitOnNull(Value, FunctionResult, S_FALSE, "You must pass in a value varialbe in which to place the keys value.");
	try
	{ //see if an exception is thrown when we try and obtain a value.
		WantedValue = IniTree.get<std::string>(std::string(Section +"." + Key));
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