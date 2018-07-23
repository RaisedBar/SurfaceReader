// AppConfig.h
//AppConfig and related Classes


// #define _SCL_SECURE_NO_WARNINGS

#ifndef APPCONFIG_H
#define APPCONFIG_H

#pragma once

#include "Protocol.h"

//#include <utility>
//#include <fstream>
//#include <iostream>
//#include <vector>
//#include <map>

//#include "RBEnums.hpp"
//#include "RBException.hpp"

//#include <boost/uuid/uuid_generators.hpp>
//#include <boost/uuid/uuid.hpp>
//#include <boost/uuid/uuid_io.hpp>         // streaming operators etc. 
//#include <boost/archive/xml_woarchive.hpp>
//#include <boost/archive/xml_wiarchive.hpp>
//#include <boost/config.hpp>
//#include <boost/preprocessor/stringize.hpp>
//#include <boost/serialization/string.hpp>
//#include <boost/serialization/map.hpp>
//#include <boost/serialization/utility.hpp>
//#include <boost/serialization/split_member.hpp>
//#include <boost/serialization/tracking.hpp>
//#include <boost/serialization/base_object.hpp>
//#include <boost/serialization/nvp.hpp>
//#include <boost/serialization/export.hpp>

//#include <wx/arrstr.h> 
//#include <wx/msw/winundef.h> 


class AppConfig
{
public:
	AppConfig();
~AppConfig();
	
	// Setters and getters
	void SetAppName( std::wstring wstrName);
	std::wstring GetAppName() const;
void SetProtocolID( std::string strID);
	std::string GetProtocolID() const;
MessageDefinition GetControlDefinition( std::string strControlHash);
void SetControlDefinition( std::pair <std::string, MessageDefinition> myPair);
MessageDefinition GetControlDefinitionItem( unsigned int nItem);

std::map <std::string , MessageDefinition> GetHardwareControls()
	{
		return HardwareControls;
}

void SetHardwareControls( std::map <std::string , MessageDefinition> myNewHardwareControls)
{
HardwareControls = myNewHardwareControls;
}

DisplayDefinition GetDisplayDefinition( std::string strDisplayHash);
std::string GetDisplayHash( std::wstring wstrLabel);
void SetDisplayDefinition( std::pair <std::string, DisplayDefinition> myPair);
void UpdateDisplayDefinitions( std::map <std::string, DisplayDefinition> myNewDisplays);
DisplayDefinition GetDisplayDefinitionItem( unsigned int nItem);
std::vector <std::wstring> GetDisplayNames();

std::map <std::string , DisplayDefinition> GetDisplays()
	{
		return Displays;
}

void SetDisplays( std::map <std::string , DisplayDefinition> myNewDisplays)
{
Displays = myNewDisplays;
}

private:
friend std::ostream & operator<<( std::ostream &os, const AppConfig &AC);
friend std::istream & operator>>( std::istream &is, const AppConfig &AC);
friend class boost::serialization::access;
    
	template<class Archive>
void serialize( Archive & myArchive, const unsigned int version)
	{
try
	{
		const char * cAppName= "AppName";
myArchive & boost::serialization::make_nvp( cAppName, wstrAppName);
const char * cProtocolID = "ProtocolID";
myArchive & boost::serialization::make_nvp( cProtocolID, strProtocolID);
const char * cHardwareControls= "HardwareControls";
myArchive & boost::serialization::make_nvp( cHardwareControls, HardwareControls);
const char * cDisplays= "Displays";
myArchive & boost::serialization::make_nvp( cDisplays, Displays);
}  // end try
catch( ...)
{
throw ID_LOAD_DATA_EXCEPTION;
}  // end catch
}
	

// Internal storage
std::wstring wstrAppName;
std::string strProtocolID;
// This map stores the Hash of the message as an std::string key and a message definition
std::map <std::string , MessageDefinition> HardwareControls;
std::map <std::string , DisplayDefinition> Displays;
};
#endif

