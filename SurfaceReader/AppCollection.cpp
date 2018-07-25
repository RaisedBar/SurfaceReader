// AppCollection.cpp
//Implementation of the AppCollection class


#include "AppCollection.h"


AppCollection::AppCollection()
	{
		myAppConfigs.clear();
}


	AppCollection::~AppCollection()
	{}


	void AppCollection::Add( std::wstring wstrAppExeName, std::wstring wstrProtocolID, AppConfig MyAppConfig)
{
	std::pair <std::wstring, std::wstring> myIndexPair;
	std::pair <std::pair <std::wstring, std::wstring>, AppConfig> myAppConfigPair;
	
	myIndexPair = std::make_pair( wstrAppExeName, wstrProtocolID);
	myAppConfigPair = std::make_pair( myIndexPair, MyAppConfig);

	std::map <std::pair <std::wstring, std::wstring>, AppConfig>::iterator it; 

it = myAppConfigs.find( myIndexPair);

if (it != myAppConfigs.end())
{
	myAppConfigs.erase( it);	
	}

myAppConfigs.insert( myAppConfigPair);
}


bool AppCollection::Add( std::wstring wstrAppExeName, std::wstring wstrProtocolID)
	{
		// Is there an existing connection between this application and the specified protocol?
		std::pair <std::wstring, std::wstring> myAppProtocolPair;
		myAppProtocolPair = std::make_pair( wstrAppExeName, wstrProtocolID);
		std::map <std::pair <std::wstring, std::wstring>, AppConfig> ::iterator it; 
		it = myAppConfigs.find( myAppProtocolPair);
		
		if (it == myAppConfigs.end())
		{
myAppConfigs.erase( it);
	}
		
AppConfig myAppConfig;
			std::pair <std::pair <std::wstring, std::wstring>, AppConfig> myNewAppCollectionPair;
			myNewAppCollectionPair = std::make_pair( myAppProtocolPair, myAppConfig);
			myAppConfigs.insert( myNewAppCollectionPair);
			return true;
		}


	void AppCollection::Remove( std::wstring wstrAppExeName, std::wstring wstrProtocolID, boost::uuids::uuid * myAppConfigID)
	{
// Does the specified configuration exist?
		std::pair <std::wstring, std::wstring> myAppProtocolPair;
		myAppProtocolPair = std::make_pair( wstrAppExeName, wstrProtocolID);
		std::map <std::pair <std::wstring, std::wstring>, AppConfig> ::iterator it; 
		
		it = myAppConfigs.find( myAppProtocolPair);
		if (it != myAppConfigs.end())
		{
				// Delete the map entry
				myAppConfigs.erase( it);		
			}  // end if appName/protocol found
		}


AppConfig AppCollection::GetAppConfig( std::pair <std::wstring, std::wstring> myID)
	{
		  std::map <std::pair <std::wstring, std::wstring>, AppConfig> ::iterator it; 
		
		it = myAppConfigs.find( myID);
		if (it != myAppConfigs.end())
		{
				return it->second;
				}  
			else
			{
    throw RBException(wstrAppConfigNotFoundError);
}  
			}


		AppConfig AppCollection::GetAppConfig( std::wstring wstrAppExeName, std::wstring wstrProtocolID)
	{
		  // Does the specified configuration exist?
		std::pair <std::wstring, std::wstring> myAppProtocolPair;
		myAppProtocolPair = std::make_pair( wstrAppExeName, wstrProtocolID);
		std::map <std::pair <std::wstring, std::wstring>, AppConfig> ::iterator it; 
		
		it = myAppConfigs.find( myAppProtocolPair);
		if (it != myAppConfigs.end())
		{
				return it->second;
				}  
			else
			{
    throw RBException(wstrAppConfigNotFoundError);
}  
			}


	std::pair <std::wstring, std::wstring> AppCollection::FindAppConfig( std::wstring wstrAppName, std::wstring wstrProtocolID)
{
	std::pair <std::wstring, std::wstring> myAppPair;
myAppPair = std::make_pair( wstrAppName, wstrProtocolID);

std::map <std::pair <std::wstring, std::wstring>, AppConfig> ::iterator it; 

it = myAppConfigs.find( myAppPair);
if (it != myAppConfigs.end())
{
	return it->first;
	}
else
{
std::wstring wstrEmpty;
wstrEmpty.clear();
myAppPair = std::make_pair( wstrEmpty, wstrEmpty);
	return myAppPair;
}
	}


	std::map <std::pair <std::wstring, std::wstring>, AppConfig>::iterator AppCollection::end()
	{
		return myAppConfigs.end();
	}


	std::vector <std::wstring> AppCollection::GetAppNames( std::wstring wstrProtocolID)
	{
std::map <std::pair <std::wstring, std::wstring>, AppConfig>::iterator it;
std::vector <std::wstring> myAppNames;

for (it = myAppConfigs.begin(); it != myAppConfigs.end(); it++)
{
	if (it->first.second.compare( wstrProtocolID) == 0)
	{
		myAppNames.push_back( it->first.first);
	}
}
			return myAppNames;
	}


AppConfig AppCollection::GetAppConfigItem( int nIndex)
{
std::map <std::pair <std::wstring, std::wstring>, AppConfig>::iterator it;
int i = 0;

for (it = myAppConfigs.begin(); it != myAppConfigs.end(); it++)
{
if (i == nIndex)
{
	return it->second;
}
	 
i++;
}  // end for
	
throw ID_NO_MATCHING_APP_CONFIG;
return it->second;
}


int AppCollection::Count()
{
	return myAppConfigs.size();
}


void AppCollection::Clone( std::wstring wstrSourceProtocolID, std::wstring wstrDestinationProtocolID)
{
std::map <std::pair <std::wstring, std::wstring>, AppConfig>::iterator it;

for (it = myAppConfigs.begin(); it != myAppConfigs.end(); it++)
{
	// Check to see if this entry is for our original protocol
	if (it->first.second.compare( wstrSourceProtocolID) == 0)
	{
		Add( it->first.first, wstrDestinationProtocolID, it->second);
}  // end if match found
}  // end for
}


void AppCollection::UpdateDisplays( std::wstring wstrProtocolID, std::map <std::wstring , DisplayDefinition> myNewDisplays)
{
std::map <std::pair <std::wstring, std::wstring>, AppConfig>::iterator it;

for (it = myAppConfigs.begin(); it != myAppConfigs.end(); it++)
{
if (it->first.second.compare( wstrProtocolID) == 0)
	{
		std::wstring wstrAppName = it->first.first;
		std::wstring wstrNewProtocolID = it->first.second;
		AppConfig myAppConfig = it->second;
		myAppConfig.UpdateDisplayDefinitions( myNewDisplays);
		it->second = myAppConfig;
}  // End if matching protocol ID
}  // end for
}

bool AppCollection::IsAppConfigUnique( std::pair <std::wstring, std::wstring> myAppConfigID)
{
// Does the specified configuration exist?
		std::map <std::pair <std::wstring, std::wstring>, AppConfig> ::iterator it; 
		
		it = myAppConfigs.find( myAppConfigID);
		if (it != myAppConfigs.end())
		{
				return false;
				}  
			else
			{
    	return true;
		}
}


