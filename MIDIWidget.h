// MIDIWidget.h
//MidiWidget class.
//note: all functions are inlined where possible.

// #define _SCL_SECURE_NO_WARNINGS

#ifndef MIDIWIDGET_H
#define MIDIWIDGET_H

#pragma once


#include <atomic>
#include <vector>
#include <string>

#include "SRConstants.h"
#include "RBException.h"
#include "MIDI.h"
#include "MidiServices.h"

#include <rtmidi/RtMidi.h>

#include <boost/serialization/nvp.hpp>
#include <boost/serialization/access.hpp>

//Control surface Modes
enum Mode
	{
		ID_LIVE_MODE, //Live mode copying continues as normal.
	ID_ID_MODE, //anounce the function of buttons, etc. 
	ID_CONFIG_MODE,   // Gather message information to tie to a protocol
	ID_SCREEN_READER_MODE,    //perform screen reader functions
	ID_UNKNOWN_MODE   // Error trap
};


	//Widget class.

// RtMidi's Windows (WinMM) backend adds " <port index>" to every port name, and the index changes whenever
// ports are added or removed, so SurfaceReader matches ports on the name without it.
inline std::string PortBaseName( std::string strPortName)
{
#ifdef __WINDOWS__
	size_t nSpace = strPortName.find_last_of( ' ');
	if ((nSpace != std::string::npos)
		&& (nSpace + 1 < strPortName.size())
		&& (strPortName.find_first_not_of( "0123456789", nSpace + 1) == std::string::npos))
	{
		strPortName.erase( nSpace);
	}
#endif
	return strPortName;
}


// The index of the port with this name (ignoring RtMidi's index suffix), or -1
inline int FindPortByName( RtMidi & myMIDI, const std::string & strPortName)
{
	std::string strWanted = PortBaseName( strPortName);
	unsigned int nCount = myMIDI.getPortCount();
	for (unsigned int i = 0; i < nCount; i++)
	{
		if (PortBaseName( myMIDI.getPortName( i)) == strWanted)
		{
			return (int) i;
		}
	}
	return -1;
}

class MIDIWidget
{
public:
std::wstring GetLabel()
	{
		return wstrWidgetName;
}

void SetLabel( std::wstring wstrLabel)
	{
wstrWidgetName = wstrLabel;
}

int GetHardwareInID()
{
return nHardwareInPortID;
}

void SetHardwareInID( int nID)
{
nHardwareInPortID = nID;
}

std::string GetHardwareInName()
{
return strHardwareInPortName;
}

void SetHardwareInName( std::string strName)
{
strHardwareInPortName = strName;
}

int GetHardwareOutID()
{
return nHardwareOutPortID;
}

void SetHardwareOutID( int nID)
{
nHardwareOutPortID = nID;
}

std::string GetHardwareOutName()
{
return strHardwareOutPortName;
}

void SetHardwareOutName( std::string strName)
{
strHardwareOutPortName = strName;
}

int GetDisplayInID()
{
return nDisplayInPortID;
}

void SetDisplayInID( int nID)
{
nDisplayInPortID = nID;
}

std::string GetDisplayInName()
{
return strDisplayInPortName;
}

void SetDisplayInName( std::string strName)
{
strDisplayInPortName = strName;
}

int GetDisplayOutID()
{
return nDisplayOutPortID;
}

void SetDisplayOutID( int nID)
{
nDisplayOutPortID = nID;
}

std::string GetDisplayOutName()
{
return strDisplayOutPortName;
}

void SetDisplayOutName( std::string strName)
{
strDisplayOutPortName = strName;
}

std::vector <std::string> GetPortErrors()
	{
return vErrors;
}

void ClearPortErrors()
	{
		vErrors.clear();
}


std::wstring GetModeDescription( Mode myMode)
{
switch (myMode)
{
case ID_LIVE_MODE:
	{
	return wstrLiveMode;
}
	break;

case 	ID_ID_MODE:
	{
return wstrIDMode;
}
	break;

case 	ID_CONFIG_MODE:
	{
return wstrConfigMode;
}
	break;

case ID_SCREEN_READER_MODE:
	{
	return wstrScreenReaderMode;
}
	break;

default:
{
std::wstring wstrDefault;
wstrDefault.clear();
	return wstrDefault;
}
break;
} // end switch
}


protected:
	// internal storage
	std::wstring wstrWidgetName;
		  int nHardwareInPortID, nHardwareOutPortID, nDisplayInPortID, nDisplayOutPortID;
std::string strHardwareInPortName, strHardwareOutPortName, strDisplayInPortName, strDisplayOutPortName;
// The current mode on a widget-by-widget basis. Atomic: the MIDI input thread reads it while the UI changes it.
std::atomic <Mode> WidgetMode;

// Error strings
std::vector <std::string> vErrors;

// MIDI devices
RtMidiIn * m_HardwareMIDIIn, * m_DisplayMIDIIn;
RtMidiOut * m_HardwareMIDIOut, * m_DisplayMIDIOut;

// MIDI message buffers
std::vector <unsigned char> vHardwareMessage, vDisplayMessage;

friend std::ostream & operator<<( std::ostream &os, const MIDIWidget &MW);
friend std::istream & operator>>( std::istream &is, const MIDIWidget &MW);
friend class boost::serialization::access;
	template<class Archive> 
		void serialize(Archive & ar, const unsigned int version)
	{
//serialize the widget name and ports.
		const char* WidgetNameTag ="WidgetName";
		ar & boost::serialization::make_nvp(WidgetNameTag, wstrWidgetName);
		const char* HardwareInPortTag ="HardwareInPort";
		ar & boost::serialization::make_nvp(HardwareInPortTag, nHardwareInPortID);
	const char* HardwareInPortNameTag ="HardwareInPortName";
ar & boost::serialization::make_nvp(HardwareInPortNameTag, strHardwareInPortName);
const char* HardwareOutPortTag ="HardwareOutPort";
		ar & boost::serialization::make_nvp(HardwareOutPortTag, nHardwareOutPortID);
const char* HardwareOutPortNameTag ="HardwareOutPortName";
ar & boost::serialization::make_nvp(HardwareOutPortNameTag, strHardwareOutPortName);
		const char* DisplayInPortTag ="DisplayInPort";
		ar & boost::serialization::make_nvp(DisplayInPortTag, nDisplayInPortID);
const char* DisplayInPortNameTag ="DisplayInPortName";
ar & boost::serialization::make_nvp(DisplayInPortNameTag, strDisplayInPortName);
		const char* DisplayOutPortTag ="DisplayOutPort";
		ar & boost::serialization::make_nvp(DisplayOutPortTag, nDisplayOutPortID);
const char* DisplayOutPortNameTag ="DisplayOutPortName";
ar & boost::serialization::make_nvp(DisplayOutPortNameTag, strDisplayOutPortName);
	const char* WidgetModeTag ="WidgetMode";
// Serialized through a plain copy, since an atomic can't be; the saved format is unchanged
Mode mySavedMode = WidgetMode.load();
ar & boost::serialization::make_nvp(WidgetModeTag, mySavedMode);
WidgetMode.store( mySavedMode);
}

public:
	MIDIWidget()
	{
		wstrWidgetName.clear();
	nHardwareInPortID =-1;
nHardwareOutPortID =-1;
nDisplayInPortID =-1;
nDisplayOutPortID =-1;
	SetWidgetMode(Mode(ID_LIVE_MODE));
	
	m_HardwareMIDIIn = 0;
	m_HardwareMIDIOut = 0;
m_DisplayMIDIIn = 0;
m_DisplayMIDIOut = 0;

	// RtMidi constructors
    try 
	{
m_HardwareMIDIIn = new RtMidiIn();
m_HardwareMIDIOut = new RtMidiOut();
    	m_DisplayMIDIIn = new RtMidiIn();
m_DisplayMIDIOut = new RtMidiOut();
    	} 
catch ( RtMidiError &error ) 
{
throw RBException( error.getMessage());	
}
}
		
	MIDIWidget(std::wstring wstrName)
		:
	wstrWidgetName( wstrName),
	nHardwareInPortID( -1),
nHardwareOutPortID( -1),
nDisplayInPortID( -1),
nDisplayOutPortID( -1),
m_HardwareMIDIIn( 0),
	m_HardwareMIDIOut( 0),
m_DisplayMIDIIn( 0),
m_DisplayMIDIOut( 0)
{
			// RtMidi constructors
    try 
	{
m_HardwareMIDIIn = new RtMidiIn();
m_HardwareMIDIOut = new RtMidiOut();
    	m_DisplayMIDIIn = new RtMidiIn();
m_DisplayMIDIOut = new RtMidiOut();
    	} 
catch ( RtMidiError &error ) 
{
throw RBException( error.getMessage());	
}
SetWidgetMode(Mode(ID_LIVE_MODE));
	}

		MIDIWidget(std::wstring wstrName, int nHWIn, int nHWOut, int nDisplayIn, int nDisplayOut)
			:
			wstrWidgetName( wstrName),
	nHardwareInPortID( nHWIn),
nHardwareOutPortID( nHWOut),
nDisplayInPortID( nDisplayIn),
nDisplayOutPortID( nDisplayOut),
	m_HardwareMIDIIn( 0),
	m_HardwareMIDIOut( 0),
m_DisplayMIDIIn( 0),
m_DisplayMIDIOut( 0)
			{
// RtMidi constructors
    try 
	{
m_HardwareMIDIIn = new RtMidiIn();
m_HardwareMIDIOut = new RtMidiOut();
    	m_DisplayMIDIIn = new RtMidiIn();
m_DisplayMIDIOut = new RtMidiOut();
    	} 
catch ( RtMidiError &error ) 
{
throw RBException( error.getMessage());	
}

SetWidgetMode(Mode(ID_LIVE_MODE));
	}

~MIDIWidget()
{
												delete m_HardwareMIDIIn;
delete m_HardwareMIDIOut;
delete m_DisplayMIDIIn;
delete m_DisplayMIDIOut;
}

	
	//WidgetMode functions.
Mode GetWidgetMode()
{
	return WidgetMode.load();
}


void SetWidgetMode( Mode NewMode)
{
	WidgetMode.store( NewMode);
	return;
}

// Validate input port
bool IsValidInputPort( int nPortID, std::string strPortName)
{
if ((nPortID == VP_PORT_ID) || (nPortID == MS_PORT_ID))
{
#ifdef __WINDOWS__
	// A virtual port, or one that replaced a loopMIDI port, is a Windows MIDI Services loopback
	return MidiServices::IsAvailable();
#else
	return true;
#endif
}

try
	{
		RtMidiIn myMIDIIn;
		return ((nPortID >= 0) && (nPortID < (int) myMIDIIn.getPortCount()));
}
catch ( RtMidiError &error )
{
#ifdef __WINDOWS__
OutputDebugStringA(error.what());
#endif
return false;
}
}


// Validate output port
bool IsValidOutputPort( int nPortID, std::string strPortName)
{
if ((nPortID == VP_PORT_ID) || (nPortID == MS_PORT_ID))
	{
#ifdef __WINDOWS__
	return MidiServices::IsAvailable();
#else
	return true;
#endif
}

try
	{
		RtMidiOut myMIDIOut;
		return ((nPortID >= 0) && (nPortID < (int) myMIDIOut.getPortCount()));
}
	catch ( RtMidiError &error )
	{
#ifdef __WINDOWS__
OutputDebugStringA(error.what());
#endif
return false;
}
}


// The index to open for a saved port: found by name if there is one, since the saved index goes stale
// whenever ports are added or removed. -1 if the named port isn't there.
int ResolveInputPort( int nPortID, const std::string & strPortName)
{
	if ((nPortID == VP_PORT_ID) || (nPortID == MS_PORT_ID) || strPortName.empty())
	{
		return nPortID;
	}
	try
	{
		RtMidiIn myMIDIIn;
		return FindPortByName( myMIDIIn, strPortName);
	}
	catch ( RtMidiError &)
	{
		return -1;
	}
}


int ResolveOutputPort( int nPortID, const std::string & strPortName)
{
	if ((nPortID == VP_PORT_ID) || (nPortID == MS_PORT_ID) || strPortName.empty())
	{
		return nPortID;
	}
	try
	{
		RtMidiOut myMIDIOut;
		return FindPortByName( myMIDIOut, strPortName);
	}
	catch ( RtMidiError &)
	{
		return -1;
	}
}


// The name of this surface's virtual port: "<surface> display" or "<surface> hardware"
std::string VirtualPortName( const std::string & strSuffix)
{
#ifdef __WINDOWS__
	// UTF-8, like the port names RtMidi and the Windows MIDI Services helper use. The surface name is shortened
	// if need be, so the whole name fits WinMM's limit and " hardware" and " display" always stay distinct.
	std::string strSurface( wxString( wstrWidgetName).utf8_str());
	return MidiServices::FitPortName( strSurface, MidiServices::nMaxPortName - strSuffix.size()) + strSuffix;
#else
	return wxString( wstrWidgetName).ToStdString() + strSuffix;
#endif
}


//Open ports.

		bool OpenHardwareInput( int nInputID)
{
try 
 {
	m_HardwareMIDIIn->openPort( nInputID);
// Don't ignore sysex, timing, or active sensing messages.
m_HardwareMIDIIn->ignoreTypes( false, false, false );
} 
	catch ( RtMidiError &error ) 
	{
    #ifdef __WINDOWS__ 
 OutputDebugStringA( error.what());
					#endif

 throw RBException( error.getMessage());
	return false;
	}
	return true;
}


bool OpenDisplayInput( int nInputID)
{
try
	{
if ((nInputID == VP_PORT_ID) || (nInputID == MS_PORT_ID))
{
// The DAW's end: the surface's own virtual port, or the loopMIDI port it replaced
std::string strName = (nInputID == VP_PORT_ID) ? VirtualPortName( strVirtualInSuffix) : strDisplayInPortName;
#ifdef __WINDOWS__
// SurfaceReader's end of the surface's loopback; SurfaceFrame makes sure the loopback exists
std::string strOwnPort = MidiServices::SurfaceReaderPortName( strName);
int nPort = FindPortByName( *m_DisplayMIDIIn, strOwnPort);
if (nPort < 0)
{
	throw RBException( "Windows MIDI Services port not found: " + strOwnPort);
}
m_DisplayMIDIIn->openPort( nPort);
#else
m_DisplayMIDIIn->openVirtualPort( strName);
#endif
// Don't ignore sysex, timing, or active sensing messages.
    m_DisplayMIDIIn->ignoreTypes( false, false, false );
}
else	
{
m_DisplayMIDIIn->openPort( nInputID);
// Don't ignore sysex, timing, or active sensing messages.
    m_DisplayMIDIIn->ignoreTypes( false, false, false );
}
} 
	catch ( RtMidiError &error ) 
	{
    #ifdef __WINDOWS__ 
 OutputDebugStringA( error.what());
					#endif
		throw RBException( error.getMessage());
return false;
	}
	return true;
}


bool OpenHardwareOutput( int nOutputID)
{
try 
	{
if ((nOutputID == VP_PORT_ID) || (nOutputID == MS_PORT_ID))
{
std::string strName = (nOutputID == VP_PORT_ID) ? VirtualPortName( strVirtualOutSuffix) : strHardwareOutPortName;
#ifdef __WINDOWS__
std::string strOwnPort = MidiServices::SurfaceReaderPortName( strName);
int nPort = FindPortByName( *m_HardwareMIDIOut, strOwnPort);
if (nPort < 0)
{
	throw RBException( "Windows MIDI Services port not found: " + strOwnPort);
}
m_HardwareMIDIOut->openPort( nPort);
#else
m_HardwareMIDIOut->openVirtualPort( strName);
#endif
}
else
{
		m_HardwareMIDIOut->openPort( nOutputID);
}
}
catch ( RtMidiError &error ) 
	{
    #ifdef __WINDOWS__ 
 OutputDebugStringA( error.what());
					#endif

 throw RBException( error.getMessage());
return false;
	}
	return true;
}


bool OpenDisplayOutput( int nOutputID)
{
try 
	{
m_DisplayMIDIOut->openPort( nOutputID);
}
	catch ( RtMidiError &error )
	{
    #ifdef __WINDOWS__ 
 OutputDebugStringA( error.what());
					#endif

 throw RBException( error.getMessage());
return false;
	}
	return true;
}


bool CloseHardwareInput()
{
		try
			{
				m_HardwareMIDIIn->closePort();
		}
catch ( RtMidiError &error ) 
{
throw RBException( error.getMessage());
wxMessageBox( error.getMessage(),                wstrAppTitle, wxOK | wxICON_ERROR);
return false;
}
return true;
}

bool CloseHardwareOutput()
{
		try
			{
m_HardwareMIDIOut->closePort();
		}
catch ( RtMidiError &error ) 
{
throw RBException( error.getMessage());
wxMessageBox( error.getMessage(),                wstrAppTitle, wxOK | wxICON_ERROR);
return false;
}
return true;
}

bool CloseDisplayInput()
{
		try
			{
				m_DisplayMIDIIn->closePort();
		}
catch ( RtMidiError &error ) 
{
throw RBException( error.getMessage());
wxMessageBox( error.getMessage(),                wstrAppTitle, wxOK | wxICON_ERROR);
return false;
}
return true;
}

bool CloseDisplayOutput()
{
		try
			{
m_DisplayMIDIOut->closePort();
		}
		catch ( RtMidiError &error ) 
{
throw RBException( error.getMessage());
wxMessageBox( error.getMessage(),                wstrAppTitle, wxOK | wxICON_ERROR);
  return false;
		}
		
		return true;
}
		
	
bool OpenHardwareThru( int nHardwareInID, int nHardwareOutID)
{
bool blnResult = true;
nHardwareInID = ResolveInputPort( nHardwareInID, strHardwareInPortName);
nHardwareOutID = ResolveOutputPort( nHardwareOutID, strHardwareOutPortName);

if (IsValidInputPort( nHardwareInID, strHardwareInPortName) == false) 
	{
std::string strOut = strHardwareInError; 
strOut.append( strHardwareInPortName);
vErrors.push_back( strOut);
blnResult = false;
}

if (IsValidOutputPort( nHardwareOutID, strHardwareOutPortName) == false)
	{
std::string strOut = strHardwareOutError;
strOut.append( strHardwareOutPortName);
vErrors.push_back( strOut);			
blnResult = false;
}

if (blnResult == false)
	{
		return blnResult;
}

nHardwareInPortID = nHardwareInID;
		nHardwareOutPortID =nHardwareOutID;
bool blnOutput = false, blnInput = false;

		try
			{
			blnOutput = OpenHardwareOutput( nHardwareOutPortID);
}
catch ( RBException &myException)
		{
			#ifdef __WINDOWS__ 
OutputDebugString( myException.what());
					#endif

std::string strOut = strHardwareOutError;
strOut.append( strHardwareOutPortName);
vErrors.push_back( strOut);			
blnOutput = false;
}  // end catch

				try
					{
						blnInput = OpenHardwareInput( nHardwareInPortID);
		}
catch ( RBException &myException)
		{
									#ifdef __WINDOWS__ 
			OutputDebugString( myException.what());
			#endif
			
			std::string strOut = strHardwareInError;
			strOut.append( strHardwareInPortName);
			vErrors.push_back( strOut);			
blnInput = false;
}  // end catch
	
		if ((blnInput == false) || (blnOutput == false))
			{
				return false;
		}
		else
			{
				return true;
		}
}


bool OpenDisplayThru( int nDisplayInID, int nDisplayOutID)
{
bool blnResult = true;
nDisplayInID = ResolveInputPort( nDisplayInID, strDisplayInPortName);
nDisplayOutID = ResolveOutputPort( nDisplayOutID, strDisplayOutPortName);
	
		if (IsValidInputPort( nDisplayInID, strDisplayInPortName) == false) 
{
	std::string strOut = strDisplayInError;
	strOut.append( strDisplayInPortName);
	vErrors.push_back( strOut);			
blnResult = false;
}

if (IsValidOutputPort( nDisplayOutID, strDisplayOutPortName) == false)
{
	std::string strOut = strDisplayOutError;
	strOut.append( strDisplayOutPortName);
	vErrors.push_back( strOut);
blnResult = false;
}

if (blnResult == false)
{
		return blnResult;
}

	nDisplayInPortID = nDisplayInID;
	nDisplayOutPortID =nDisplayOutID;
bool blnInput = false, blnOutput = false;

	try
		{
			blnOutput = OpenDisplayOutput( nDisplayOutPortID);
}
	catch ( RBException &myException)
		{
			#ifdef __WINDOWS__ 						
	OutputDebugString( myException.what());
			#endif

std::string strOut = strDisplayOutError;
strOut.append( strDisplayOutPortName);
vErrors.push_back( strOut);
blnResult = false;
}  // end catch

try
					{
						blnInput = OpenDisplayInput( nDisplayInPortID);
		}
catch ( RBException &myException)
		{
			#ifdef __WINDOWS__ 						
			OutputDebugString( myException.what());
			#endif
			
			std::string strOut = strDisplayInError;
			strOut.append( strDisplayInPortName);
			vErrors.push_back( strOut);
					blnResult = false;
		}  // end catch
		
if ((blnInput == false) || (blnOutput == false))
	{
		return false;
}
else
	{
		return blnResult;
}
}
	
		
bool CloseHardwareThru()
		{
bool blnResult = CloseHardwareInput();
blnResult = CloseHardwareOutput();
return blnResult;
				}

		bool CloseDisplayThru()
{
		bool blnResult = CloseDisplayInput();
blnResult = CloseDisplayOutput();
return blnResult;
				}

		//get messages.
		std::vector <unsigned char> GetHardwareMessage()
		{
			return vHardwareMessage;
		}

		
		std::vector <unsigned char> GetDisplayMessage()
		{
return vDisplayMessage;
		}
};
#endif