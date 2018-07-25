// Protocol.h
//SurfaceProtocol Class.


// #define _SCL_SECURE_NO_WARNINGS

#ifndef PROTOCOL_H
#define PROTOCOL_H

#pragma once

		#include <vector>
#include <map>
#include <string>
#include "DisplayDefinition.h"

#include <boost/uuid/uuid_generators.hpp>
		#include <boost/uuid/uuid.hpp>
		#include <boost/uuid/uuid_io.hpp>         // streaming operators etc. 
#include <boost/uuid/uuid_serialize.hpp>

#include <wx/string.h>
//#include <wx/msw/winundef.h> 


const int RB_NO_DISPLAY_DEFINITION = -2;
const int RB_NO_CONTROL_DEFINITION = -3;
const unsigned int GLOBAL_STATE_INDEX = 0;


class SurfaceProtocol 
 {
public:
	SurfaceProtocol();
	~SurfaceProtocol();

	// Create a new protocol from an existing one:
SurfaceProtocol Clone( std::wstring wstrNewName);
	
// Setters and getters
bool IsUsingGlobalStates() const
{
return blnUseGlobalStates;
}


void SetGlobalStatesUsage( bool blnNewValue)
{
blnUseGlobalStates = blnNewValue;
}


bool IsEncrypted() const
		{
			return blnIsEncrypted;
	}
	
	void SetEncryptionFlag( bool blnValue)
		{
blnIsEncrypted = blnValue;			
	};
	
	std::wstring GetProtocolID() const;	
	
// SysEx page
std::wstring GetProtocolName() const;
void SetProtocolName( std::wstring strPName);
std::vector <long> GetSysExManufacturerID() const;
void SetSysExManufacturerID( std::vector <long> vID);
int GetSysExModelID() const;
void SetSysExModelID( int nID);
int GetSysExDeviceID() const;
void SetSysExDeviceID( int nDeviceID);
std::vector <unsigned char> GetSysExHeader() const;

// Display pages
std::map <std::wstring, DisplayDefinition> GetDisplays()
	{
		return DisplayMessages;
}

DisplayDefinition GetDisplay( std::wstring strControlHash);
int GetNibbleCursorPos( std::wstring strHash);
std::pair <DisplayDefinition, std::wstring> GetNibbleDisplay( std::wstring strControlHash);
DisplayDefinition GetSysExDisplay( std::wstring strControlHash);
void SetDisplay( std::wstring strHash, DisplayDefinition myDisplayDefinition);
std::vector <std::wstring> GetDisplayNames();
DisplayDefinition GetDisplayItem( unsigned int nItem);
std::wstring GetDisplayHash( std::wstring wstrName);
bool DeleteDisplay( std::wstring strHash);
bool RenameDisplay( std::wstring wstrOldName, std::wstring wstrNewName);
wxInt32 GetDisplayLines( wxInt32 nHash) const;
void SetDisplayLines( wxInt32 nHash, wxInt32 nLines);
wxInt32 GetDisplayLineLength( wxInt32 nHash) const;
void SetDisplayLineLength( wxInt32 nHash, wxInt32 nDisplayLength);
wxInt32 GetFirstDisplayBytePosition( wxInt32 nHash) const;
void SetFirstDisplayBytePosition( wxInt32 nHash, wxInt32 nOffset);
wxInt32 GetDisplayCursorPositionOffset( wxInt32 nHash) const;
void SetDisplayCursorPositionOffset( wxInt32 nHash, wxInt32 nOffset);
int GetDisplayCursorPosition( std::wstring strHash, std::vector <unsigned char> vMessage);
unsigned int GetDisplayCount() const;
int GetTranslationTableCount();
std::wstring GetTranslationTableID( std::wstring wstrTableName);
std::wstring GetTranslationTableName( std::wstring strTableID);
void SetTranslationTableName( std::wstring strTableID, std::wstring wstrNewName);
std::vector <std::wstring> GetTableLabels();
std::pair <std::wstring, std::map <unsigned char, std::wstring> > GetTranslationTable( std::wstring strTableID);
void SetTranslationTable( std::wstring strTableID, std::pair <std::wstring, std::map <unsigned char, std::wstring> > vTable);
bool DeleteTranslationTable( std::wstring strTableID);
std::wstring GetTranslatedString( std::wstring strTableID, std::wstring wstrText);

// Hardware page
MessageDefinition GetHardwareControl( std::wstring strControlHash);
std::pair <MessageDefinition, std::wstring> GetNibbleHardwareControl( std::wstring strControlHash);
bool SetHardwareControl( std::wstring strControlHash, MessageDefinition myDefinition);
std::map <std::wstring, MessageDefinition> GetHardwareControls()
	{
return HardwareMessages;		
}

std::vector <std::wstring> GetHardwareControlNames();
std::pair <std::wstring, MessageDefinition> GetHardwareControlItem( unsigned int nItem);
MessageDefinition GetHardwareControlDefinitionItem( unsigned int nItem);
std::wstring GetHardwareControlHashItem( unsigned int nItem);
std::wstring GetHardwareControlHash( std::wstring wstrName);
bool DeleteHardwareControl( std::wstring strHash);
bool RenameHardwareControl( std::wstring wstrOldName, std::wstring wstrNewName);
unsigned int GetHardwareControlCount() const;
	
	// Message buffering
unsigned int GetMessageBufferSize();
void SetMessageBufferSize();
bool GetDropExtraStatusBytes();
void SetDropExtraStatusBytes( bool nDropBytes);

private:
void SetProtocolID( std::wstring strNewProtocolID);

// Flag to indicate whether this protocol should be encrypted
bool blnIsEncrypted;

// Unique protocol ID, used to link to surfaces and AppConfigs contained in AppMap
	std::wstring strProtocolID;
std::wstring GetTranslatedByte( std::wstring strTableID, unsigned char nByteValue);

friend std::ostream & operator<<( std::ostream &os, const SurfaceProtocol &SP);
friend std::istream & operator>>( std::istream &is, const SurfaceProtocol &SP);
friend class boost::serialization::access;
    
	template<class Archive>
void serialize( Archive & myArchive, const unsigned int version)
     	{
 								try
									{
										const char * cProtocolNameTag = "ProtocolName";
myArchive & boost::serialization::make_nvp( cProtocolNameTag, strProtocolName);
const char * cEncryptedTag = "Encrypted";
myArchive & boost::serialization::make_nvp( cEncryptedTag, blnIsEncrypted);
const char * cProtocolIDTag = "ProtocolID";
myArchive & boost::serialization::make_nvp( cProtocolIDTag, strProtocolID);
// System Exclusive
const char * cSysExManufacturerIDTag = "SysExManufacturerID";
myArchive & boost::serialization::make_nvp( cSysExManufacturerIDTag, vSysExManufacturerID);
const char * cSysExModelIDTag = "SysExModelID";
myArchive & boost::serialization::make_nvp( cSysExModelIDTag, nSysExModelID);
const char * cSysExDeviceIDTag = "SysExDeviceID";
myArchive & boost::serialization::make_nvp( cSysExDeviceIDTag, nSysExDeviceID);
// Hardware input
const char * cHardwareMessagesTag = "HardwareMessages";
myArchive & boost::serialization::make_nvp( cHardwareMessagesTag, HardwareMessages);
// Display output
const char * cDisplayMessagesTag = "DisplayMessages";
myArchive & boost::serialization::make_nvp( cDisplayMessagesTag, DisplayMessages);
const char * cCharTablesTag= "CharacterTables";
myArchive & boost::serialization::make_nvp( cCharTablesTag, CharacterTables);
								
        // Support for protocol-global display states only in V1 onwards:
if(version > 0)
            {
				const char * cGlobalStatesTag= "UseGlobalStates";
myArchive & boost::serialization::make_nvp( cGlobalStatesTag, blnUseGlobalStates);
}  // end if > v0

// Message buffering introduced in V2 to handle HUI messages
if(version > 1)
            {
// Message buffering
const char * cMsgBufferSizeTag = "MessageBufferSize";
myArchive & boost::serialization::make_nvp( cMsgBufferSizeTag , nMsgBufferSize);
const char * cDropExtraStatusBytesTag = "DropExtraStatusBytes";
myArchive & boost::serialization::make_nvp( cDropExtraStatusBytesTag , blnDropExtraStatusBytes);
}								  // end if > v1
								}  // end try
catch( ...)
{
throw ID_LOAD_DATA_EXCEPTION;
}  // end catch
}
	

// Internal storage
// Surface page
	std::wstring strProtocolName;
// SysEx page
	std::vector <long> vSysExManufacturerID;
int nSysExModelID, nSysExDeviceID;

// Message definition storage for all message pages
// These maps store the ControlHash of the message as an std::string key and a MessageDefinition 

// Hardware page
std::map <std::wstring, MessageDefinition> HardwareMessages;

// Display pages
std::map <std::wstring, DisplayDefinition> DisplayMessages;
bool blnUseGlobalStates;
std::map <std::wstring, std::pair <std::wstring, std::map <unsigned char, std::wstring> > > CharacterTables;

// Message buffering
unsigned int nMsgBufferSize;
bool blnDropExtraStatusBytes;
};  // end of Protocol class

// Set the class version for serialisation
BOOST_CLASS_VERSION(SurfaceProtocol, 2);
	
#endif   // PROTOCOL_H
