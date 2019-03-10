// MIDISurface.h

#ifndef MIDI_SURFACE_H
#define MIDI_SURFACE_H

#pragma once

#include "MIDIWidget.h"
#include "RBSpeech.h"
#include "SurfaceParameters.h"
#include "ProtocolCollection.h"
#include "AppCollection.h"

#ifdef __WINDOWS__ 
#include <windows.h>
#define SLEEP( milliseconds ) Sleep( (DWORD) milliseconds )
#else // Unix variants
#include <unistd.h>
#define Sleep( milliseconds ) usleep( (unsigned long) (milliseconds * 1000.0) )
#endif


#include <boost/log/trivial.hpp>
#include <boost/log/expressions.hpp>
#include <boost/log/sinks/text_file_backend.hpp>
#include <boost/log/utility/setup/file.hpp>
#include <boost/log/utility/setup/common_attributes.hpp>
#include <boost/log/sources/severity_logger.hpp>
#include <boost/log/sources/record_ostream.hpp>
#include <boost/shared_ptr.hpp>

// Enumeration to handle Unicode characters encoded in 7-bit MIDI messages
enum UCode
{
HByteMSNibble = 0,
HByteLSNibble,
LByteMSNibble,
LByteLSNibble 
};


// define a custom event type 
wxDEFINE_EVENT( wxEVT_MIDI_SURFACE_UPDATE, wxThreadEvent);

// it may also be convenient to define an event table macro for this event type
#define EVT_MIDI_SURFACE_UPDATE( id, fn) \
    DECLARE_EVENT_TABLE_ENTRY( \
        wxEVT_MIDI_SURFACE_UPDATE, id, wxID_ANY, \
        wxThreadEventHandler(fn), \
        (wxObject *) NULL \
    ),

// declare a new type of event, to be used by our MIDISurface class:
        wxDECLARE_EVENT(wxEVT_COMMAND_MIDISURFACE_UPDATE, wxThreadEvent);


/*
// Structure for the action queue
struct ActionRequest
	{
SurfaceAction Action;
int ControlState;
AppConfig * AppConfigPointer;
};
*/

const unsigned char CH_SPACE = 32;
const int NO_CURSOR = 0;


class SpeechObject: public wxObject
{
public:
SpeechObject():
	  m_Msg( wstrEmpty),
	  blnInterrupt( false)
	  {};

~SpeechObject()
{};

void SetMsg( std::wstring wstrNewMsg)
{
	m_Msg = wstrNewMsg;
}

std::wstring GetMsg()
{
return m_Msg;
}

bool Interrupt()
{
return blnInterrupt;
}

void Interrupt( bool blnNewInterrupt)
{
blnInterrupt = blnNewInterrupt;
}

private:
	std::wstring m_Msg;
	bool blnInterrupt;
};


class StatusObject: public wxObject
{
public:
StatusObject():
	  m_Mode( wstrEmpty)
	  {};

~StatusObject()
{};

void SetMode( std::wstring wstrNewMode)
{
	m_Mode = wstrNewMode;
}

std::wstring GetMode()
{
return m_Mode;
}

private:
	std::wstring m_Mode;
};


class DisplayObject: public wxObject
{
public:
DisplayObject():
	  m_Label( wstrEmpty),
	  m_Contents( wstrEmpty),
	  blnIsLEDLamp( false),
	  nLineCount( 0)
	  {};

~DisplayObject()
{};

void SetLabel( std::wstring wstrNewLabel)
{
	m_Label = wstrNewLabel;
}

std::wstring GetLabel()
{
return m_Label;
}

void SetContents( std::wstring wstrNewContents)
{
	m_Contents = wstrNewContents;
}

std::wstring GetContents()
{
return m_Contents;
}

bool IsLEDLamp()
{
return blnIsLEDLamp;
}

void IsLEDLamp( bool blnLEDLamp)
{
blnIsLEDLamp = blnLEDLamp;
}

int GetLineCount()
{
return nLineCount;
}

void SetLineCount( int nNewCount)
{
nLineCount = nNewCount;
}

private:
	std::wstring m_Label, m_Contents;
bool blnIsLEDLamp;
int nLineCount;
};


class HotSpotObject: public wxObject
{
public:
HotSpotObject():
	  m_SpotName( wstrEmpty),
	  m_HSCFileName( wstrEmpty)
	  	  {};

~HotSpotObject()
{};

void SetSpotName( std::wstring wstrSpotName)
{
	m_SpotName = wstrSpotName;
}

std::wstring GetSpotName()
{
return m_SpotName;
}

void SetHSCFileName( std::wstring wstrNewFileName)
{
	m_HSCFileName = wstrNewFileName;
}

std::wstring GetHSCFileName()
{
return m_HSCFileName;
}

private:
	std::wstring m_SpotName, m_HSCFileName;
	};


class SRFunctionObject: public wxObject
{
public:
SRFunctionObject():
	  m_FunctionName( wstrEmpty),
	  	  m_ActionType( ID_FUNCTION)
{};

~SRFunctionObject()
{};

void SetFunctionName( std::wstring wstrNewFunctionName)
{
	m_FunctionName = wstrNewFunctionName;
}

std::wstring GetFunctionName()
{
return m_FunctionName;
}

void SetActionType( ScreenReaderActionType newActionType)
{
	m_ActionType = newActionType;
}

ScreenReaderActionType GetActionType()
{
return m_ActionType;
}

private:
	std::wstring m_FunctionName;
ScreenReaderActionType m_ActionType;
};


class SurfaceFrame;

class MIDISurface : public MIDIWidget 
	{
public:
MIDISurface(SurfaceFrame * parent, ActiveProduct apProduct, SurfaceParameters * myParameters, boost::shared_ptr<ProtocolCollection> pProtocols, boost::shared_ptr<AppCollection> pApps);
	MIDISurface( SurfaceFrame * parent, ActiveProduct apProduct, boost::shared_ptr<ProtocolCollection> pProtocols, boost::shared_ptr<AppCollection> pApps);
			MIDISurface(SurfaceFrame * parent, ActiveProduct apProduct, std::wstring wstrName, boost::shared_ptr<ProtocolCollection> pProtocols, boost::shared_ptr<AppCollection> pApps);
			MIDISurface(SurfaceFrame * parent, ActiveProduct apProduct, std::wstring wstrName, std::string myNewProtocolID, boost::shared_ptr<ProtocolCollection> pProtocols, boost::shared_ptr<AppCollection> pApps);
			        	MIDISurface( SurfaceFrame * parent, ActiveProduct apProduct, std::wstring wstrName, std::string strNewProtocolID, int nHWIn, int nHWOut, int nDisplayIn, int nDisplayOut, boost::shared_ptr<ProtocolCollection> pProtocols, boost::shared_ptr<AppCollection> pApps);
	    ~MIDISurface();

void FlushLog();
void AnalyseDisplayMessage(std::vector <unsigned char> vBytes);
void AnalyseHardwareMessage(std::vector <unsigned char> vBytes);
void SendHardwareMessage( std::vector <unsigned char> * pMessage);
void SendDisplayMessage( std::vector <unsigned char> * pMessage);

std::string GetSurfaceID();
			std::wstring GetSurfaceName();
	void SetSurfaceName( std::wstring wstrName);
	std::string GetProtocolID();
	void SetProtocolID( std::string myNewProtocolID);
	std::wstring GetProtocolName();
void SetProtocolName( std::wstring wstrNewProtocolName);

bool IsAutoMapDevice();
void SetAutoMap( bool blnNovation);

	// appconfig
	std::pair <std::wstring, std::wstring> GetAppConfigID();
void SetAppConfigID( std::pair <std::wstring, std::wstring> myNewAppConfigID);	
std::wstring GetAppConfigName();
void SetAppConfigName( std::wstring wstrNewAppConfigName);

std::wstring GetDisplayText( std::wstring wstrLabel);
std::wstring GetCurrentState( std::wstring wstrDisplayLabel);

	bool IsValidReadDisplayDelay( int nDelay);
	int GetReadDisplayDelay();
	bool SetReadDisplayDelay( int nNewDelay);
bool StopHardwareProcessing();
bool StopDisplayProcessing();
bool ResumeHardwareProcessing();
bool ResumeDisplayProcessing();
void LogIt( std::wstring wstrMessage);
void LogIt( std::string strMessage);

private:
	// Logging
boost::log::sources::severity_logger<boost::log::trivial::severity_level> lg; //create a logging level.
	
void Speak( std::wstring wstrText, bool blnInterrupt);
void UpdateStatusBar( std::wstring wstrMode);
	void UpdateUI( std::wstring wstrLabel, bool blnLEDLamp, int nDisplayLength, int nLineCount, SurfaceProtocol * pProtocol, std::string strTranslationID);
	std::wstring FormatDisplay( const std::wstring &wstrDisplayText, const int &nDisplayLength, const int &nLineCount);
	int GetNibble( int nNibblePos, std::vector <unsigned char> myMessage);
	void SetCurrentState( std::wstring wstrDisplayLabel, std::wstring wstrState);
void SetDisplayText( std::wstring wstrLabel, std::wstring wstrText, bool blnCursorFromLeft, bool blnLEDLamp, int nDisplayLength, int nLineCount, SurfaceProtocol * pProtocol, std::string strTranslationID);
void SetDisplayText( std::wstring wstrLabel, int nCursorPosition, std::wstring wstrNewText, bool blnCursorFromLeft, bool blnLEDLamp, int nDisplayLength, int nLineCount, SurfaceProtocol * pProtocol, std::string strTranslationID);
bool IsNoteMessage( std::string strHash);
bool IsNoteOffMessage( std::string strHash, int nDataValue);
void UpdateDisplay( std::pair <DisplayDefinition, std::string> myDisplayPair, std::vector <unsigned char> myMessage, SurfaceProtocol * myProtocol);
std::pair <int, int> GetCursorInfo( std::string strHash, std::vector <unsigned char> myMessage, DisplayDefinition * myDisplay, SurfaceProtocol * pProtocol);

// Action processing
std::pair <MessageDefinition, std::string> IdentifyHardwareControl( std::vector <unsigned char> vBuffer, SurfaceProtocol * pProtocol);
std::pair <DisplayDefinition, std::string> IdentifyDisplay( std::vector <unsigned char> vBuffer, AppConfig * pAppConfig);
void ProcessLiveModeMessage( std::pair <MessageDefinition, std::string> myHardwarePair, SurfaceProtocol * pProtocol, AppConfig * pAppConfig, unsigned int nControlValue, std::vector <unsigned char> vBuffer);
bool ProcessIDModeMessage( std::string strHash, MessageDefinition myMessageDefinition, SurfaceProtocol * pProtocol, AppConfig * pAppConfig, unsigned int nControlState, std::vector <unsigned char> vBuffer);
bool ProcessConfigModeMessage( std::string strHash, MessageDefinition myMessageDefinition, SurfaceProtocol * pProtocol, AppConfig * pAppConfig, unsigned int nControlState, std::vector <unsigned char> vBuffer);
void ProcessScreenReaderModeMessage( std::string strHash, MessageDefinition myMessageDefinition, SurfaceProtocol * pProtocol, AppConfig * pAppConfig, unsigned int nControlValue, std::vector <unsigned char> vBuffer);
std::wstring GetActionString( SurfaceAction myAction, MessageDefinition myMessageDefinition, std::string strHash, int nControlState, SurfaceProtocol * pProtocol, AppConfig * pAppConfig, std::vector <unsigned char> vBuffer);
std::wstring GetControlLabelString( MessageDefinition * myDefinition, std::string strHash, int nDataValue, AppConfig * pAppConfig);
std::wstring GetControlStateString( MessageDefinition * myDefinition, int nDataValue, AppConfig * pAppConfig);
std::wstring GetControlLabelPlusStateString( MessageDefinition * myDefinition, std::string strHash, int nDataValue, AppConfig * pAppConfig);
void ProcessDisplayActions();
std::wstring GetLEDString( std::string strHash, DisplayDefinition * pDisplay, SurfaceProtocol * pProtocol, int nDataValue);
std::wstring GetDisplayString( SurfaceAction myAction, AppConfig * pAppConfig, int nDataValue);
std::wstring GetDisplayLineString( SurfaceAction myAction, AppConfig * pAppConfig);
std::wstring GetDisplayStripString( SurfaceAction myAction, AppConfig * pAppConfig);
std::wstring GetDisplaySubstring( SurfaceAction myAction, AppConfig * pAppConfig);
std::wstring GetLEDString( SurfaceAction myAction, AppConfig * pAppConfig, int nDataValue);
	std::wstring GetSpeakString( SurfaceAction myAction, AppConfig * pAppConfig);
void ProcessChangeSurfaceMode( SurfaceAction myAction);
void ProcessRunHotSpot( SurfaceAction myAction, AppConfig * pAppConfig);
void ProcessRunScreenReaderFunction( SurfaceAction myAction, AppConfig * pAppConfig);
std::wstring GetMIDIInfoString( bool blnIsEncrypted, SurfaceAction myAction, AppConfig * pAppConfig, std::vector <unsigned char> vBuffer);
std::wstring DecodeText( std::vector <unsigned char> vBytes, SurfaceProtocol * pProtocol, DisplayDefinition * myDisplay);
std::wstring UniCoded( std::vector <unsigned char> vBytes);

friend std::ostream & operator<<( std::ostream &os, const MIDISurface &MS);
friend std::istream & operator>>( std::istream &is, const MIDISurface &MS);
friend class boost::serialization::access;
template<class Archive>
void serialize(Archive & ar, const unsigned int version)
{
//Serialize the widget base class.
	const char* cWidgetTag ="Surface";
ar & boost::serialization::make_nvp(cWidgetTag, boost::serialization::base_object<MIDIWidget>(*this));
const char * cAutoMapTag= "AutoMap";
ar & boost::serialization::make_nvp( cAutoMapTag, blnAutoMap);
const char* cProtocolIDTag ="ProtocolID";
ar & boost::serialization::make_nvp(cProtocolIDTag, myProtocolID);

if (version > 1)
{
const char* cProtocolNameTag ="ProtocolName";
ar & boost::serialization::make_nvp(cProtocolNameTag, wstrProtocolName);
}

const char* cAppConfigIDTag ="AppConfigID";
ar & boost::serialization::make_nvp(cAppConfigIDTag, myAppConfigID);
	
if (version > 1)
{
const char* cAppConfigNameTag ="AppConfigName";
ar & boost::serialization::make_nvp(cAppConfigNameTag, wstrAppConfigName);
}

// Support for configurable MIDI buffer size and processing delays introduced in V1, buffer size dropped in V2
if((version > 0) & (version < 2))
            {
				const char * cReadDisplayDelayTag= "ReadDisplayDelay";
				ar & boost::serialization::make_nvp( cReadDisplayDelayTag, nReadDisplayDelay);
} // if version > 0
}
	
// Event handlers
void OnDisplayUpdate(wxThreadEvent& event);

	// Internal storage
// Frame to target with updates
SurfaceFrame * pMyParent;
ActiveProduct MyProduct;

	// Special case: Novation AutoMap devices:
bool blnAutoMap;

std::string mySurfaceID;
	std::string myProtocolID;
	std::wstring wstrProtocolName;
	std::pair <std::wstring, std::wstring> myAppConfigID; 
std::wstring wstrAppConfigName;

				// Map to hold the actual contents of the displays, indexed by their labels
std::map <std::wstring, std::wstring> myDisplays;

int nReadDisplayDelay;

boost::shared_ptr<ProtocolCollection> pMyProtocols;
boost::shared_ptr<AppCollection> pMyApps;

// Message buffering
std::vector <unsigned char> vHardwareBuffer, vDisplayBuffer;
unsigned int nHardwareMessages, nDisplayMessages;
};  // end of class

// Set the class version for serialisation
BOOST_CLASS_VERSION(MIDISurface, 3);
	
typedef MIDISurface * SurfacePointer;
typedef std::vector <SurfacePointer> SurfaceVectorType;

void HardwareCallback(double deltatime, std::vector< unsigned char > *message, void *pSurface);
void DisplayCallback(double deltatime, std::vector< unsigned char > *message, void *pSurface);
#endif

