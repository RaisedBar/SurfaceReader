// AppConfig.cpp
//Implementation of the AppConfig class

#define _HAS_STD_BYTE 0

#include "AppConfig.h"
	
AppConfig::AppConfig()
	{
		wstrAppName.clear();
		strProtocolID.clear();
HardwareControls.clear();
}

	AppConfig::~AppConfig()
	{}

	std::wstring AppConfig::GetAppName() const
	{
		return wstrAppName;
	}

	void AppConfig::SetAppName( std::wstring wstrName)
	{	
		wstrAppName = wstrName;
	}

std::string AppConfig::GetProtocolID() const
	{
		return strProtocolID;
	}

	void AppConfig::SetProtocolID( std::string strID)
	{	
		strProtocolID = strID;
	}

		MessageDefinition AppConfig::GetControlDefinition( std::string strControlHash)
	{
			if (!HardwareControls.contains(strControlHash))
			{
				throw RBException(wstrNoMessageDefinitionError);
			}
			return HardwareControls[strControlHash];
			}

	void AppConfig::SetControlDefinition( std::pair <std::string, MessageDefinition> myPair)
	{
		HardwareControls.insert_or_assign(myPair.first, myPair.second);
	}
	
MessageDefinition AppConfig::GetControlDefinitionItem( unsigned int nItem)
{
	if ((HardwareControls.empty()) && (nItem == HardwareControls.size()))
	{
		throw RB_NO_CONTROL_DEFINITION;
	}

std::optional<MessageDefinition> myDefinition;
unsigned int i = 0;

	for (auto hardwareControl : HardwareControls)
	{
		if (i == nItem)
		{
			myDefinition = hardwareControl.second;
			break;
		}
		i++;
	}
	
if (!myDefinition)
{
throw RB_NO_CONTROL_DEFINITION;
}

	return *myDefinition;
}

DisplayDefinition AppConfig::GetDisplayDefinition( std::string strDisplayHash)
	{
	if (!Displays.contains(strDisplayHash))
	{
		throw RB_NO_DISPLAY_DEFINITION;
	}
	return Displays[strDisplayHash];
}

	void AppConfig::SetDisplayDefinition( std::pair <std::string, DisplayDefinition> myPair)
	{
		Displays.insert_or_assign(myPair.first, myPair.second);
	}

	void AppConfig::UpdateDisplayDefinitions( std::map<std::string, DisplayDefinition> myNewDisplays)
	{
		for(auto newDisplay : myNewDisplays)
				{
					auto originalDisplay = Displays.find(newDisplay.first);

		if (originalDisplay != Displays.end())
		{
			originalDisplay->second.SetDisplayLength( newDisplay.second.GetDisplayLength());
			originalDisplay->second.SetLineCount( newDisplay.second.GetLineCount());
			originalDisplay->second.SetCursorFromLeft( newDisplay.second.IsCursorFromLeft());
			originalDisplay->second.SetStripCount(newDisplay.second.GetStripCount());
			}  // end if found
	}  // end for
}

	std::string AppConfig::GetDisplayHash( std::wstring wstrLabel)
	{
		std::string strHash = strEmpty;
		 auto filteredDisplays = Displays | std::ranges::views::filter([wstrLabel](std::pair<const std::string, DisplayDefinition> item) {return item.second.GetLabel().compare(wstrLabel) == 0; });
		 strHash = std::begin(filteredDisplays)->first;
	return strHash;
	}

	DisplayDefinition AppConfig::GetDisplayDefinitionItem( unsigned int nItem)
{
		if ((Displays.empty()) && (nItem == Displays.size()))
		{
			throw RB_NO_DISPLAY_DEFINITION;
		}

	std::optional<DisplayDefinition> myDefinition;
				unsigned int i = 0;
				
	for(auto display : Displays)
{
				if (i == nItem)
					{
						myDefinition = display.second;
						break;
					}

				i++;
}  // end for

	if (!myDefinition)
	{
		throw RB_NO_DISPLAY_DEFINITION;
	}

	return *myDefinition;
}

std::vector <std::wstring> AppConfig::GetDisplayNames()
{
std::vector<std::wstring> myDisplayNames;
	std::transform(std::begin(Displays), std::end(Displays), std::back_inserter(myDisplayNames), [](std::pair<const std::string, DisplayDefinition> item)-> std::wstring {return item.second.GetLabel(); });
return myDisplayNames;
}