// MIDIWidget.h
//MidiWidget class.
//note: all functions are inlined where possible.

// #define _SCL_SECURE_NO_WARNINGS

#ifndef MIDIWIDGET_H
#define MIDIWIDGET_H

#pragma once


#include <vector>
#include <string>

#include "SRConstants.h"
#include "RBException.h"
#include "MIDI.h"
//#include "RBStringFuncs.h"

#ifdef __WINDOWS__ 
//Windows includes.
//#include <windows.h>
//#include <wx/msw/winundef.h> 
#endif  // if windows/Mac

#include <RtMidi.h>
//#include <wx/msw/winundef.h> 

//#include <boost/algorithm/string.hpp>
#include <boost/serialization/nvp.hpp>
#include <boost/serialization/access.hpp>

//#include <wx/msgdlg.h>
//#include <wx/msw/winundef.h> 

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
Mode WidgetMode; //store the current mode on a widget-by-widget basis.

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
ar & boost::serialization::make_nvp(WidgetModeTag, WidgetMode);
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
	return WidgetMode;
}


void SetWidgetMode( Mode NewMode)
{
	WidgetMode = NewMode;
	return;
}

// Validate input port
bool IsValidInputPort( int nPortID, std::string strPortName)
{
// Test for a currently valid port ID
if (nPortID == VP_PORT_ID)
{
	#ifdef __WINDOWS__ 
return false;
#else
	return true;
#endif
}

RtMidiIn * myMIDIIn = 0;	

try
	{
		myMIDIIn = new RtMidiIn;

		if ((nPortID < 0)
			|| (nPortID > (int) myMIDIIn->getPortCount())
			|| (myMIDIIn->getPortCount() == 0))
{
	delete myMIDIIn;
		return false;
}
		else
			{
	delete myMIDIIn;
		return true;
}

// Create an array with the character, c, and a null terminator 
char buff[2] = { '\1', '\0' };
std::string strOne( buff);
std::string strEmpty( "");

std::string strTempName = StripSpaces( strPortName);
	strTempName = StripNonAlphaNums( strTempName);
boost::replace_all(strTempName, strOne, strEmpty); 	
	
std::string strCurrentPortName = StripSpaces( myMIDIIn->getPortName( nPortID));
strCurrentPortName = StripNonAlphaNums( strCurrentPortName);
boost::replace_all(strCurrentPortName, strOne, strEmpty); 
#ifdef __WINDOWS__ 
// Remove trailing null
strCurrentPortName = strCurrentPortName.substr( 0, strCurrentPortName.length() -1);
#endif

if (strTempName.compare( strCurrentPortName) == 0)
	{
		delete myMIDIIn;
		return true;
		}
else
{
		delete myMIDIIn;
return false;
}
}  // end try
catch ( RtMidiError &error )
{
#ifdef __WINDOWS__
OutputDebugStringA(error.what());
#endif

		delete myMIDIIn;
return false;
}
}


// Validate output port
bool IsValidOutputPort( int nPortID, std::string strPortName)
{
// Test for a currently valid port ID
if (nPortID == VP_PORT_ID)
	{
		#ifdef __WINDOWS__ 
return false;
#else
		return true;
#endif
}

RtMidiOut * myMIDIOut = 0;

try
	{
myMIDIOut = new RtMidiOut;	

	if ((nPortID < 0) || (nPortID > (int) myMIDIOut->getPortCount())
		|| (myMIDIOut->getPortCount() == 0))
{
		delete myMIDIOut;
return false;
}
	else
		{
	delete myMIDIOut;
		return true;
}

// Create an array with the character, c, and a null terminator 
char buff[2] = { '\1', '\0' };
std::string strOne( buff);
std::string strEmpty( "");

std::string strTempName = StripSpaces( strPortName);
	strTempName = StripNonAlphaNums( strTempName);
boost::replace_all(strTempName, strOne, strEmpty); 	
	
	std::string strCurrentPortName = StripSpaces( myMIDIOut->getPortName( nPortID));
strCurrentPortName = StripNonAlphaNums( strCurrentPortName);
boost::replace_all(strCurrentPortName, strOne, strEmpty); 	
		
#ifdef __WINDOWS__ 
// Remove trailing null
strCurrentPortName = strCurrentPortName.substr( 0, strCurrentPortName.length() -1);
#endif

		if (strTempName.compare( strCurrentPortName) == 0)
{
		delete myMIDIOut;
		return true;
}
else
{
delete myMIDIOut;
return false;
}
}  // end try
	catch ( RtMidiError &error )
	{
#ifdef __WINDOWS__ 
 OutputDebugStringA( error.what());
					#endif

delete myMIDIOut;
return false;
		}
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
if (nInputID == VP_PORT_ID)
{
wxString wxstrName = wstrWidgetName;
	std::string strName = wxstrName.ToStdString();
		strName.append( strVirtualInSuffix);
m_DisplayMIDIIn->openVirtualPort( strName);
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
if (nOutputID == VP_PORT_ID)
{
	wxString wxstrName( wstrWidgetName);
	std::string strName = wxstrName.ToStdString();
	strName = strName.append( strVirtualOutSuffix);
m_HardwareMIDIOut->openVirtualPort( strName);
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
bool blnOutput, blnInput = true;

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
bool blnInput, blnOutput;

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