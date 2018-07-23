// AppCollection.h

#ifndef APP_COLLECTION_H
#define APP_COLLECTION_H

#pragma once

//#include <map>
//#include <vector>

#include "AppConfig.h"
//#include "RBException.h"

//#include <wx/arrstr.h> 
//#include <wx/msw/winundef.h> 


//AppCollection class, holding AppConfigs.
class AppCollection
{
public:
AppCollection();
~AppCollection();

std::pair <std::wstring, std::wstring> FindAppConfig( std::wstring wstrAppName, std::wstring wstrProtocolID);
std::map <std::pair <std::wstring, std::wstring>, AppConfig> ::iterator end(); 
bool Add( std::wstring wstrAppExeName, std::wstring wstrProtocolID);
void Add( std::wstring wstrAppExeName, std::wstring wstrProtocolID, AppConfig MyAppConfig);
void Remove( std::wstring wstrAppExeName, std::wstring wstrProtocolID, boost::uuids::uuid * myAppConfigID);
void Clone( std::wstring wstrSourceProtocolID, std::wstring wstrDestinationProtocolID);

std::vector <std::wstring> GetAppNames( std::wstring wstrProtocolID);

AppConfig GetAppConfig( std::wstring wstrAppExeName, std::wstring wstrProtocolID);
AppConfig GetAppConfig( std::pair <std::wstring, std::wstring> myID);
AppConfig GetAppConfigItem( int nIndex);
int Count();
bool IsAppConfigUnique( std::pair <std::wstring, std::wstring> myAppConfigID);
void UpdateDisplays( std::wstring wstrProtocolID, std::map <std::string , DisplayDefinition> myNewDisplays);

private:

friend std::ostream & operator<<( std::ostream &os, const AppCollection &AC);
friend std::istream & operator>>( std::istream &is, const AppCollection &AC);
friend class boost::serialization::access;
    
	template<class Archive>
void serialize( Archive & myArchive, const unsigned int version)
	{
const char * cAppCollectionTag = "ApplicationMappings";
myArchive & boost::serialization::make_nvp( cAppCollectionTag, myAppConfigs);
}
	

// Internal storage
// This map uses a pair of the application exe name and a string version of a Protocol UID as a composite key to reference an application configuration 
std::map <std::pair <std::wstring, std::wstring>, AppConfig> myAppConfigs;
};

#endif