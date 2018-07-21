// AppConfig.cpp
//Implementation of the AppConfig class


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
		std::map <std::string, MessageDefinition>::iterator it;
		it = HardwareControls.find( strControlHash);

		if (it != HardwareControls.end())
		{
			return it->second;
		}
		else
		{
throw RBException( wstrNoMessageDefinitionError);
			}
}


	void AppConfig::SetControlDefinition( std::pair <std::string, MessageDefinition> myPair)
	{
		/*
		std::map <std::string, MessageDefinition>::iterator it;
		it = HardwareControls.find( strControlHash);

		if (it != HardwareControls.end())
		{
			HardwareControls.erase( it);
		}
*/		

		HardwareControls.insert( myPair);
	}

	
MessageDefinition AppConfig::GetControlDefinitionItem( unsigned int nItem)
{
MessageDefinition myDefinition;
std::wstring wstrLabel;
wstrLabel.clear();
myDefinition.SetLabel( wstrLabel);

if ((nItem < HardwareControls.size())
		&& (HardwareControls.empty() == false))
	{
				unsigned int i = 0;
std::map <std::string, MessageDefinition>::iterator it;

for (it = HardwareControls.begin(); it != HardwareControls.end(); it++)
{
				if (i == nItem)
					{
						myDefinition = it->second;
				}

				i++;
}  // end for
	
if (myDefinition.GetLabel().empty())
{
throw RB_NO_CONTROL_DEFINITION;
}
else
{
	return myDefinition;
}
}  // end valid index and not empty map
	else
	{
		throw RB_NO_CONTROL_DEFINITION;
	}		
}

DisplayDefinition AppConfig::GetDisplayDefinition( std::string strDisplayHash)
	{
		std::map <std::string, DisplayDefinition>::iterator it;
		it = Displays.find( strDisplayHash);

		if (it != Displays.end())
		{
			return it->second;
		}
		else
		{
throw RB_NO_DISPLAY_DEFINITION;
		}
}


	void AppConfig::SetDisplayDefinition( std::pair <std::string, DisplayDefinition> myPair)
	{
		/*
		std::map <std::string, DisplayDefinition>::iterator it;
		it = Displays.find( strDisplayHash);

		if (it != Displays.end())
		{
			Displays.erase( it);
		}
*/		

		Displays.insert( myPair);
	}

	
	void AppConfig::UpdateDisplayDefinitions( std::map<std::string, DisplayDefinition> myNewDisplays)
	{
				std::map <std::string, DisplayDefinition>::iterator it, it2;
		
				for (it = myNewDisplays.begin(); it != myNewDisplays.end(); it++)				
				{
					it2 = Displays.find( it->first);

		if (it2 != Displays.end())
		{
						it2->second.SetDisplayLength( it->second.GetDisplayLength());
			it2->second.SetLineCount( it->second.GetLineCount());
			it2->second.SetCursorFromLeft( it->second.IsCursorFromLeft());
			it2->second.SetStripCount( it->second.GetStripCount());
			}  // end if found
	}  // end for
}

	
	std::string AppConfig::GetDisplayHash( std::wstring wstrLabel)
	{
std::string strHash;
strHash.clear();
std::map <std::string, DisplayDefinition>::iterator it;

for (it = Displays.begin(); it != Displays.end(); it++)
{
	if (it->second.GetLabel().compare( wstrLabel) == 0)
	{
		strHash = it->first;
	}
}  // end for

	return strHash;
	}


	DisplayDefinition AppConfig::GetDisplayDefinitionItem( unsigned int nItem)
{
DisplayDefinition myDefinition;
std::wstring wstrEmpty;
wstrEmpty.clear();
myDefinition.SetLabel( wstrEmpty);

if ((nItem < Displays.size())
		&& (Displays.empty() == false))
	{
				unsigned int i = 0;
std::map <std::string, DisplayDefinition>::iterator it;

for (it = Displays.begin(); it != Displays.end(); it++)
{
				if (i == nItem)
					{
						myDefinition = it->second;
				}

				i++;
}  // end for
	
if (myDefinition.GetLabel().empty())
{
throw RB_NO_DISPLAY_DEFINITION;
}
else
{
	return myDefinition;
}
}  // end valid index and not empty map
	else
	{
		throw RB_NO_DISPLAY_DEFINITION;
	}		
}


std::vector <std::wstring> AppConfig::GetDisplayNames()
{
std::map <std::string, DisplayDefinition>::iterator it;
std::vector <std::wstring> myDisplayNames;

for (it = Displays.begin(); it != Displays.end(); it++)
{
	myDisplayNames.push_back( it->second.GetLabel());
}

return myDisplayNames;
}


