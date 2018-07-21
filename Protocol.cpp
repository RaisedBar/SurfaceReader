// Protocol.cpp
//Implementation of the SurfaceProtocol Class.

#include "Protocol.h"


SurfaceProtocol::SurfaceProtocol()
{
	// Generate a new Protocol ID 
	boost::uuids::uuid myUUID = 	boost::uuids::random_generator()();
	strProtocolID = boost::lexical_cast <std::string> (myUUID); 
	strProtocolName.clear();
blnIsEncrypted = false;
		
		vSysExManufacturerID.reserve( 0);
vSysExManufacturerID.resize( 0);

nSysExModelID = -1;
nSysExDeviceID = -1;

blnUseGlobalStates = true;

// Message buffering
nMsgBufferSize = 1;
blnDropExtraStatusBytes = false;
}


	SurfaceProtocol::~SurfaceProtocol()
	{}


	SurfaceProtocol 	SurfaceProtocol::Clone( std::wstring wstrNewName)
{
SurfaceProtocol myProtocol = *this;
myProtocol.SetProtocolName( wstrNewName);
// Generate a new ID
	boost::uuids::uuid myUUID = 	boost::uuids::random_generator()();
	myProtocol.SetProtocolID( boost::lexical_cast <std::string> (myUUID));
return myProtocol;
}

	
// Setters and getters
	
std::string SurfaceProtocol::GetProtocolID() const
	{
return strProtocolID;
	}


	std::wstring SurfaceProtocol::GetProtocolName() const
{
	return strProtocolName;
}


void SurfaceProtocol::SetProtocolName( std::wstring strPName)
{
	strProtocolName = strPName;
}


std::vector <long> SurfaceProtocol::GetSysExManufacturerID() const
{
return vSysExManufacturerID;
}


void SurfaceProtocol::SetSysExManufacturerID( std::vector <long> vID)
{
int nSize = vID.size();
	vSysExManufacturerID.reserve( nSize);
	vSysExManufacturerID.resize( nSize);
vSysExManufacturerID = vID;
}


int SurfaceProtocol::GetSysExModelID() const
{
return nSysExModelID;
}


void SurfaceProtocol::SetSysExModelID( int nID)
{
nSysExModelID = nID;
}


int SurfaceProtocol::GetSysExDeviceID() const
{
	return nSysExDeviceID;
}


void SurfaceProtocol::SetSysExDeviceID( int nDeviceID)
{
	nSysExDeviceID = nDeviceID;
}


	DisplayDefinition SurfaceProtocol::GetDisplay( std::string strHash)
	{
std::map <std::string, DisplayDefinition> ::iterator it;
		it = DisplayMessages.find( strHash);

		if (it != DisplayMessages.end())
		{
			return it->second;
			}
		else
		{
			// Error:
					throw RB_NO_DISPLAY_DEFINITION;
		}
	}


void SurfaceProtocol::SetDisplay( std::string strHash, DisplayDefinition myDisplayDefinition)
{
std::map <std::string, DisplayDefinition>::iterator it;
it = DisplayMessages.find( strHash);

if (it != DisplayMessages.end())
{
	std::wstring wstrMessage = it->second.GetLabel();
	wstrMessage.append( wstrMIDIOverwriteCheck);
		
	if (		wxMessageBox( wstrMessage,                wstrAppTitle, wxYES_NO | wxNO_DEFAULT | wxICON_QUESTION) == wxYES) 
{
	DisplayMessages.erase( it);
	}
}

std::pair <std::string, DisplayDefinition> myPair;
myPair = std::make_pair( strHash, myDisplayDefinition);
DisplayMessages.insert( myPair);
}


int SurfaceProtocol::GetNibbleCursorPos( std::string strHash)
{
	int nResult = 0;
	
	for (unsigned int i = 0; i < strHash.length(); i++)
		{
			if (strHash.substr( i, 1).compare( HASH_DELIMITER) == 0)
			{
				nResult++;
			}
	}  // end for

	return nResult;
}


std::pair <DisplayDefinition, std::string> SurfaceProtocol::GetNibbleDisplay( std::string strDisplayHash)
	{
std::map <std::string, DisplayDefinition> ::iterator it;

for (it = DisplayMessages.begin(); it != DisplayMessages.end(); it++)
{
	std::string strKey = it->first; 

	int nValuePos = strKey.find( NIBBLE_WILD_CARD_MARKER, 0);
int nFirstRangePos = strKey.find( NIBBLE_RANGE_MARKER, 0);
bool blnRange = false;
bool blnValue = false;

if ((nValuePos == wxNOT_FOUND) && (nFirstRangePos == wxNOT_FOUND))
{
	strKey = it->first;	
}
else   // search for the shortest substring
{
	if (nValuePos != wxNOT_FOUND)
	{
blnValue = true;

strKey = strKey.substr( 0, nValuePos);
}
	else if (nFirstRangePos != wxNOT_FOUND)
	{
blnRange = true;
strKey = strKey.substr( 0, nFirstRangePos);
}
	else if (nValuePos < nFirstRangePos)
	{
		blnValue = true;
		strKey = strKey.substr( 0, nValuePos);
	}
	else
	{
blnRange = true;
strKey = strKey.substr( 0, (nFirstRangePos -1));
	}
}

if (strDisplayHash.substr( 0, strKey.length()).compare( strKey) == 0)  // Possible match
{
if (blnRange)
{
	std::string strTest = it->first;
	int nEnd = strDisplayHash.find( HASH_DELIMITER, nFirstRangePos);
std::string strValue = strDisplayHash.substr( nFirstRangePos, (nEnd - nFirstRangePos));
	
int nMinPos = nFirstRangePos +2;
nEnd = it->first.find( HASH_DELIMITER, nMinPos);
std::string strMin = it->first.substr( nMinPos, (nEnd - nMinPos));
	
int nMaxPos = nEnd +1;
nEnd = it->first.find( HASH_DELIMITER, (nMaxPos));
std::string strMax = it->first.substr( nMaxPos, (nEnd - nMinPos) -1);
nEnd = strMax.find( HASH_DELIMITER, 0);

if (nEnd > 0)
{
	strMax = strMax.substr( 0, nEnd);
}

	long lValue, lMin, lMax;

	try
		{
lValue = boost::lexical_cast <long> (strValue);
}
catch(bad_lexical_cast &)
{
std::string strVariable = "strValue = ";
	wxMessageBox( strVariable.append( strValue), wstrErrorTitle, wxOK | wxICON_ERROR);
}

try
	{
		lMin = boost::lexical_cast <long> (strMin);
}
catch(bad_lexical_cast &)
{
std::string strVariable = "strMin = ";
	wxMessageBox( strVariable.append( strMin), wstrErrorTitle, wxOK | wxICON_ERROR);
}

try
	{
		lMax = boost::lexical_cast <long> (strMax);
}
catch(bad_lexical_cast &)
{
std::string strVariable = "strMax = ";
		wxMessageBox( strVariable.append( strMax), wstrErrorTitle, wxOK | wxICON_ERROR);
}

unsigned char nValue = (unsigned char) lValue;
unsigned char nMin = (unsigned char) lMin;
unsigned char nMax = (unsigned char) lMax;

if ((nValue >= nMin) && (nValue <= nMax))
{
	std::pair <DisplayDefinition, std::string> myPair;
myPair = std::make_pair( it->second, it->first);
	return myPair;	
}	
}

else if (blnValue)
{
std::string strRemainder = it->first;
unsigned int nStartPos = nValuePos +2;
unsigned int nLength = (strRemainder.length() - nValuePos) -2;
strRemainder = strRemainder.substr( nStartPos, nLength);
std::string strKey2 = strDisplayHash.substr( nStartPos, nLength);

if ( strKey2.compare( strRemainder) == 0)
{
std::pair <DisplayDefinition, std::string> myPair;
myPair = std::make_pair( it->second, it->first);
	return myPair;	
}  // Remainder matches the rest of the incoming message after skipping the value wild card
}  // end else if blnValue
}  // end if substring match
}  // end for

			// Error:
					throw RB_NO_DISPLAY_DEFINITION;
}


std::vector <std::wstring> SurfaceProtocol::GetDisplayNames()
{
std::map <std::string, DisplayDefinition>::iterator it;
std::vector <std::wstring> myDisplayNames;

for (it = DisplayMessages.begin(); it != DisplayMessages.end(); it++)
{
	myDisplayNames.push_back( it->second.GetLabel());
}

return myDisplayNames;
}


DisplayDefinition SurfaceProtocol::GetDisplayItem( unsigned int nItem)
{
	if ((nItem < DisplayMessages.size())
		&& (DisplayMessages.empty() == false))
	{
DisplayDefinition myDefinition;
		unsigned int i = 0;
bool blnFound = false;
std::map <std::string, DisplayDefinition>::iterator it;

for (it = DisplayMessages.begin(); it != DisplayMessages.end(); it++)
{
				if (i == nItem)
					{
blnFound = true;
myDefinition = it->second;
break;
				}
				else
				{
					i++;
				}
}  // end for
	
if (blnFound)
{
	return myDefinition;
}
else
{
throw RB_NO_DISPLAY_DEFINITION;
}
	}  // end valid index and not empty map
	else
	{
		throw RB_NO_DISPLAY_DEFINITION;
	}		
}


std::string SurfaceProtocol::GetDisplayHash( std::wstring wstrName)
{
	std::map <std::string, DisplayDefinition> ::iterator it;
std::string strResult;
strResult.clear();

		for (it = DisplayMessages.begin(); it != DisplayMessages.end(); it++)
{
	if (it->second.GetLabel().compare( wstrName) == 0)
				{
			strResult = it->first;
}  // end if found in map
		}  // end for
		
return strResult;
	}


bool SurfaceProtocol::DeleteDisplay( std::string strHash)
{
	std::map <std::string, DisplayDefinition>::iterator it;
	it = DisplayMessages.find( strHash);

	if (it != DisplayMessages.end())
{
	DisplayMessages.erase( it);
return true;
}
else
{
return false;
}
}


unsigned int SurfaceProtocol::GetDisplayCount() const
{
	return DisplayMessages.size();
}


bool SurfaceProtocol::RenameDisplay( std::wstring wstrOldName, std::wstring wstrNewName)
{
bool blnResult = false;
std::map <std::string, DisplayDefinition> ::iterator it;

for (it = DisplayMessages.begin(); it != DisplayMessages.end(); it++)
{
	if (it->second.GetLabel().compare( wstrOldName) == 0)
{
	std::pair <std::string, DisplayDefinition> myPair;
	myPair = std::make_pair( it->first, it->second);
	myPair.second.SetLabel( wstrNewName);
	DisplayMessages.erase( it);
	DisplayMessages.insert( myPair);
	blnResult = true;
break;
	}
}  // end for
	
return blnResult;
}


MessageDefinition SurfaceProtocol::GetHardwareControl( std::string strHash)
	{
std::map <std::string, MessageDefinition> ::iterator it;
it = HardwareMessages.find( strHash);

		if (it != HardwareMessages.end())
		{
			return it->second;
			}
		else
		{
			// Error:
					throw RB_NO_CONTROL_DEFINITION;
		}		
}


bool SurfaceProtocol::SetHardwareControl( std::string strHash, MessageDefinition myControlDefinition)
{
std::map <std::string, MessageDefinition>::iterator it;
it = HardwareMessages.find( strHash);

if (it != HardwareMessages.end())
{
	std::wstring wstrMessage = it->second.GetLabel();
	wstrMessage.append( wstrMIDIOverwriteCheck);
		
	if (		wxMessageBox( wstrMessage,                wstrAppTitle, wxYES_NO | wxNO_DEFAULT | wxICON_QUESTION) == wxYES) 
{
	HardwareMessages.erase( it);
		}
	else
{
	return false;
	}
}

std::pair <std::string, MessageDefinition> myPair;
myPair = std::make_pair( strHash, myControlDefinition);
HardwareMessages.insert( myPair);
return true;
}


std::vector <std::wstring> SurfaceProtocol::GetHardwareControlNames()
	{
std::map <std::string, MessageDefinition>::iterator it;
std::vector <std::wstring> myControlNames;

for (it = HardwareMessages.begin(); it != HardwareMessages.end(); it++)
{
	myControlNames.push_back( it->second.GetLabel());
}

return myControlNames;
}


std::pair <std::string, MessageDefinition> SurfaceProtocol::GetHardwareControlItem( unsigned int nItem)
{
bool blnFound = false;
std::pair <std::string, MessageDefinition> myPair;

	if ((nItem < HardwareMessages.size())
		&& (HardwareMessages.empty() == false))
	{
				unsigned int i = 0;
std::map <std::string, MessageDefinition>::iterator it;

for (it = HardwareMessages.begin(); it != HardwareMessages.end(); it++)
{
				if (i == nItem)
					{
												myPair = std::make_pair( it->first, it->second);
blnFound = true;				
				break;
				}
				else
				{
					i++;
				}
}  // end for
	
	if (blnFound)
	{
return myPair;
	}
	else
	{
throw RB_NO_CONTROL_DEFINITION;
return myPair;
	}
	}  // end valid index and not empty map
	else
	{
		throw RB_NO_CONTROL_DEFINITION;
		return myPair;
	}		
}


MessageDefinition SurfaceProtocol::GetHardwareControlDefinitionItem( unsigned int nItem)
{
	MessageDefinition myDefinition;
bool blnFound = false;

	if ((nItem < HardwareMessages.size())
		&& (HardwareMessages.empty() == false))
	{
				unsigned int i = 0;
std::map <std::string, MessageDefinition>::iterator it;

for (it = HardwareMessages.begin(); it != HardwareMessages.end(); it++)
{
				if (i == nItem)
					{
						myDefinition = it->second;
blnFound = true;				
				}
				else
				{
					i++;
				}
}  // end for
	
	if (blnFound)
	{
return myDefinition;
	}
	else
	{
throw RB_NO_CONTROL_DEFINITION;
return myDefinition;
	}
	}  // end valid index and not empty map
	else
	{
		throw RB_NO_CONTROL_DEFINITION;
		return myDefinition;
	}		
}


std::string SurfaceProtocol::GetHardwareControlHashItem( unsigned int nItem)
{
	std::string strResult;
	strResult.clear();

	if ((nItem < HardwareMessages.size())
		&& (HardwareMessages.empty() == false))
	{
unsigned int i = 0;
std::map <std::string, MessageDefinition>::iterator it;

for (it = HardwareMessages.begin(); it != HardwareMessages.end(); it++)
{
				if (i = nItem)
					{
												strResult = it->first;
				}
				else
				{
					i++;
				}
}  // end for
	}  // end valid index and not empty map
return strResult;	
	}


std::string SurfaceProtocol::GetHardwareControlHash( std::wstring wstrName)
{
	std::map <std::string, MessageDefinition> ::iterator it;
std::string strResult;
strResult.clear();

		for (it = HardwareMessages.begin(); it != HardwareMessages.end(); it++)
{
	if (it->second.GetLabel().compare( wstrName) == 0)
		{
			strResult = it->first;
}  // end if found in map
		}  // end for
		
return strResult;
	}


bool SurfaceProtocol::DeleteHardwareControl( std::string strHash)
	{
	std::map <std::string, MessageDefinition>::iterator it;
it = HardwareMessages.find( strHash);

if (it != HardwareMessages.end())
{
	HardwareMessages.erase( it);
return true;
}
else
{
return false;
}
}


bool SurfaceProtocol::RenameHardwareControl( std::wstring wstrOldName, std::wstring wstrNewName)
{
bool blnResult = false;
std::map <std::string, MessageDefinition> ::iterator it;

for (it = HardwareMessages.begin(); it != HardwareMessages.end(); it++)
{
	if (it->second.GetLabel().compare( wstrOldName) == 0)
{
	std::pair <std::string, MessageDefinition> myPair;
	myPair = std::make_pair( it->first, it->second);
	myPair.second.SetLabel( wstrNewName);
	HardwareMessages.erase( it);
	HardwareMessages.insert( myPair);
	blnResult = true;
break;
	}
}  // end for
	
return blnResult;
}


unsigned int SurfaceProtocol::GetHardwareControlCount() const
{
	return HardwareMessages.size();
}


std::vector <unsigned char> SurfaceProtocol::GetSysExHeader() const
{
std::vector <unsigned char> vHeader;
vHeader.clear();
vHeader.push_back( MIDI_SYSEX);

for (unsigned int i = 0; i < this->GetSysExManufacturerID().size(); i++)
{
	vHeader.push_back( this->GetSysExManufacturerID().at( i));
}

if (this->GetSysExModelID() > -1)
{
	vHeader.push_back( (unsigned char) this->GetSysExModelID());
}

if (this->GetSysExDeviceID() > -1)
{
	vHeader.push_back( (unsigned char) this->GetSysExDeviceID());
}

return vHeader;
}


DisplayDefinition SurfaceProtocol::GetSysExDisplay( std::string strControlHash)
{
std::map <std::string, DisplayDefinition> ::iterator it;
DisplayDefinition myDisplayDefinition;
bool blnFound = false;

for (it = DisplayMessages.begin(); it != DisplayMessages.end(); it++)
{
	unsigned int nSize = it->first.length(); 			
	
	if (strControlHash.substr( 0, nSize).compare( it->first) == 0)
	{
myDisplayDefinition = it->second;
			blnFound = true;
	}
			}  // end for

if (blnFound)
{
	return myDisplayDefinition;
}
		else
		{
			// Error:
					throw RB_NO_DISPLAY_DEFINITION;
		}
}


std::pair <std::wstring, std::map <unsigned char, std::wstring> > SurfaceProtocol::GetTranslationTable( std::string strTableID)
{
std::map <std::string, std::pair <std::wstring, std::map <unsigned char, std::wstring> > > ::iterator it;
it = CharacterTables.find( strTableID);

if (it != CharacterTables.end())
{
	return it->second;
}
else
{
throw RBException( wxT( "No matching table."));
	return it->second;
}
}


void SurfaceProtocol::SetTranslationTable( std::string strTableID, std::pair <std::wstring, std::map <unsigned char, std::wstring> > myTablePair)
{
	std::pair <std::string, std::pair <std::wstring, std::map <unsigned char, std::wstring> > > myPair;	
	myPair = std::make_pair( strTableID, myTablePair);	
	
std::map <std::string, std::pair <std::wstring, std::map <unsigned char, std::wstring> > >::iterator it;

it = CharacterTables.find( strTableID);

if (it != CharacterTables.end())
{
	CharacterTables.erase( it);
}
	
CharacterTables.insert( myPair);
}


std::wstring SurfaceProtocol::GetTranslatedByte( std::string strTableID, unsigned char nByteValue)
		{
	std::wstring wstrResult;
	wstrResult.clear();
std::pair <std::wstring, std::map <unsigned char, std::wstring> > myTable;
	
try
		{
			myTable = GetTranslationTable( strTableID);
}  // end try
catch( RBException &myException)
{
#ifdef __WINDOWS__ 
OutputDebugString( myException.what());
					#endif

// If we can't find a translation table, treat bytes as ASCII
wstrResult.push_back( nByteValue);
	return wstrResult;
}  // end catch

			try
				{
					wstrResult = myTable.second.at( nByteValue);
			}  // End try
			catch( int nError)
{
if (nError == ID_NO_MATCHING_CHARACTER)
{
	wstrResult = wstrGraphicLabel;
	wstrResult.push_back( nByteValue);
	wstrResult.append( wstrSpace);
}
			}
	
			return wstrResult;
}


std::wstring SurfaceProtocol::GetTranslatedString( std::string strTableID, std::wstring wstrText)
{
	if (wstrText.empty())
		{
			return wstrText;
	}
	
	std::wstring wstrResult;
	wstrResult.clear();
	
	for (unsigned int i = 0; i < wstrText.length(); i++)
	{
		wstrResult.append( GetTranslatedByte( strTableID, wstrText.at( i)));
	}
	return wstrResult;
}


int SurfaceProtocol::GetTranslationTableCount()
{
	return CharacterTables.size();
}


std::vector <std::wstring> SurfaceProtocol::GetTableLabels()
{
	std::map <std::string, std::pair <std::wstring, std::map <unsigned char, std::wstring> > > ::iterator it;
std::vector <std::wstring> myTableLabels;

for (it = CharacterTables.begin(); it != CharacterTables.end(); it++)
{
	myTableLabels.push_back( it->second.first); 
}

return myTableLabels;
}

bool SurfaceProtocol::DeleteTranslationTable( std::string strTableID)
{
std::map <std::string, std::pair <std::wstring, std::map <unsigned char, std::wstring> > > ::iterator it;
it = CharacterTables.find( strTableID);

if (it != CharacterTables.end())
{
	CharacterTables.erase( it);
	return true;
}
else
{
return false;
}
}


std::string SurfaceProtocol::GetTranslationTableID( std::wstring wstrTableName)
{
std::string strResult;
strResult.clear();

std::map <std::string, std::pair <std::wstring, std::map <unsigned char, std::wstring> > >::iterator it;

for (it = CharacterTables.begin(); it != CharacterTables.end(); it++)
{
	if (it->second.first.compare( wstrTableName) == 0)
	{
		strResult = it->first;
		break;
	}
} // end for

return strResult;
}


std::wstring SurfaceProtocol::GetTranslationTableName( std::string strTableID)
{
std::map <std::string, std::pair <std::wstring, std::map <unsigned char, std::wstring> > >::iterator it;
it = CharacterTables.find( strTableID); 

if (it == CharacterTables.end())
{
	std::wstring wstrEmpty;
	wstrEmpty.clear();
	return wstrEmpty;
}
else
	{
return it->second.first;
}
}


void SurfaceProtocol::SetTranslationTableName( std::string strTableID, std::wstring wstrNewName)
{
std::map <std::string, std::pair <std::wstring, std::map <unsigned char, std::wstring> > >::iterator it;
it = CharacterTables.find( strTableID); 

if (it != CharacterTables.end())
{
	it->second.first = wstrNewName;
}
}


int SurfaceProtocol::GetDisplayCursorPosition( std::string strHash, std::vector <unsigned char> vMessage)
{
if (strHash.substr( 0, 1).compare( strNibbleHashPrefix) == 0)
																																{
																																MIDI myMIDI;
																																return myMIDI.ShortMsgData1HighNibble( vMessage);
																																}
																																else
																																{
if (strHash.substr( 0, 1).compare( strSysExHashPrefix) == 0)
																																{
																																	return vMessage.at( (GetSysExHeader().size() + GetDisplay( strHash).GetSysExAddressBytes().size()));
}
else
																																{
return -1;
}
																																}
																																}


void SurfaceProtocol::SetProtocolID( std::string strNewProtocolID)
{
strProtocolID = strNewProtocolID;
}


unsigned int SurfaceProtocol::GetMessageBufferSize()
{
return nMsgBufferSize;
}


void SurfaceProtocol::SetMessageBufferSize()
{
std::map <std::string, MessageDefinition> ::iterator it;
unsigned int nMax = 0;

for (it = HardwareMessages.begin(); it != HardwareMessages.end(); it++)
{
	std::string strHash = it->first;
	std::wstring wstrHash( strHash.begin(), strHash.end());

	unsigned int nLength = CharCount( wstrHash, wxucFullStop);
	
	if (strHash.substr( 0, 1).compare( strNibbleHashPrefix) == 0)  // We want the number of bytes, so halve the count for nibble-based hashes
		{
	nLength = nLength / 2;
	}
	
	if (nLength > nMax)
	{
nMax = nLength; 
	}  // end if
}  // end for

nMsgBufferSize = nMax / SHORT_MIDI_MESSAGE_LENGTH;
unsigned int nRemainder = nMax % SHORT_MIDI_MESSAGE_LENGTH;

if (nRemainder > 0)
{
nMsgBufferSize++; 
}
}


bool SurfaceProtocol::GetDropExtraStatusBytes()
{
return blnDropExtraStatusBytes;
}


void SurfaceProtocol::SetDropExtraStatusBytes( bool blnDropBytes)
{
blnDropExtraStatusBytes = blnDropBytes;
}


std::pair <MessageDefinition, std::string> SurfaceProtocol::GetNibbleHardwareControl( std::string strControlHash)
{
std::map <std::string, MessageDefinition> ::iterator it;

for (it = HardwareMessages.begin(); it != HardwareMessages.end(); it++)
{
	std::string strKey = it->first; 

	int nValuePos = strKey.find( NIBBLE_WILD_CARD_MARKER, 0);
int nFirstRangePos = strKey.find( NIBBLE_RANGE_MARKER, 0);
bool blnRange = false;
bool blnValue = false;

if ((nValuePos == wxNOT_FOUND) && (nFirstRangePos == wxNOT_FOUND))
{
	strKey = it->first;	
}
else   // search for the shortest substring
{
	if (nValuePos != wxNOT_FOUND)
	{
blnValue = true;
strKey = strKey.substr( 0, nValuePos);
}
	else if (nFirstRangePos != wxNOT_FOUND)
	{
blnRange = true;
strKey = strKey.substr( 0, nFirstRangePos);
}
	else if (nValuePos < nFirstRangePos)
	{
		blnValue = true;
		strKey = strKey.substr( 0, nValuePos);
	}
	else
	{
blnRange = true;
strKey = strKey.substr( 0, (nFirstRangePos -1));
	}
}

if (strControlHash.substr( 0, strKey.length()).compare( strKey) == 0)  // Possible match
{

if (blnRange)
{
	std::string strTest = it->first;
	int nEnd = strControlHash.find( HASH_DELIMITER, nFirstRangePos);
std::string strValue = strControlHash.substr( nFirstRangePos, (nEnd - nFirstRangePos) +1);
	
int nMinPos = nFirstRangePos +2;
nEnd = it->first.find( HASH_DELIMITER, nMinPos);
std::string strMin = it->first.substr( nMinPos, (nEnd - nMinPos));
	
int nMaxPos = nEnd +1;
nEnd = it->first.find( HASH_DELIMITER, (nMaxPos));
std::string strMax = it->first.substr( nMaxPos, (nEnd - nMinPos));
if (strMax.substr( (strMax.length() -1), 1).compare( HASH_DELIMITER) == 0)
{
	strMax = strMax.substr( 0, (strMax.length() -1));
}

long lValue, lMin, lMax;
lValue = boost::lexical_cast <long> (strValue);
lMin = boost::lexical_cast <long> (strMin);
lMax = boost::lexical_cast <long> (strMax);

unsigned char nValue = (unsigned char) lValue;
unsigned char nMin = (unsigned char) lMin;
unsigned char nMax = (unsigned char) lMax;

if ((nValue >= nMin) && (nValue <= nMax))
{
	std::pair <MessageDefinition, std::string> myPair;
myPair = std::make_pair( it->second, it->first);
	return myPair;	
}	
}

else if (blnValue)
{
std::string strRemainder = it->first;
unsigned int nStartPos = nValuePos +2;
unsigned int nLength = (strRemainder.length() - nValuePos) -2;
strRemainder = strRemainder.substr( nStartPos, nLength);
	
if (strControlHash.substr( nStartPos, nLength).compare( strRemainder) == 0)
{
std::pair <MessageDefinition, std::string> myPair;
myPair = std::make_pair( it->second, it->first);
	return myPair;	
}  // Remainder matches the rest of the incoming message after skipping the value wild card
}  // end else if blnValue
}  // end if substring match
}  // end for

			// Error:
					throw RB_NO_CONTROL_DEFINITION;
}



