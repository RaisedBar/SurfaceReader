// AppCollection.cpp
//Implementation of the AppCollection class

#define _HAS_STD_BYTE 0

#include "AppCollection.h"

using namespace std;
using namespace std::ranges::views;

AppCollection::AppCollection()
	{
		myAppConfigs.clear();
}

AppCollection::~AppCollection()
	{}

	void AppCollection::Add( std::wstring wstrAppExeName, std::wstring wstrProtocolID, AppConfig MyAppConfig)
{
		myAppConfigs.insert_or_assign({ wstrAppExeName, wstrProtocolID}, MyAppConfig);
	}

bool AppCollection::Add( std::wstring wstrAppExeName, std::wstring wstrProtocolID)
	{
	AppConfig MyAppConfig;
	myAppConfigs.insert_or_assign({ wstrAppExeName, wstrProtocolID }, MyAppConfig);
	return true;
		}


	void AppCollection::Remove( std::wstring wstrAppExeName, std::wstring wstrProtocolID, boost::uuids::uuid * myAppConfigID)
	{
		myAppConfigs.erase({ wstrAppExeName, wstrProtocolID});
	}

AppConfig AppCollection::GetAppConfig( std::pair <std::wstring, std::wstring> myID)
	{
	if (!IsAppConfigUnique(myID))
	{
		throw RBException(wstrAppConfigNotFoundError);
	}
	return myAppConfigs[myID];
	}

		AppConfig AppCollection::GetAppConfig( std::wstring wstrAppExeName, std::wstring wstrProtocolID)
	{
			return GetAppConfig({ wstrAppExeName, wstrProtocolID});
	}

	std::pair <std::wstring, std::wstring> AppCollection::FindAppConfig( std::wstring wstrAppName, std::wstring wstrProtocolID)
{
auto it  = myAppConfigs.find({ wstrAppName, wstrProtocolID});
if (it != myAppConfigs.end())
{
	return it->first;
	}
else
{
	return {L"", L""};
}
	}

	map <pair <wstring, wstring>, AppConfig>::iterator AppCollection::end()
	{
		return std::end(myAppConfigs);
	}

	vector <wstring> AppCollection::GetAppNames( wstring wstrProtocolID)
	{
		auto appConfigKeys = keys(myAppConfigs);
		auto filteredAppConfigs = appConfigKeys | filter([wstrProtocolID](const pair<wstring, wstring>& key) {return key.second.compare(wstrProtocolID) == 0; });
		vector <wstring> myAppNames;
		std::transform(begin(filteredAppConfigs), std::end(filteredAppConfigs), back_inserter(myAppNames), [](pair<wstring, wstring> val) { return val.first; });
			return myAppNames;
	}

AppConfig AppCollection::GetAppConfigItem( int nIndex)
{
int i = 0;

for (auto appConfig : myAppConfigs)
{
if (i == nIndex)
{
	return appConfig.second;
}
	 
i++;
}  // end for
	
throw ID_NO_MATCHING_APP_CONFIG;
}

int AppCollection::Count()
{
	return myAppConfigs.size();
}

void AppCollection::Clone( wstring wstrSourceProtocolID, wstring wstrDestinationProtocolID)
{
	auto range = myAppConfigs | filter([wstrSourceProtocolID](auto pairToCheck) {return pairToCheck.first.second.compare(wstrSourceProtocolID) == 0; });
	for(auto item : range)
	{
		Add(item.first.first, wstrDestinationProtocolID, item.second);
	}
	}

void AppCollection::UpdateDisplays( std::wstring wstrProtocolID, std::map <std::string , DisplayDefinition> myNewDisplays)
{
	auto range = myAppConfigs | filter([wstrProtocolID](auto pairToCheck) {return pairToCheck.first.second.compare(wstrProtocolID) == 0; });
	for(auto item : range)
	{
		item.second.UpdateDisplayDefinitions(myNewDisplays);
	}
	}

bool AppCollection::IsAppConfigUnique( std::pair <std::wstring, std::wstring> myAppConfigID)
{
// Does the specified configuration exist?
	return myAppConfigs.contains(myAppConfigID);
}