// ProtocolCollection.cpp

#include "ProtocolCollection.h"

ProtocolCollection::ProtocolCollection()
	{		};
	
bool ProtocolCollection::IsProtocolNameUnique(std::wstring wstrName)
		{
bool blnResult = true;

if (myProtocols.size() == 0)
{
	return blnResult;
}

std::map <std::string, SurfaceProtocol> ::iterator it;

for (it = myProtocols.begin(); it != myProtocols.end(); it++)
{
	if (it->second.GetProtocolName().compare( wstrName) == 0)
	{
		blnResult = false;
		break;
}  // end if
}  // end for

return blnResult;
	};
	

int ProtocolCollection::Add(SurfaceProtocol myNewProtocol)
	{
	 std::map <std::string, SurfaceProtocol> ::iterator it;
	 
	 it = myProtocols.find( myNewProtocol.GetProtocolID());
	if (it != myProtocols.end())
	{
		myProtocols.erase( it);
	}
	
	std::pair <std::string, SurfaceProtocol> myPair;
	myPair = std::make_pair( myNewProtocol.GetProtocolID(), myNewProtocol);

	myProtocols.insert( myPair);
		return myProtocols.size();
	}
	
void ProtocolCollection::Remove(std::string strProtocolID)
	{
		std::map <std::string, SurfaceProtocol> ::iterator it;
		it = myProtocols.find( strProtocolID);
	
	if (it != myProtocols.end())
	{
		myProtocols.erase( it);
	}
}
		
// Expose the start of the map for iteration
std::map <std::string, SurfaceProtocol>::iterator ProtocolCollection::begin()
	{
		return myProtocols.begin();		
	}
	
	// Expose the end of the map for error handling
std::map <std::string, SurfaceProtocol>::iterator ProtocolCollection::end()
	{
return myProtocols.end();
	}
	
	// Reports whether a given protocol name already exists in the map by returning its protocol ID
std::string ProtocolCollection::FindIDForProtocolName(std::wstring myProtocolName)
	{
		std::map <std::string, SurfaceProtocol>::const_iterator it;
				
		for (it = myProtocols.begin(); it != myProtocols.end(); it++)
		{
			if (it->second.GetProtocolName() == myProtocolName)
			{
				return it->second.GetProtocolID();
			}  // end if found
		}  // end for
	
	// if not found
		throw ID_NO_MATCHING_PROTOCOL_NAME;
	}

	// Indicates whether a specified protocol exists in the collection
bool ProtocolCollection::ProtocolExists(std::string strProtocolID)
		{
std::map <std::string, SurfaceProtocol> ::iterator it;
it = myProtocols.find( strProtocolID);

if (it != myProtocols.end()) 
	{
return true;
}
else
{
	return false;
}
}
	

	// Returns a protocol, based on its ID
SurfaceProtocol ProtocolCollection::GetProtocol(std::string strProtocolID)
	{
				std::map <std::string, SurfaceProtocol> ::iterator it;
it = myProtocols.find( strProtocolID);
if (it != myProtocols.end()) 
	{
		return it->second;
}
else
{
throw ID_NO_MATCHING_PROTOCOL_ID;
return it->second;
}
	}
		

// Returns a protocol, based on its location in the map
SurfaceProtocol ProtocolCollection::GetProtocol(int nIndex)
	{
		std::map <std::string, SurfaceProtocol>::iterator it;
int i = 0;		

for (it = myProtocols.begin(); it != myProtocols.end(); it++)
{
if (i == nIndex)
{
	return it->second;
}

i++;
}

throw ID_NO_MATCHING_PROTOCOL_ID;
return myProtocols.end()->second;
}
		

//Count.
int ProtocolCollection::count()
	{
		return myProtocols.size();
	}
	