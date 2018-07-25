// SurfaceParameters.h
// 

#ifndef SURFACE_PARAMETERS_H
#define SURFACE_PARAMETERS_H

#pragma once

#include "MIDIWidget.h"
#include "RBEnums.h"
#include "RBException.h"
#include "SRConstants.h"

#include <boost/serialization//version.hpp>
#include <boost/serialization/access.hpp>
#include <boost/serialization/nvp.hpp>

#include <wx/msw/winundef.h> 

const int MIN_READ_DISPLAY_DELAY = 0;
const int MAX_READ_DISPLAY_DELAY = 1000;   // = 1 second
const int MIN_MIDI_BUFFER_SIZE = 1;
const int DEFAULT_MIDI_BUFFER_SIZE = 1024;  
const int MAX_MIDI_BUFFER_SIZE = 2048;  

class SurfaceParameters
{
public:
	SurfaceParameters();
	
std::wstring GetSurfaceName();
void SetSurfaceName( std::wstring wstrSurfaceName);
std::wstring GetProtocolID();
void SetProtocolID( std::wstring strNewProtocolID);
std::wstring GetProtocolName();
void SetProtocolName( std::wstring wstrNewProtocolName);
int GetHardwareInID();
void SetHardwareInID( int nNewID);
int GetHardwareOutID();
void SetHardwareOutID( int nNewID);
int GetDisplayInID();
void SetDisplayInID( int nNewID);
int GetDisplayOutID();
	void SetDisplayOutID( int nNewID);
	std::wstring GetHardwareInName();
	void SetHardwareInName( std::wstring strNewName);
	std::wstring GetHardwareOutName();
void SetHardwareOutName( std::wstring strNewName);
std::wstring GetDisplayInName();
void SetDisplayInName( std::wstring strNewName);
std::wstring GetDisplayOutName();
void SetDisplayOutName( std::wstring strNewName);
std::pair <std::wstring, std::wstring> GetAppConfigID();
void SetAppConfigID( std::pair <std::wstring, std::wstring> myAppConfigID);
std::wstring GetAppConfigName();
void SetAppConfigName( std::wstring wstrNewAppConfigName);
Mode GetSurfaceMode();
void SetSurfaceMode( Mode myMode);
int GetReadDisplayDelay();
void SetReadDisplayDelay( int nNewDelay);
	
private:
friend std::ostream & operator<<( std::ostream &os, const SurfaceParameters &SP);
friend std::istream & operator>>( std::istream &is, const SurfaceParameters &SP);
friend class boost::serialization::access;
template<class Archive>
void serialize(Archive & myArchive, const unsigned int version)
{
try
	{
		const char* cSurfaceNameTag ="SurfaceName";
myArchive & boost::serialization::make_nvp(cSurfaceNameTag, m_SurfaceName);
const char* cProtocolIDTag ="ProtocolID";
myArchive & boost::serialization::make_nvp(cProtocolIDTag, m_ProtocolID);

if (version > 2)
{
const char* cProtocolNameTag ="ProtocolName";
myArchive & boost::serialization::make_nvp(cProtocolNameTag, wstrProtocolName);
}

const char* cHardwareInIDTag ="HardwareInID";
myArchive & boost::serialization::make_nvp(cHardwareInIDTag, m_HardwareInID);
const char* cHardwareInNameTag ="HardwareInName";
myArchive & boost::serialization::make_nvp(cHardwareInNameTag, m_HardwareInName);
const char* cHardwareOutIDTag ="HardwareOutID";
myArchive & boost::serialization::make_nvp(cHardwareOutIDTag, m_HardwareOutID);
const char* cHardwareOutNameTag ="HardwareOutName";
myArchive & boost::serialization::make_nvp(cHardwareOutNameTag, m_HardwareOutName);
const char* cDisplayInIDTag ="DisplayInID";
myArchive & boost::serialization::make_nvp(cDisplayInIDTag, m_DisplayInID);
const char* cDisplayInNameTag ="DisplayInName";
myArchive & boost::serialization::make_nvp(cDisplayInNameTag, m_DisplayInName);
const char* cDisplayOutIDTag ="DisplayOutID";
myArchive & boost::serialization::make_nvp(cDisplayOutIDTag, m_DisplayOutID);
const char* cDisplayOutNameTag ="DisplayOutName";
myArchive & boost::serialization::make_nvp(cDisplayOutNameTag, m_DisplayOutName);
const char* AppConfigIDTag ="AppConfigID";
myArchive & boost::serialization::make_nvp(AppConfigIDTag, m_AppConfigID);

if (version > 2)
{
const char* cAppConfigNameTag ="AppConfigName";
myArchive & boost::serialization::make_nvp(cAppConfigNameTag, wstrAppConfigName);
}

const char* SurfaceModeTag ="SurfaceMode";
myArchive & boost::serialization::make_nvp(SurfaceModeTag, m_SurfaceMode);

if(version < 4)
            {
				const char * cMIDIBufferSizeTag= "MIDIBufferSize";
myArchive & boost::serialization::make_nvp( cMIDIBufferSizeTag, m_MIDIBufferSize);
}

if (version > 0)
	{
		const char * cReadDisplayDelayTag= "ReadDisplayDelay";
myArchive & boost::serialization::make_nvp( cReadDisplayDelayTag, m_ReadDisplayDelay);
}  // end if > v0
}  // end try
catch( ...)
{
throw ID_LOAD_DATA_EXCEPTION;
}  // end catch
}
	
	// Internal storage
std::wstring m_SurfaceName;
std::wstring m_ProtocolID;
std::wstring wstrProtocolName;
int m_HardwareInID, m_HardwareOutID, m_DisplayInID, m_DisplayOutID; 
std::wstring m_HardwareInName, m_HardwareOutName, m_DisplayInName, m_DisplayOutName;
Mode m_SurfaceMode;
int m_ReadDisplayDelay;
int m_MIDIBufferSize;
std::wstring wstrAppConfigName;
std::pair <std::wstring, std::wstring> m_AppConfigID; 
};
	
// Set the class version for serialisation
BOOST_CLASS_VERSION(SurfaceParameters, 4)
#endif