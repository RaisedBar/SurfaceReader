// Protocol.h
//SurfaceProtocol Class.


// #define _SCL_SECURE_NO_WARNINGS

#ifndef PROTOCOL_H
#define PROTOCOL_H

#pragma once

		#include <vector>
#include <map>

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
	
	std::string GetProtocolID() const;	
	
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
std::map <std::string, DisplayDefinition> GetDisplays()
	{
		return DisplayMessages;
}

DisplayDefinition GetDisplay( std::string strControlHash);
int GetNibbleCursorPos( std::string strHash);
std::pair <DisplayDefinition, std::string> GetNibbleDisplay( std::string strControlHash);
DisplayDefinition GetSysExDisplay( std::string strControlHash);
void SetDisplay( std::string strHash, DisplayDefinition myDisplayDefinition);
std::vector <std::wstring> GetDisplayNames();
DisplayDefinition GetDisplayItem( unsigned int nItem);
std::string GetDisplayHash( std::wstring wstrName);
bool DeleteDisplay( std::string strHash);
bool RenameDisplay( std::wstring wstrOldName, std::wstring wstrNewName);
wxInt32 GetDisplayLines( wxInt32 nHash) const;
void SetDisplayLines( wxInt32 nHash, wxInt32 nLines);
wxInt32 GetDisplayLineLength( wxInt32 nHash) const;
void SetDisplayLineLength( wxInt32 nHash, wxInt32 nDisplayLength);
wxInt32 GetFirstDisplayBytePosition( wxInt32 nHash) const;
void SetFirstDisplayBytePosition( wxInt32 nHash, wxInt32 nOffset);
wxInt32 GetDisplayCursorPositionOffset( wxInt32 nHash) const;
void SetDisplayCursorPositionOffset( wxInt32 nHash, wxInt32 nOffset);
int GetDisplayCursorPosition( std::string strHash, std::vector <unsigned char> vMessage);
unsigned int GetDisplayCount() const;
int GetTranslationTableCount();
std::string GetTranslationTableID( std::wstring wstrTableName);
std::wstring GetTranslationTableName( std::string strTableID);
void SetTranslationTableName( std::string strTableID, std::wstring wstrNewName);
std::vector <std::wstring> GetTableLabels();
std::pair <std::wstring, std::map <unsigned char, std::wstring> > GetTranslationTable( std::string strTableID);
void SetTranslationTable( std::string strTableID, std::pair <std::wstring, std::map <unsigned char, std::wstring> > vTable);
bool DeleteTranslationTable( std::string strTableID);
std::wstring GetTranslatedString( std::string strTableID, std::wstring wstrText);

// Hardware page
MessageDefinition GetHardwareControl( std::string strControlHash);
std::pair <MessageDefinition, std::string> GetNibbleHardwareControl( std::string strControlHash);
bool SetHardwareControl( std::string strControlHash, MessageDefinition myDefinition);
std::map <std::string, MessageDefinition> GetHardwareControls()
	{
return HardwareMessages;		
}

std::vector <std::wstring> GetHardwareControlNames();
std::pair <std::string, MessageDefinition> GetHardwareControlItem( unsigned int nItem);
MessageDefinition GetHardwareControlDefinitionItem( unsigned int nItem);
std::string GetHardwareControlHashItem( unsigned int nItem);
std::string GetHardwareControlHash( std::wstring wstrName);
bool DeleteHardwareControl( std::string strHash);
bool RenameHardwareControl( std::wstring wstrOldName, std::wstring wstrNewName);
unsigned int GetHardwareControlCount() const;
	
	// Message buffering
unsigned int GetMessageBufferSize();
void SetMessageBufferSize();
bool GetDropExtraStatusBytes();
void SetDropExtraStatusBytes( bool nDropBytes);

private:
void SetProtocolID( std::string strNewProtocolID);

// Flag to indicate whether this protocol should be encrypted
bool blnIsEncrypted;

// Unique protocol ID, used to link to surfaces and AppConfigs contained in AppMap
	std::string strProtocolID;
std::wstring GetTranslatedByte( std::string strTableID, unsigned char nByteValue);

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
std::map <std::string, MessageDefinition> HardwareMessages;

// Display pages
std::map <std::string, DisplayDefinition> DisplayMessages;
bool blnUseGlobalStates;
std::map <std::string, std::pair <std::wstring, std::map <unsigned char, std::wstring> > > CharacterTables;

// Message buffering
unsigned int nMsgBufferSize;
bool blnDropExtraStatusBytes;
};  // end of Protocol class

// Set the class version for serialisation
BOOST_CLASS_VERSION(SurfaceProtocol, 2);
	
#endif   // PROTOCOL_H
