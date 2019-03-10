	// MIDISurface.cpp
// MIDISurface implementation


#include "MIDISurface.h"
#include "SurfaceFrame.h"
// #include "DispatchQueue.h"

MIDISurface::MIDISurface( SurfaceFrame * pParent, ActiveProduct apProduct, boost::shared_ptr<ProtocolCollection> pProtocols, boost::shared_ptr<AppCollection> pApps)
		:
MIDIWidget(),
							pMyProtocols( new ProtocolCollection()),
pMyApps( new AppCollection()),
blnAutoMap( false),
nReadDisplayDelay( MIN_READ_DISPLAY_DELAY),
nHardwareMessages( 0),
nDisplayMessages( 0)
{
pMyParent = pParent;
		MyProduct = apProduct;
	MyProduct = apProduct;
	pMyProtocols = pProtocols;
pMyApps = pApps;

vHardwareBuffer.clear();
vDisplayBuffer.clear();

// Generate a new surface ID 
						boost::uuids::uuid myUUID = boost::uuids::random_generator()(); 
	mySurfaceID = boost::lexical_cast <std::string> (myUUID); 	

// Logging
std::experimental::filesystem::path myPath = LogPath();
myPath /= mySurfaceID.append( strLogExtension);
boost::log::add_file_log( myPath.generic_string(), boost::log::keywords::rotation_size = 10 * 1024 * 1024, boost::log::keywords::time_based_rotation = boost::log::sinks::file::rotation_at_time_point(0, 0, 0), boost::log::keywords::format = "[%TimeStamp%]: %Message%"); //create a log file.
boost::log::add_common_attributes(); //add common attributes see documentation.
BOOST_LOG_SEV( lg, boost::log::trivial::info) << "Log created.";

// Specify the callbacks
						m_HardwareMIDIIn->setCallback( &HardwareCallback, (void*) this);
						m_DisplayMIDIIn->setCallback( &DisplayCallback, (void*) this);
}


MIDISurface::MIDISurface(SurfaceFrame * pParent, ActiveProduct apProduct, SurfaceParameters * myParameters, boost::shared_ptr<ProtocolCollection> pProtocols, boost::shared_ptr<AppCollection> pApps) 
	: 
						MIDIWidget(),
				pMyProtocols( new ProtocolCollection()),
pMyApps( new AppCollection()),
blnAutoMap( false),
			nReadDisplayDelay( MIN_READ_DISPLAY_DELAY),
			nHardwareMessages( 0),
nDisplayMessages( 0)
{
				pMyParent = pParent;
					MyProduct = apProduct;
	pMyProtocols = pProtocols;
pMyApps = pApps;

vHardwareBuffer.clear();
vDisplayBuffer.clear();

// Generate a new surface ID 
						boost::uuids::uuid myUUID = boost::uuids::random_generator()(); 
	mySurfaceID = boost::lexical_cast <std::string> (myUUID); 	

// Logging
std::experimental::filesystem::path myPath = LogPath();
myPath /= myParameters->GetSurfaceName().append( wstrLogExtension);
boost::log::add_file_log( myPath.generic_string(), boost::log::keywords::rotation_size = 10 * 1024 * 1024, boost::log::keywords::time_based_rotation = boost::log::sinks::file::rotation_at_time_point(0, 0, 0), boost::log::keywords::format = "[%TimeStamp%]: %Message%"); //create a log file.
boost::log::add_common_attributes(); //add common attributes see documentation.
BOOST_LOG_SEV( lg, boost::log::trivial::info) << "Log created.";
    
						SetSurfaceName( myParameters->GetSurfaceName());
						SetProtocolID(myParameters->GetProtocolID());
						SetProtocolName( myParameters->GetProtocolName());
												SetHardwareInID( myParameters->GetHardwareInID());
SetHardwareInName( myParameters->GetHardwareInName());
SetHardwareOutID(myParameters->GetHardwareOutID());
SetHardwareOutName(myParameters->GetHardwareOutName());
SetDisplayInID(myParameters->GetDisplayInID());
SetDisplayInName(myParameters->GetDisplayInName());
SetDisplayOutID(myParameters->GetDisplayOutID());
SetDisplayOutName(myParameters->GetDisplayOutName());
SetAppConfigID(myParameters->GetAppConfigID());
SetAppConfigName( myParameters->GetAppConfigName());
SetWidgetMode(myParameters->GetSurfaceMode());
SetReadDisplayDelay(myParameters->GetReadDisplayDelay());

// Specify the callbacks
						m_HardwareMIDIIn->setCallback( &HardwareCallback, (void*) this);
m_DisplayMIDIIn->setCallback( &DisplayCallback, (void*) this);
}


MIDISurface::MIDISurface(SurfaceFrame * pParent, ActiveProduct apProduct, std::wstring wstrName, boost::shared_ptr<ProtocolCollection> pProtocols, boost::shared_ptr<AppCollection> pApps) 
							:
MIDIWidget( wstrName),
				pMyProtocols( new ProtocolCollection()),
pMyApps( new AppCollection()),
blnAutoMap( false),
nReadDisplayDelay( MIN_READ_DISPLAY_DELAY),
nHardwareMessages( 0),
nDisplayMessages( 0)
{
				pMyParent = pParent;
	MyProduct = apProduct;
							pMyProtocols = pProtocols;
pMyApps = pApps;

vHardwareBuffer.clear();
vDisplayBuffer.clear();

// Generate a new surface ID 
						boost::uuids::uuid myUUID = boost::uuids::random_generator()(); 
	mySurfaceID = boost::lexical_cast <std::string> (myUUID); 	

// Logging
std::experimental::filesystem::path myPath = LogPath();
myPath /= wstrName.append( wstrLogExtension);
boost::log::add_file_log( myPath.generic_string(), boost::log::keywords::rotation_size = 10 * 1024 * 1024, boost::log::keywords::time_based_rotation = boost::log::sinks::file::rotation_at_time_point(0, 0, 0), boost::log::keywords::format = "[%TimeStamp%]: %Message%"); //create a log file.
boost::log::add_common_attributes(); //add common attributes see documentation.
BOOST_LOG_SEV( lg, boost::log::trivial::info) << "Log created.";
    
// Specify the callbacks
						m_HardwareMIDIIn->setCallback( &HardwareCallback, (void*) this);
m_DisplayMIDIIn->setCallback( &DisplayCallback, (void*) this);
}


	MIDISurface::MIDISurface(SurfaceFrame * pParent, ActiveProduct apProduct, std::wstring wstrName, std::string myNewProtocolID, boost::shared_ptr<ProtocolCollection> pProtocols, boost::shared_ptr<AppCollection> pApps) 
: 
	MIDIWidget( wstrName),
				pMyProtocols( new ProtocolCollection()),
pMyApps( new AppCollection()),
	blnAutoMap( false),
nReadDisplayDelay( MIN_READ_DISPLAY_DELAY),
nHardwareMessages( 0),
nDisplayMessages( 0)
{
pMyParent = pParent;
	MyProduct = apProduct;
	pMyProtocols = pProtocols;
pMyApps = pApps;

vHardwareBuffer.clear();
vDisplayBuffer.clear();

// Generate a new surface ID 
boost::uuids::uuid myUUID = boost::uuids::random_generator()(); 
	mySurfaceID = boost::lexical_cast <std::string> (myUUID); 	

// Logging
std::experimental::filesystem::path myPath = LogPath();
myPath /= wstrName.append( wstrLogExtension);
boost::log::add_file_log( myPath.generic_string(), boost::log::keywords::rotation_size = 10 * 1024 * 1024, boost::log::keywords::time_based_rotation = boost::log::sinks::file::rotation_at_time_point(0, 0, 0), boost::log::keywords::format = "[%TimeStamp%]: %Message%"); //create a log file.
boost::log::add_common_attributes(); //add common attributes see documentation.
BOOST_LOG_SEV( lg, boost::log::trivial::info) << "Log created.";
    
						// Link to the map of protocols
						myProtocolID =myNewProtocolID;
						SetProtocolName( pMyProtocols->GetProtocol( myProtocolID).GetProtocolName());

						m_HardwareMIDIIn->setCallback( &HardwareCallback, (void*) this);
m_DisplayMIDIIn->setCallback( &DisplayCallback, (void*) this);
}

	
MIDISurface::MIDISurface( SurfaceFrame * pParent, ActiveProduct apProduct, std::wstring wstrName, std::string strNewProtocolID, int nHWIn, int nHWOut, int nDisplayIn, int nDisplayOut, boost::shared_ptr<ProtocolCollection> pProtocols, boost::shared_ptr<AppCollection> pApps)
				        : 
MIDIWidget(wstrName, nHWIn, nHWOut, nDisplayIn, nDisplayOut),
								pMyProtocols( new ProtocolCollection()),
pMyApps( new AppCollection()),
blnAutoMap( false),
nReadDisplayDelay( MIN_READ_DISPLAY_DELAY),
nHardwareMessages( 0),
nDisplayMessages( 0)
{
		pMyParent = pParent;
					MyProduct = apProduct;
	pMyProtocols = pProtocols;
pMyApps = pApps;

vHardwareBuffer.clear();
vDisplayBuffer.clear();

				// Generate a new surface ID 
boost::uuids::uuid myUUID = boost::uuids::random_generator()(); 
	mySurfaceID = boost::lexical_cast <std::string> (myUUID); 	

// Logging
std::experimental::filesystem::path myPath = LogPath();
myPath /= wstrName.append( wstrLogExtension);
boost::log::add_file_log( myPath.generic_string(), boost::log::keywords::rotation_size = 10 * 1024 * 1024, boost::log::keywords::time_based_rotation = boost::log::sinks::file::rotation_at_time_point(0, 0, 0), boost::log::keywords::format = "[%TimeStamp%]: %Message%"); //create a log file.
boost::log::add_common_attributes(); //add common attributes see documentation.
BOOST_LOG_SEV( lg, boost::log::trivial::info) << "Log created.";
    
						// Link to the map of protocols
						myProtocolID =strNewProtocolID;
						SetProtocolName( pMyProtocols->GetProtocol( myProtocolID).GetProtocolName());
						
						m_HardwareMIDIIn->setCallback( &HardwareCallback, (void*) this);
m_DisplayMIDIIn->setCallback( &DisplayCallback, (void*) this);
}


MIDISurface::~MIDISurface()
{
CloseHardwareThru();
CloseDisplayThru();

StopDisplayProcessing();
	StopHardwareProcessing();

// Dereference the callbacks
						m_HardwareMIDIIn->setCallback( NULL);
m_DisplayMIDIIn->setCallback( NULL);
}


void MIDISurface::FlushLog()
{
}


std::wstring MIDISurface::GetSurfaceName()
{
	return wstrWidgetName;
}

	
void MIDISurface::SetSurfaceName( std::wstring wstrName)
{
	wstrWidgetName = wstrName;
}


std::string MIDISurface::GetSurfaceID()
		{ 
			return mySurfaceID;
	}
	
	std::string MIDISurface::GetProtocolID()
	{
		return myProtocolID;
	}


	void MIDISurface::SetProtocolID( std::string myNewProtocolID)
	{
		myProtocolID = myNewProtocolID;
	}


std::pair <std::wstring, std::wstring> MIDISurface::GetAppConfigID()
{
	return myAppConfigID;
	}


void MIDISurface::SetAppConfigID( std::pair <std::wstring, std::wstring> myNewAppConfigID)
{
myAppConfigID = myNewAppConfigID;
}


void MIDISurface::AnalyseDisplayMessage(std::vector <unsigned char> myMessage)
{
SurfaceProtocol myProtocol; 
bool blnFound = false;

// Reset the buffer and message count if the new message is a system exclusive
if (myMessage.at( 0) == MIDI_SYSEX)
	{
		vDisplayBuffer.clear();
		nDisplayMessages = 0;
}

// Get the protocol being used by this surface
			try
		{
			 myProtocol = pMyProtocols->GetProtocol( GetProtocolID());
			 LogIt(myProtocol.GetProtocolName());
			 }
		catch( int nError)
		{
if (nError == ID_NO_MATCHING_PROTOCOL_ID)
{
	LogIt(L"Protocol not identified");
	nDisplayMessages = 0;
	vDisplayBuffer.clear();
	return;
}
else
{
	// Unknown error
	LogIt(L"Unknown exception in protocol detection.");
	nDisplayMessages = 0;
	vDisplayBuffer.clear();
return;
}
		}  // end catch

AppConfig myAppConfig;
	std::pair <std::wstring, std::wstring> myAppConfigID = GetAppConfigID();

	// Try to load the identified configuration
	try
		{
myAppConfig = pMyApps->GetAppConfig( myAppConfigID.first, myAppConfigID.second);
LogIt(myAppConfig.GetAppName());
	}
catch ( RBException &myException)
	    {
	LogIt(L"No App Config identified");
			LogIt(myException.what());
			std::wstring wstrAppName;
		wstrAppName.clear();
		myAppConfig.SetAppName( wstrAppName);
}  // end catch

		if (myAppConfig.GetAppName().empty())  // Default behaviour - no further processing necessary
	{
		nDisplayMessages = 0;
	vDisplayBuffer.clear();
return;
}

		// Add the new message to the buffer
bool blnDropStatus = myProtocol.GetDropExtraStatusBytes();
unsigned int nStart = 0;

if ((vDisplayBuffer.size() >= SHORT_MIDI_MESSAGE_LENGTH)  // Never drop the status byte for the first buffered message and leave sysex messages alone
&& (vDisplayBuffer.at( 0) != MIDI_SYSEX))
{
if (blnDropStatus)   // Don't add the status byte to the buffer
{
nStart++;
}
}

for (unsigned int i = nStart; i < myMessage.size(); i++)
{
	vDisplayBuffer.push_back( myMessage.at( i));
}

// Update how many messages we've processed since the last reset
nDisplayMessages++;

// Attempt to identify the display
std::pair <DisplayDefinition, std::string> myDisplayPair; 
	
try
	{
myDisplayPair = IdentifyDisplay( vDisplayBuffer, &myAppConfig);
blnFound = true;
}
catch( ...)
{
	LogIt(L"Display not identified");
}

								if (blnFound)
									{
// Cope with linked displays (i.e. multiple messages to refer to a single physical display
std::string strLinkedHash = myDisplayPair.first.GetLinkedDisplayHash();										
										
if (strLinkedHash.empty() == false)
										{
											// This ensures that we update the correct internal representation
											myDisplayPair.first.SetLabel( myProtocol.GetDisplay( strLinkedHash).GetLabel());										
}

// Log it
std::wstring wstrDisplayInfo = L"Display identified: ";
wstrDisplayInfo.append( myDisplayPair.first.GetLabel());
	LogIt( wstrDisplayInfo);

// Now try to update the display
										try
					{
						UpdateDisplay( myDisplayPair, vDisplayBuffer, &myProtocol);  
				}
				catch( ...)
				{
					LogIt(L"Updating of the display failed.");
				}

				// Take appropriate actions
		switch (GetWidgetMode())
			{
						case ID_LIVE_MODE:
							{}
							break;

		case ID_CONFIG_MODE:
					{
		std::wstring wstrOut = L"Display ";

										// Get the display state, if it's a LED lamp
										if (myDisplayPair.first.IsLEDLamp())
										{
SurfaceAction myAction;
unsigned int nDisplayState = 0;
MIDI myMIDI( vDisplayBuffer, false);
std::string strHash = myDisplayPair.second;

if (strHash.substr( 0, 1).compare( strNibbleHashPrefix) == 0)
{
	nDisplayState = myMIDI.NibbledMessageValue( strHash, vDisplayBuffer);
}
else
	{
		if ( vDisplayBuffer.size() == SHORT_MIDI_MESSAGE_LENGTH)
			{
				nDisplayState = myMIDI.ShortMsgData2( vDisplayBuffer);
		}
}

wstrOut.append( wstrSpace);
myAction.SetSurfaceActionType( ReadDisplay);
std::wstring wstrHash( strHash.begin(), strHash.end());
myAction.SetParameter( Type_String, wstrHash);
wstrOut.append( GetDisplayString( myAction, &myAppConfig, nDisplayState));	
}
										
										else   // Not a LED lamp
										{
// Get the display name
if (myAppConfig.GetAppName().empty() == false)  // Active app config
	{
// Check to see if this display has an entry in the active app config with a different label
										try
										{
											wstrOut.append( myAppConfig.GetDisplayDefinition( myDisplayPair.second).GetLabel());
											}
										catch( ...)
										{
										wstrOut.append( myDisplayPair.first.GetLabel());
						}
}

										wstrOut.append( L" updated at position ");
																				std::pair <int, int> myCursorPair;
										myCursorPair = GetCursorInfo( myDisplayPair.second, myMessage, &myDisplayPair.first, &myProtocol);
						wstrOut.append( boost::lexical_cast <std::wstring> (myCursorPair.first));
wstrOut.append( L" text = ");
wstrOut.append( wstrDoubleQuotes);
std::wstring wstrNewText = DecodeText( myMessage, &myProtocol, &myDisplayPair.first);
wstrOut.append( wstrNewText);
wstrOut.append( wstrDoubleQuotes);
wstrOut.append( L" length = ");
wstrOut.append( boost::lexical_cast <std::wstring> (wstrNewText.length()));
										}  // end if not LED
										
										// Announce the information
										Speak( wstrOut, false);
}
break;

		default:
			{}
		break;
				}  // end mode switch
								}  // end if blnFound

// Do we need to reset the buffer?
unsigned int nMaxSize = myProtocol.GetMessageBufferSize();

if ((nDisplayMessages >= nMaxSize)
			|| (blnFound)
|| ((vDisplayBuffer.at( 0) == MIDI_SYSEX) && (vDisplayBuffer.back() == MIDI_EOX)))
						{
			vDisplayBuffer.clear();
			nDisplayMessages = 0;
		}
}

	
	std::wstring MIDISurface::FormatDisplay( const std::wstring &wstrDisplayText, const int &nDisplayLength, const int &nLineCount)
	{
std::wstring wstrOut;
int nTempDisplayLength = nDisplayLength;

// Garbage values, so don't perform any modification
if ((nDisplayLength < 1)
	|| (nLineCount < 1))
	{
		return wstrDisplayText;
}

// Don't need to modify anything for single-line displays
if (nLineCount == 1)
	{
		return wstrDisplayText;;
}

// Are there any padding characters to hold line feeds?
if ((nDisplayLength % nLineCount) > 0)
{
nTempDisplayLength++;
}

int nLineLength = nTempDisplayLength / nLineCount;
int nStart = 0;

for (int nLineNumber = 1; nLineNumber <= nLineCount; nLineNumber++)
{
	if (nLineNumber < nLineCount)
		{
			wstringstream msg;
			msg << wstrDisplayText.substr( nStart, nLineLength) << endl;
		wstrOut.append( msg.str());		
	}
	else
		{
wstrOut.append( wstrDisplayText.substr( nStart, nLineLength));
				}

	nStart = nStart + nLineLength;
}
return wstrOut;
	}


void MIDISurface::AnalyseHardwareMessage(std::vector <unsigned char> myMessage)
{
SurfaceProtocol myProtocol; 
bool blnFound = false;

// Get the protocol being used by this surface
			try
		{
			 myProtocol = pMyProtocols->GetProtocol( GetProtocolID());
}
		catch( int nError)
		{
if (nError == ID_NO_MATCHING_PROTOCOL_ID)
{
	nHardwareMessages = 0;
	vHardwareBuffer.clear();
	return;
}
else
{
	// Unknown error
		nHardwareMessages = 0;
	vHardwareBuffer.clear();
return;
}
		}  // end catch

		AppConfig myAppConfig;
		try
		{
std::pair <std::wstring, std::wstring> myAppConfigID = GetAppConfigID();

	// Try to load the identified configuration
myAppConfig = pMyApps->GetAppConfig( myAppConfigID.first, myAppConfigID.second);
}
catch ( RBException &myException)
	    {
        					#ifdef __WINDOWS__ 
OutputDebugString( myException.what());
					#endif

std::wstring wstrAppName;
		wstrAppName.clear();
		myAppConfig.SetAppName( wstrAppName);
}  // end catch

	// Add the new message to the buffer
bool blnDropStatus = myProtocol.GetDropExtraStatusBytes();
unsigned int nStart = 0;

if (vHardwareBuffer.size() >= SHORT_MIDI_MESSAGE_LENGTH)  // Never drop the status byte for the first buffered message
{
if (blnDropStatus)   // Don't add the status byte to the buffer
{
nStart++;
}
}

for (unsigned int i = nStart; i < myMessage.size(); i++)
{
	vHardwareBuffer.push_back( myMessage.at( i));
}

// Update how many messages we've processed since the last reset
nHardwareMessages++;

		switch( GetWidgetMode())
	{
case Mode( ID_LIVE_MODE): //process message in live mode.
		{
			try
				{
std::pair <MessageDefinition, std::string> myHardwarePair;
myHardwarePair = IdentifyHardwareControl( vHardwareBuffer, &myProtocol);

if (myHardwarePair.second.empty())
{}
else
{
blnFound = true;

// Log it
std::wstring wstrHardwareInfo = L"Hardware identified: ";
wstrHardwareInfo.append( myHardwarePair.first.GetLabel());
LogIt( wstrHardwareInfo);
	
// Calculate the value/state of the control
unsigned int nControlState = 0;
MIDI myMIDI( vHardwareBuffer, false);

if (myHardwarePair.second.substr( 0, 1).compare( strNibbleHashPrefix) == 0)
{
	nControlState = myMIDI.NibbledMessageValue( myHardwarePair.second, vHardwareBuffer);
}
else
	{
		if (vHardwareBuffer.size() == SHORT_MIDI_MESSAGE_LENGTH)
			{
				nControlState = myMIDI.ShortMsgData2( vHardwareBuffer);
		}
}

ProcessLiveModeMessage( myHardwarePair, &myProtocol, &myAppConfig, nControlState, vHardwareBuffer);
}  // end if empty hash
			}
			catch ( unsigned int nException)
			{
if (nException == RB_NO_CONTROL_DEFINITION)
{}
else
{}
			}  // end catch
					}  // end case live mode
				break;

	case Mode( ID_ID_MODE): //Process message in ID mode.
{
try
	{
		std::pair <MessageDefinition, std::string> myHardwarePair;
myHardwarePair = IdentifyHardwareControl( vHardwareBuffer, &myProtocol);

if (myHardwarePair.second.empty())
{}
else
{
blnFound = true;
// Calculate the value/state of the control
unsigned int nControlState = 0;
MIDI myMIDI( vHardwareBuffer, false);
std::string strHash = myHardwarePair.second;

if (strHash.substr( 0, 1).compare( strNibbleHashPrefix) == 0)
{
	nControlState = myMIDI.NibbledMessageValue( strHash, vHardwareBuffer);
}
else
	{
		if ( vHardwareBuffer.size() == SHORT_MIDI_MESSAGE_LENGTH)
			{
				nControlState = myMIDI.ShortMsgData2( vHardwareBuffer);
		}
}

ProcessLiveModeMessage( myHardwarePair, &myProtocol, &myAppConfig, nControlState, vHardwareBuffer);
}  // end if empty hash
			}
			catch ( unsigned int nException)
			{
if (nException == RB_NO_CONTROL_DEFINITION)
{}
else
{}
			}  // end catch
}  // end case ID Mode
break;

	case Mode( ID_CONFIG_MODE):
{
std::wstring wstrOut;

try
				{
std::pair <MessageDefinition, std::string> myHardwarePair;
myHardwarePair = IdentifyHardwareControl( vHardwareBuffer, &myProtocol);

if (myHardwarePair.second.empty())
{}
else
{
blnFound = true;
// Calculate the value/state of the control
unsigned int nControlState = 0;
MIDI myMIDI( vHardwareBuffer, false);
std::string strHash = myHardwarePair.second;

if (strHash.substr( 0, 1).compare( strNibbleHashPrefix) == 0)
{
	nControlState = myMIDI.NibbledMessageValue( strHash, vHardwareBuffer);
}
else
	{
		if (vHardwareBuffer.size() == SHORT_MIDI_MESSAGE_LENGTH) 
			{
				nControlState = myMIDI.ShortMsgData2( vHardwareBuffer);
		}
}

// ProcessIDModeMessage( myHardwarePair.second, myHardwarePair.first, &myProtocol, &myAppConfig, nControlState, vHardwareBuffer);
wstrOut = myHardwarePair.first.GetLabel();
wstrOut.append( L" value equals ");
wstrOut.append( boost::lexical_cast <std::wstring> (nControlState));
Speak( wstrOut, false);
}  // end if empty hash
			}
			catch ( unsigned int nException)
			{
if (nException == RB_NO_CONTROL_DEFINITION)
{
Speak( wstrUnknownStatus, false);
}
else
{
Speak( wstrUnknownStatus, false);
}
			}  // end catch
}  // end case config mode
break;
	
	case Mode( ID_SCREEN_READER_MODE): //Process message in screen reader mode.
		{
			try
				{
std::pair <MessageDefinition, std::string> myHardwarePair;
myHardwarePair = IdentifyHardwareControl( vHardwareBuffer, &myProtocol);

if (myHardwarePair.second.empty())
{}
else
{
blnFound = true;
// Calculate the value/state of the control
unsigned int nControlState = 0;
MIDI myMIDI( vHardwareBuffer, false);
std::string strHash = myHardwarePair.second;

if (strHash.substr( 0, 1).compare( strNibbleHashPrefix) == 0)
{
	nControlState = myMIDI.NibbledMessageValue( strHash, vHardwareBuffer);
}
else
	{
		if (vHardwareBuffer.size() == SHORT_MIDI_MESSAGE_LENGTH)
			{
				nControlState = myMIDI.ShortMsgData2( vHardwareBuffer);
		}
}

ProcessScreenReaderModeMessage( myHardwarePair.second, myHardwarePair.first, &myProtocol, &myAppConfig, nControlState, vHardwareBuffer);
}  // end if empty hash
			}
			catch ( unsigned int nException)
			{
if (nException == RB_NO_CONTROL_DEFINITION)
{}
else
{}
			}  // end catch
					}  // end case Screen reader mode
		break;
	
	default:
		{
		blnFound = true;
}
		break;
	};   // end switch
	
// Do we need to reset the buffer?
unsigned int nMaxSize = myProtocol.GetMessageBufferSize();

if ((nHardwareMessages >= nMaxSize)
			|| (blnFound))
			{
			vHardwareBuffer.clear();
			nHardwareMessages = 0;
		}
}


std::wstring MIDISurface::DecodeText( std::vector <unsigned char> vBytes, SurfaceProtocol * pProtocol, DisplayDefinition * pDisplay)
{
	std::wstring wstrOut;
std::vector <unsigned char> vUniCode;
	
	if (pDisplay->GetSysExAddressBytes().size() > 0)
	{
		// Display addressed using a system exclusive message
		unsigned int nHeaderLength = pProtocol->GetSysExHeader().size();
unsigned int nAddressBytes = pDisplay->GetSysExAddressBytes().size();
// Add an offset to take the cursor location byte into account
nHeaderLength = nHeaderLength + nAddressBytes +1;

for (unsigned int i = nHeaderLength; (vBytes.at( i) != 0xf7); i++)
{
if (pDisplay->IsUniCode())
		{
	vUniCode.push_back( vBytes.at( i)); 
}
	else
		{
if (vBytes.at( i) != 0)
	{
// Only add non-null characters
		wstrOut.push_back( vBytes.at( i)); 
}
	}  // end if Unicode
}  // end for
		
	if (pDisplay->IsUniCode())
	{
LogIt( "Calling UniCoded");
wstrOut = UniCoded( vUniCode);
		LogIt( "UniCoded complete");
	}
		}  // end if SysEx display
else  
	{// Not SysEx, so assume nibble-based
		wstrOut.push_back( vBytes.at( (vBytes.size() -1)));
}  // end nibble-based display
			
return wstrOut;
}


std::wstring MIDISurface::GetDisplayText( std::wstring wstrLabel)
{
std::map <std::wstring, std::wstring>::iterator it;
it = myDisplays.find( wstrLabel);

if (it == myDisplays.end())
{
std::wstring wstrOut;
wstrOut.clear();
return wstrOut;
}
else
{
	return it->second;
}
}


void MIDISurface::SetDisplayText( std::wstring wstrLabel, std::wstring wstrText, bool blnCursorFromLeft, bool blnLEDLamp, int nDisplayLength, int nLineCount, SurfaceProtocol * pProtocol, std::string strTranslationID)
{
if (blnCursorFromLeft == false)
	{
		std::reverse( wstrText.begin(), wstrText.end());
}
	
std::map <std::wstring, std::wstring>::iterator it;
it = myDisplays.find( wstrLabel);

if (it == myDisplays.end())
{
std::pair <std::wstring, std::wstring> myPair = std::make_pair( wstrLabel, wstrText);
myDisplays.insert( myPair);
UpdateUI( myPair.first, blnLEDLamp, nDisplayLength, nLineCount, pProtocol, strTranslationID);
}
else
		{
			it->second = wstrText;
UpdateUI( it->first, blnLEDLamp, nDisplayLength, nLineCount, pProtocol, strTranslationID);
}
}


void MIDISurface::SetDisplayText( std::wstring wstrLabel, int nCursorPosition, std::wstring wstrNewText, bool blnCursorFromLeft, bool blnLEDLamp, int nDisplayLength, int nLineCount, SurfaceProtocol * pProtocol, std::string strTranslationID)
{
std::map <std::wstring, std::wstring>::iterator it;
it = myDisplays.find( wstrLabel);

if (it != myDisplays.end())
{
if (blnCursorFromLeft == false)
	{
std::reverse( wstrNewText.begin(), wstrNewText.end());
nCursorPosition = ((it->second.length() -1) - nCursorPosition);
}

it->second.replace( nCursorPosition , wstrNewText.length(), wstrNewText);	
UpdateUI( it->first, blnLEDLamp, nDisplayLength, nLineCount, pProtocol, strTranslationID);
}
}


bool MIDISurface::IsAutoMapDevice()
{
	return blnAutoMap;
}


void MIDISurface::SetAutoMap( bool blnNovation)
{
	blnAutoMap = blnNovation;
}


bool MIDISurface::IsNoteMessage( std::string strHash)
{
	if (strHash.substr( 0, 3).compare( strNoteOn) == 0) 
	{
		return true;
}
else
	{
		return false;
}
}
			

bool MIDISurface::IsNoteOffMessage( std::string strHash, int nDataValue)
{
if ((IsNoteMessage( strHash)) 
	&& (nDataValue == 0))
{
	return true;
}
else
{
	return false;
}
}


std::wstring MIDISurface::GetActionString( SurfaceAction myAction, MessageDefinition myMessageDefinition, std::string strHash, int nControlState, SurfaceProtocol * pProtocol, AppConfig * pAppConfig, std::vector <unsigned char> vBuffer)
		{
switch ( myAction.GetSurfaceActionType())
{
case NoAction:
	{
		return wstrEmpty;
	}
	break;

case ReadControlLabel:
{
	if (IsNoteMessage( strHash)) 
{
	if (IsNoteOffMessage( strHash, nControlState))
{
return wstrEmpty;
}
else  // note on
{
	return GetControlLabelString( &myMessageDefinition, strHash, nControlState, pAppConfig);
}
}
else  // other than note on/off
{
	if (nControlState > MIDI_NOTE_OFF_VELOCITY)   // 0 usually used to indicate "off"
		{
			return GetControlLabelString( &myMessageDefinition, strHash, nControlState, pAppConfig);
	}
	else  // "on"
{
return wstrEmpty;
	}
	}
}
	break;

case ReadControlState:
{
	if (IsNoteMessage( strHash)) 
{
	if (IsNoteOffMessage( strHash, nControlState))
{
return wstrEmpty;
}
else  // note on
{
return GetControlStateString( &myMessageDefinition, nControlState, pAppConfig);
}
}

else  // other than note on/off

{
	
if (nControlState > MIDI_NOTE_OFF_VELOCITY)   // 0 usually used to indicate "off"
		
{

return GetControlStateString( &myMessageDefinition, nControlState, pAppConfig);
}
	else  // "on"
{
return wstrEmpty;
	}
	}
}
	break;

case ReadControlLabelPlusState:
{
if (IsNoteMessage( strHash)) 
{
	if (IsNoteOffMessage( strHash, nControlState))
{
return wstrEmpty;
}
else  // note on
{
return GetControlLabelPlusStateString( &myMessageDefinition, strHash, nControlState, pAppConfig);
}
}
else  // other than note on/off
{
	if (nControlState > MIDI_NOTE_OFF_VELOCITY)   // 0 usually used to indicate "off"		
{
return GetControlLabelPlusStateString( &myMessageDefinition, strHash, nControlState, pAppConfig);
}
	else  // "on"
{
return wstrEmpty;
	}
	}
}
break;

case ReadDisplay:
	{
		if (IsNoteMessage( strHash)) 
{
	if (IsNoteOffMessage( strHash, nControlState))
{
int nDelay = GetReadDisplayDelay();

if (nDelay > 0)
	{
		// Pause to allow time for the display to update
	Sleep( nDelay);
}
	
return GetDisplayString( myAction, pAppConfig, nControlState);	
		}
else
{
// Note on, so ignore as the display might not yet be updated
return wstrEmpty;
}
	}
else  // any other kind of message
{
if (nControlState == MIDI_NOTE_OFF_VELOCITY)   // 0 usually used to indicate "off", so effectively read on release
		{
	int nDelay = GetReadDisplayDelay();

if (nDelay > 0)
	{
		// Pause to allow time for the display to update
	Sleep( nDelay);
}
MIDI myMIDI( vBuffer, false);
int nDataValue = myMIDI.NibbledMessageValue( strHash, vBuffer);
return GetDisplayString( myAction, pAppConfig, nDataValue);	
		}
		else
		{
return wstrEmpty;
}  // end non-note message Tvalue > 0
		}  // end if note message
	}
 	break;


case ReadDisplayLine:
	{
		if (IsNoteMessage( strHash)) 
{
	if (IsNoteOffMessage( strHash, nControlState))
{
				int nDelay = GetReadDisplayDelay();

if (nDelay > 0)
	{
		// Pause to allow time for the display to update
	Sleep( nDelay);
}

return GetDisplayLineString( myAction, pAppConfig);
}
else  // ignore note on
{
return wstrEmpty;
}
}
else  //  any other message type
{
	int nDelay = GetReadDisplayDelay();

if (nDelay > 0)
	{
		// Pause to allow time for the display to update
	Sleep( nDelay);
}

return GetDisplayLineString( myAction, pAppConfig);
}
}
	break;

case ReadDisplayStrip:
	{
		if (IsNoteMessage( strHash))
			{
				if (IsNoteOffMessage( strHash, nControlState))
{
		int nDelay = GetReadDisplayDelay();

if (nDelay > 0)
	{
		// Pause to allow time for the display to update
	Sleep( nDelay);
}

return GetDisplayStripString( myAction, pAppConfig);
		}
				else  // ignore note on
		{
				return wstrEmpty;
				}
		}
		else  // any other message type
		{
		int nDelay = GetReadDisplayDelay();

if (nDelay > 0)
	{
		// Pause to allow time for the display to update
	Sleep( nDelay);
}

return GetDisplayStripString( myAction, pAppConfig);
		}
		}
	break;

case ReadDisplaySection:
	{
		if (IsNoteMessage( strHash))
			{
				if (IsNoteOffMessage( strHash, nControlState))
{
		int nDelay = GetReadDisplayDelay();

if (nDelay > 0)
	{
		// Pause to allow time for the display to update
	Sleep( nDelay);
}

return GetDisplaySubstring( myAction, pAppConfig);
		}
		else  // ignore note on
		{
			return wstrEmpty;
		}
		}
		else  // any other message type
			{
				int nDelay = GetReadDisplayDelay();

if (nDelay > 0)
	{
		// Pause to allow time for the display to update
	Sleep( nDelay);
}

return GetDisplaySubstring( myAction, pAppConfig);
		}
	}
	break;

case MIDIInfo:
{
	if (IsNoteMessage( strHash)) 
{
	if (IsNoteOffMessage( strHash, nControlState))
{
return wstrEmpty;
}
else  // note on

{
return GetMIDIInfoString( pProtocol->IsEncrypted(), myAction, pAppConfig, vBuffer);
}
}
else  // other than note on/off
{
	if (nControlState > MIDI_NOTE_OFF_VELOCITY)   // 0 usually used to indicate "off"
		{
return GetMIDIInfoString( pProtocol->IsEncrypted(), myAction, pAppConfig, vBuffer);
}
	else  // "on"
{
return wstrEmpty;
	}
	}
}
	break;

case ChangeSurfaceMode:
{
if (IsNoteMessage( strHash)) 
{
	if (IsNoteOffMessage( strHash, nControlState))
{
return wstrEmpty;
}
else  // note on
{
ProcessChangeSurfaceMode( myAction);
return wstrEmpty;
}
}
else  // other than note on/off
{
if (nControlState > MIDI_NOTE_OFF_VELOCITY)   // 0 usually used to indicate "off"
		{
ProcessChangeSurfaceMode( myAction);
return wstrEmpty;
}
	else  // "on"
{
return wstrEmpty;
	}
	}
}
break;

case RunHotSpot:
{
if (IsNoteMessage( strHash)) 
{
	if (IsNoteOffMessage( strHash, nControlState))
{
return wstrEmpty;
}
else  // note on
{
ProcessRunHotSpot( myAction, pAppConfig);
return wstrEmpty;
}
}
else  // other than note on/off
{
	if (nControlState > MIDI_NOTE_OFF_VELOCITY)   // 0 usually used to indicate "off"
		{
ProcessRunHotSpot( myAction, pAppConfig);
return wstrEmpty;
}
	else  // "on"
{
return wstrEmpty;
	}
	}
}
break;

case RunScreenReaderFunction:
{
if (IsNoteMessage( strHash)) 
{
	if (IsNoteOffMessage( strHash, nControlState))
{
return wstrEmpty;
}
else  // note on

{
ProcessRunScreenReaderFunction( myAction, pAppConfig);
return wstrEmpty;
}
}
else  // other than note on/off
{
	if (nControlState > MIDI_NOTE_OFF_VELOCITY)   // 0 usually used to indicate "off"
		{
ProcessRunScreenReaderFunction( myAction, pAppConfig);
return wstrEmpty;
}
	else  // "on"
{
return wstrEmpty;
	}
	}
}
	break;

case SpeakString:
{
if (IsNoteMessage( strHash)) 
{
	if (IsNoteOffMessage( strHash, nControlState))
{
return wstrEmpty;
}
else
{
return GetSpeakString( myAction, pAppConfig);
	}
}
else
{
return GetSpeakString( myAction, pAppConfig);
	}
}
break;

case ReadLEDIfOn:
case ReadLEDIfOff:
	{
		if (IsNoteMessage( strHash)) 
{
	if (IsNoteOffMessage( strHash, nControlState))
{
int nDelay = GetReadDisplayDelay();

if (nDelay > 0)
	{
		// Pause to allow time for the display to update
	Sleep( nDelay);
}
	
return GetLEDString( myAction, pAppConfig, nControlState);	
		}
else
{
// Note on, so ignore as the display might not yet be updated
return wstrEmpty;
}
	}
else  // any other kind of message
{
if (nControlState == MIDI_NOTE_OFF_VELOCITY)   // 0 usually used to indicate "off", so effectively read on release
		{
	int nDelay = GetReadDisplayDelay();

if (nDelay > 0)
	{
		// Pause to allow time for the display to update
	Sleep( nDelay);
}
MIDI myMIDI( vBuffer, false);
int nDataValue = myMIDI.NibbledMessageValue( strHash, vBuffer);
return GetLEDString( myAction, pAppConfig, nControlState);	
		}
		else
		{
return wstrEmpty;
}  // end non-note message Tvalue > 0
		}  // end if note message
	}
 	break;


default:
{
return wstrEmpty;
}
break;
}  // end switch
	}


std::wstring MIDISurface::GetControlLabelString( MessageDefinition * myDefinition, std::string strHash, int nDataValue, AppConfig * pAppConfig)
{
	// Is this a note off?
	if (IsNoteOffMessage( strHash, nDataValue))
		{
return wstrEmpty;
}
		else  // note on (button press), so announce label
{
return myDefinition->GetLabel();
}  // end if not off
}


std::wstring MIDISurface::GetControlStateString( MessageDefinition * myDefinition, int nDataValue, AppConfig * pAppConfig)
{
std::vector <unsigned char> vBytes;
	vBytes.push_back( (unsigned char) nDataValue);
	return myDefinition->GetState( vBytes);
}


std::wstring MIDISurface::GetControlLabelPlusStateString( MessageDefinition * myDefinition, std::string strHash, int nDataValue, AppConfig * pAppConfig)
{
				if ((IsNoteMessage( strHash))
					&& (IsNoteOffMessage( strHash, nDataValue)))
		{
return wstrEmpty;
}

std::wstring wstrOut = myDefinition->GetLabel();

if (wstrOut.empty() == false)
	{
		wstrOut.append( wstrSpace);
		
		// State value
		std::vector <unsigned char> vBytes;
		vBytes.push_back( (unsigned char) nDataValue);
		wstrOut.append( myDefinition->GetState( vBytes));
}  // end if empty string

		return wstrOut;
}


std::wstring MIDISurface::GetLEDString( std::string strHash, DisplayDefinition * pDisplay, SurfaceProtocol * pProtocol, int nDataValue)
{
std::wstring wstrOut = pDisplay->GetLabel();
		wstrOut.append( wstrSpace);

		if (pProtocol->IsUsingGlobalStates())
{
std::vector <unsigned char> vState;
vState.push_back( (unsigned char) nDataValue);
wstrOut.append( pProtocol->GetDisplayItem( 0).GetState( vState));
}
		else
			{
std::vector <unsigned char> vValues;
vValues.push_back( (unsigned char) nDataValue);
				wstrOut.append( pProtocol->GetDisplay( strHash).GetState( vValues));
		}  // end if using global state labels
	
	return wstrOut;
}


std::wstring MIDISurface::GetDisplayString( SurfaceAction myAction, AppConfig * pAppConfig, int nDataValue)
{
	// Get the protocol information so that we can decode correctly 
std::string strProtocolID = GetProtocolID();
SurfaceProtocol myProtocol = pMyProtocols->GetProtocol( strProtocolID);

const int DISPLAY_NAME_PARAM = 0;
std::string strDisplayHash = wxString( myAction.GetParameter(DISPLAY_NAME_PARAM).second).ToStdString();
DisplayDefinition myDisplay;

try
	{
		myDisplay = myProtocol.GetDisplay( strDisplayHash);
}
catch( ...)
	{
		return wstrEmpty;
}

std::wstring wstrDisplayName = myDisplay.GetLabel();
std::wstring wstrOut;

if (myDisplay.IsLEDLamp())
{
std::wstring wstrText = GetDisplayText( wstrDisplayName);

if (wstrText.empty() == false)
{
	wstrOut = wstrDisplayName;
wstrOut.append( wstrSpace);
wstrOut.append( wstrText);
								}

				return wstrOut;
				}
				else  // text display
				{
					std::string strTranslationID = myDisplay.GetCharacterTranslationID();
std::wstring wstrDisplayText = GetDisplayText( wstrDisplayName);

if (wstrDisplayText.empty())
{
	return wstrEmpty;
}
else
{
		wstrOut.append( myProtocol.GetTranslatedString( strTranslationID, wstrDisplayText));	
	return wstrOut;
}  // end if empty wstrDisplay 
				}  // end if LED
}


std::wstring MIDISurface::GetDisplayLineString( SurfaceAction myAction, AppConfig * pAppConfig)
{
// Get the protocol information so that we can decode correctly 
std::string strProtocolID = this->GetProtocolID();
SurfaceProtocol myProtocol = pMyProtocols->GetProtocol( strProtocolID);
const int DISPLAY_NAME_PARAM = 0;
std::string strDisplayHash = wxString( myAction.GetParameter(DISPLAY_NAME_PARAM).second).ToStdString();
		
const int DISPLAY_LINE_PARAM = 1;
std::wstring wstrDisplayLine = myAction.GetParameter( DISPLAY_LINE_PARAM).second;
		long nDisplayLine = boost::lexical_cast <long> (wstrDisplayLine);

int nLength = myProtocol.GetDisplay( strDisplayHash).GetDisplayLength();
int nLines = myProtocol.GetDisplay( strDisplayHash).GetLineCount();
std::wstring wstrDisplayName = myProtocol.GetDisplay( strDisplayHash).GetLabel();
std::wstring wstrOut; 
std::wstring wstrDisplay = GetDisplayText( wstrDisplayName);

if (wstrDisplay.empty())
{
	return wstrEmpty;
}

if (nLines == 1)   // Single-line display, so read it all
{
				wstrOut = wstrDisplay;
}
else  // Multi-line display
{
int nLineLength = nLength / nLines;
int nStart = ((nDisplayLine -1) * nLineLength);
wstrOut = wstrDisplay.substr( nStart, nLineLength);
}

return wstrOut;
}


std::wstring MIDISurface::GetDisplayStripString( SurfaceAction myAction, AppConfig * pAppConfig)
{
SurfaceProtocol myProtocol; 
	
	// Get the protocol information so that we can decode correctly 
std::string strProtocolID = GetProtocolID();

if (strProtocolID.empty())
	{
LogIt( "Empty protocol ID in GetDisplayStripString");
return wstrEmpty;
}

try
	{
		myProtocol = pMyProtocols->GetProtocol( strProtocolID);
	}
	catch( ...)
	{
		LogIt( "Protocol not found in GetDisplayStripString");
		return wstrEmpty;
	}

const int DISPLAY_NAME_PARAM = 0;
std::string strDisplayHash = wxString( myAction.GetParameter(DISPLAY_NAME_PARAM).second).ToStdString();
DisplayDefinition myDisplay;

if (strDisplayHash.empty())
	{
		LogIt( "Empty display hash in GetDisplayStripString");
		return wstrEmpty;
}

try
	{
myDisplay = myProtocol.GetDisplay( strDisplayHash);
}
catch( ...)
{
std::string strOut = "Display not found";
strOut.append( strDisplayHash);
strOut.append( " in GetDisplayStripString");
LogIt( strOut);
return wstrEmpty;
}

const int DISPLAY_STRIP_PARAM = 1;
std::wstring wstrDisplayStrip = myAction.GetParameter( DISPLAY_STRIP_PARAM).second;
		long nDisplayStrip = 0; 

			try
			{
				nDisplayStrip = boost::lexical_cast <long> (wstrDisplayStrip);
std::wstring wstrOut = L"Strip number = ";
wstrOut.append( wstrDisplayStrip);
wstrOut.append( L" in GetDisplayStripString");
LogIt( wstrOut);
}
		catch( ...)
			{
LogIt( "Failed to convert display strip to a long value in GetDisplayStripString.");
return wstrEmpty;
		}
		
		int nStrips = myDisplay.GetStripCount();
int nLength = myDisplay.GetDisplayLength();
int nLines = myDisplay.GetLineCount();
bool blnLineFeed = false;

if ((nLength % nLines) > 0)
{  // We must have a line-feed or similar, so adjust
	nLength++;   // Stops fractional substring positions, etc.
blnLineFeed = true;
}

int nLineLength = nLength / nLines;
		int nStripLength = nLineLength / nStrips;

		if (nLineLength % nStrips > 0)
		{
		nStripLength++;
		}

		std::wstring wstrDisplayName;
std::string strLinkedHash = myDisplay.GetLinkedDisplayHash();

if (strLinkedHash.empty())
{
	wstrDisplayName = myDisplay.GetLabel();
}
else
{
	wstrDisplayName = myProtocol.GetDisplay( strLinkedHash).GetLabel();
}

std::wstring wstrOut, wstrLog; 
std::wstring wstrDisplay = GetDisplayText( wstrDisplayName);

if (wstrDisplay.empty())
{
	wstrLog.append( wstrDisplayName);
	wstrLog.append( L" is empty");
	LogIt( wstrLog);
	return wstrEmpty;
}

if (wstrDisplay.length() != nLength)
{
	wstrDisplay.append( wstrSpace);
}

	for (int nLineNumber = 1; nLineNumber <= nLines; nLineNumber++)
{
wstrLog = L"Line ";
wstrLog.append( boost::lexical_cast <std::wstring> (nLineNumber));
wstrLog.append( wstrSpacedColon);
wstrLog.append( wstrDoubleQuotes);

// Get the whole line
unsigned int nLineFeeds = 0;

if (blnLineFeed)
{  // We'll have a line feed at the end of each line, so calculate an offset for all lines except the first
		nLineFeeds = nLineNumber -1;
}

int nStart = (nLineNumber -1) * nLineLength;
std::wstring wstrTemp = wstrDisplay.substr( nStart, nLineLength);

wstrLog.append( wstrTemp);
wstrLog.append( wstrDoubleQuotes);
wstrLog.append( wstrNewLine);
wstrLog.append( L"Strip: ");
wstrLog.append( boost::lexical_cast <std::wstring> (nDisplayStrip));
wstrLog.append( wstrNewLine);
wstrLog.append( L"Strip length: ");
wstrLog.append( boost::lexical_cast <std::wstring> (nStripLength));
wstrLog.append( wstrNewLine);

// Extract the correct strip
nStart = ((nDisplayStrip -1) * nStripLength);
wstrLog.append( L"Start: ");
wstrLog.append( boost::lexical_cast <std::wstring> (nStart));
wstrLog.append( wstrNewLine);

wstrTemp = wstrTemp.substr( nStart, nStripLength);

wstrLog.append( L"Strip content:\"");
wstrLog.append( wstrTemp);
wstrLog.append( wstrDoubleQuotes);
LogIt( wstrLog);

// Update the output string
wstrOut.append( wstrTemp);
wstrOut.append( wstrSpace);
	}

wstrLog = L"GetDisplayStrip = \"";
wstrLog.append( wstrOut);
wstrLog.append( wstrDoubleQuotes);
LogIt( wstrLog);
return wstrOut;
}


std::wstring MIDISurface::GetDisplaySubstring( SurfaceAction myAction, AppConfig * pAppConfig)
{
// Get the protocol information so that we can decode correctly 
std::string strProtocolID = this->GetProtocolID();
SurfaceProtocol myProtocol = pMyProtocols->GetProtocol( strProtocolID);
const int DISPLAY_NAME_PARAM = 0;
std::string strDisplayHash = wxString( myAction.GetParameter(DISPLAY_NAME_PARAM).second).ToStdString();
std::wstring wstrDisplayName = myProtocol.GetDisplay( strDisplayHash).GetLabel();
	int nLength = myProtocol.GetDisplay( strDisplayHash).GetDisplayLength();
std::wstring wstrDisplayText = GetDisplayText( wstrDisplayName);
std::wstring wstrOut;

if (wstrDisplayText.empty())
{
	return wstrEmpty;
}

// Get the requested substring of the display
	const int SUBSTRING_START = 1;
std::wstring wstrStart = myAction.GetParameter( SUBSTRING_START).second;
long nStart = boost::lexical_cast <long> (wstrStart);
nStart--;

const int SUBSTRING_LENGTH = 2;
std::wstring wstrSubLength = myAction.GetParameter( SUBSTRING_LENGTH).second;	
long nSubLength = boost::lexical_cast <long> (wstrSubLength);
nSubLength--;

// Extract the required substring
	wstrOut = wstrDisplayText.substr( nStart, nSubLength);

	return wstrOut;
}


std::wstring MIDISurface::GetLEDString( SurfaceAction myAction, AppConfig * pAppConfig, int nDataValue)
{
	LogIt( "In GetLEDString");

	// Get the protocol information so that we can decode correctly 
std::string strProtocolID = this->GetProtocolID();

if (strProtocolID.empty())
	{
		LogIt( "Empty protocol ID");
		return wstrEmpty;
}

			SurfaceProtocol myProtocol; 

				try
			{
				myProtocol = pMyProtocols->GetProtocol( strProtocolID);
			}
			catch( ...)
			{
				LogIt( "Protocol not found");
				return wstrEmpty;
			}

const int DISPLAY_NAME_PARAM = 0;
std::string strDisplayHash = wxString( myAction.GetParameter(DISPLAY_NAME_PARAM).second).ToStdString();

if (strDisplayHash.empty())
	{
		LogIt( "Empty display hash");
return wstrEmpty;
}

DisplayDefinition myDisplay;

try
	{
		myDisplay = myProtocol.GetDisplay( strDisplayHash);
}
catch( ...)
	{
		LogIt( "Display not found");
		return wstrEmpty;
}

std::wstring wstrDisplayName = myDisplay.GetLabel();
std::wstring wstrOut;

if (myDisplay.IsLEDLamp())
{
std::wstring wstrText = GetDisplayText( wstrDisplayName);

	if (wstrText.empty() == false)
{
bool blnReadIt = false;

switch( myAction.GetSurfaceActionType())
{
case ReadLEDIfOn:
	{
	if (nDataValue != 0)
	{
	blnReadIt = true;
	}
	}
	break;

case ReadLEDIfOff:
	{
		if (nDataValue == 0)
	{
	blnReadIt = true;
	}
}
	break;

default:
	{
	blnReadIt = true;
	}
	}  // end switch

if (blnReadIt)
{
	LogIt( "ReadIt = true");
	wstrOut = wstrDisplayName;
wstrOut.append( wstrSpace);
wstrOut.append( wstrText);
								}  // end if blnReadIt
else
{
LogIt( "ReadIt = false");
}
		}  // end if empty display string

				return wstrOut;
				}
				else  // text display
				{
					return wstrEmpty;
									}  // end if LED
}


std::wstring MIDISurface::GetSpeakString( SurfaceAction myAction, AppConfig * pAppConfig)
{
const int MESSAGE_STRING_PARAM = 0;
return myAction.GetParameter( MESSAGE_STRING_PARAM).second;
}


void MIDISurface::ProcessChangeSurfaceMode( SurfaceAction myAction)
{
	Mode myCurrentMode = this->GetWidgetMode();
	
	if (myCurrentMode == ID_SCREEN_READER_MODE)
	{
		this->SetWidgetMode( ID_LIVE_MODE);
	}
	else
	{
		// myCurrentMode = (unsigned int) myCurrentMode +1;
			int nMode = static_cast <int> (myCurrentMode);
nMode++;
myCurrentMode = static_cast <Mode> (nMode);
		this->SetWidgetMode( myCurrentMode);
	}

	Speak( this->GetLabel(), true);
	std::wstring wstrMode = this->GetModeDescription( myCurrentMode);
Speak( wstrMode, false);
	this->UpdateStatusBar( wstrMode);
}


void MIDISurface::ProcessRunHotSpot( SurfaceAction myAction, AppConfig * pAppConfig)
{
	if (myAction.GetParameterCount() != 2)
	{
		return;
	}

	std::wstring wstrHSCFileName = myAction.GetParameter( 0).second;
	std::wstring wstrSpotName = myAction.GetParameter( 1).second;

	if ((wstrHSCFileName.empty())
		|| (wstrSpotName.empty()))
	{
		return;
	}

HotSpotObject * pHSCObj = new HotSpotObject();
pHSCObj->SetHSCFileName( wstrHSCFileName);
pHSCObj->SetSpotName( wstrSpotName);

wxCommandEvent event( wxEVT_COMMAND_TEXT_UPDATED, HOT_SPOT_CALL_ID);
event.SetEventObject( pHSCObj);
pMyParent->GetEventHandler()->AddPendingEvent( event );
// delete pHSCObj;
}


void MIDISurface::ProcessRunScreenReaderFunction( SurfaceAction myAction, AppConfig * pAppConfig)
{
	std::wstring wstrFunctionName = myAction.GetParameter( 1).second;

	if (wstrFunctionName.empty())
	{
		return;
	}

	ScreenReaderActionType myActionType = myAction.GetScreenReaderActionType();
SRFunctionObject * pSRFunctionObj = new SRFunctionObject();
	pSRFunctionObj->SetFunctionName( wstrFunctionName);
	pSRFunctionObj->SetActionType( myActionType);

wxCommandEvent event( wxEVT_COMMAND_TEXT_UPDATED, SCREEN_READER_FUNCTION_CALL_ID);
event.SetEventObject( pSRFunctionObj);
pMyParent->GetEventHandler()->AddPendingEvent( event );
// delete pSRFunctionObj;
}


std::wstring MIDISurface::GetCurrentState( std::wstring wstrDisplayLabel)
{
				std::map <std::wstring, std::wstring>::iterator it;
it = myDisplays.find( wstrDisplayLabel);

if (it == myDisplays.end())
{
std::wstring wstrOut;
wstrOut.clear();
		return wstrOut;
}
else
{
	return it->second;
}
			}


void MIDISurface::SetCurrentState( std::wstring wstrDisplayLabel, std::wstring wstrState)
{
std::pair <std::wstring, std::wstring> myPair;
myPair = std::make_pair( wstrDisplayLabel, wstrState);

std::map <std::wstring, std::wstring>::iterator it;
it = myDisplays.find( wstrDisplayLabel);

if (it != myDisplays.end())
{
myDisplays.erase( it);
}
myDisplays.insert( myPair);
}


int MIDISurface::GetNibble( int nNibblePos, std::vector <unsigned char> myMessage)
{
int nResult = -1;

if ( (unsigned int) nNibblePos > (myMessage.size() * 2))
{
	return nResult;
}

int nOffset = nNibblePos % 2;
nResult = (nNibblePos /2) + nOffset;
return nResult;
}

	
	void MIDISurface::Speak( std::wstring wstrText, bool blnInterrupt)
		{
SpeechObject * pSpeechObj = new SpeechObject();
pSpeechObj->SetMsg( wstrText);
pSpeechObj->Interrupt( blnInterrupt);

wxCommandEvent event( wxEVT_COMMAND_TEXT_UPDATED, SPEECH_UPDATE_ID);
event.SetEventObject( pSpeechObj);
pMyParent->GetEventHandler()->AddPendingEvent( event );
// delete pSpeechObj;
}
	

void MIDISurface::UpdateStatusBar( std::wstring wstrMode)
	{
		if ( wstrMode.empty())
			{
			return;
	}

	StatusObject * pStatusObj = new StatusObject();
pStatusObj->SetMode( wstrMode);

wxCommandEvent event( wxEVT_COMMAND_TEXT_UPDATED, STATUS_UPDATE_ID);
event.SetEventObject( pStatusObj);
pMyParent->GetEventHandler()->AddPendingEvent( event );
// delete pDisplayObj;
}


		void MIDISurface::UpdateUI( std::wstring wstrLabel, bool blnLEDLamp, int nDisplayLength, int nLineCount, SurfaceProtocol * pProtocol, std::string strTranslationID)
	{
		if ( wstrLabel.empty())
			{
			return;
	}

	std::map <std::wstring, std::wstring>::iterator it;
it = myDisplays.find( wstrLabel);

if (it == myDisplays.end())
{
return;
}

DisplayObject * pDisplayObj = new DisplayObject();
AppConfig myAppConfig;
myAppConfig = pMyApps->GetAppConfig( myAppConfigID);

		if (myAppConfig.GetAppName().empty())  // default behaviour
{
	pDisplayObj->SetLabel( wstrLabel);
}
else
{
std::string strNewHash = wxString( myAppConfig.GetDisplayHash( wstrLabel)).ToStdString();

if (strNewHash.empty())
 {
pDisplayObj->SetLabel( wstrLabel);
}
else
{
	pDisplayObj->SetLabel( myAppConfig.GetDisplayDefinition( strNewHash).GetLabel());
}
}

std::wstring wstrContents = pProtocol->GetTranslatedString( strTranslationID, it->second);
	wstrContents = FormatDisplay( wstrContents, nDisplayLength, nLineCount);

	pDisplayObj->SetContents( wstrContents);
pDisplayObj->IsLEDLamp( blnLEDLamp);
pDisplayObj->SetLineCount( nLineCount);
LogIt(wstrContents);

wxCommandEvent event( wxEVT_COMMAND_TEXT_UPDATED, SURFACE_UPDATE_ID);
event.SetEventObject( pDisplayObj);
pMyParent->GetEventHandler()->AddPendingEvent( event );
// delete pDisplayObj;
	}


		void MIDISurface::UpdateDisplay( std::pair <DisplayDefinition, std::string> myDisplayPair, std::vector <unsigned char> myMessage, SurfaceProtocol * myProtocol)
{
							std::string strMsg = strEmpty;
							// Update the display contents
if (myDisplayPair.first.IsLEDLamp())
{
		std::vector <unsigned char> vState;

		if (myDisplayPair.second.substr( 0, 1).compare( strSysExHashPrefix) == 0)
		{
			std::vector <unsigned char> vAddressBytes = GetSysExAddressBytesFromHash( myDisplayPair.second, myProtocol->GetSysExHeader().size());
unsigned int nHeaderLength = myProtocol->GetSysExHeader().size() + vAddressBytes.size();

				// Extract the value bytes
for (unsigned int i = nHeaderLength; i < (myMessage.size() -1); i++)
{
	vState.push_back( myMessage.at( i));
}  // end for

strMsg = "LED value = [";

for (unsigned int i = 0; i < vState.size(); i++)
{
int nValue = vState.at( i);
strMsg.append( boost::lexical_cast <std::string> (nValue));
strMsg.append( ",");
}

strMsg.append( "]");
LogIt( strMsg);
				}
	else  // Not SysEx
		{
			if (myDisplayPair.second.substr( 0, 1).compare( strNibbleHashPrefix) == 0)
	{   // Nibble-by-nibble hash, so need to figure out where the value is
int nValuePos = 0;

// Count the delimiters within the hash to find the position of the value nibble itself
for (unsigned int i = 0; (myDisplayPair.second.substr( i, 1).compare( NIBBLE_WILD_CARD_MARKER) != 0); i++)
{
	if (myDisplayPair.second.substr( i, 1).compare( HASH_DELIMITER) == 0)
{
nValuePos++;
}
}  // end for

// Add 1 more to account for the delimiter after the wild card
nValuePos++;

// Figure out which byte based on the located nibble position
int nBytePos = nValuePos / 2;
// Which nibble within the byte?
int nByteOffset = nValuePos % 2;
unsigned char nByte  = myMessage.at( nBytePos);

if (nByteOffset == 0)
{
// return the least significant nibble
	vState.push_back( (nByte & 0x0f));
}
else
{
	// return the most significant nibble
	vState.push_back( ((nByte & 0xf0) /16));
}
	}
	else  // Short message
		{
			MIDI myMIDI;
int nData2 = myMIDI.ShortMsgData2( myMessage);
vState.push_back( (unsigned char) nData2);
	}  // end if nibbled
		}  // end if SysEx

if (myProtocol->IsUsingGlobalStates())
{
	SetCurrentState( myDisplayPair.first.GetLabel(), myProtocol->GetDisplayItem( 0).GetState( vState));
std::wstring wstrOut = L"LED state [";
wstrOut.append( myProtocol->GetDisplayItem( 0).GetState( vState));
wstrOut.append( L"]");
LogIt( wstrOut);
}
else
	{
		SetCurrentState( myDisplayPair.first.GetLabel(), myProtocol->GetDisplay( myDisplayPair.second).GetState( vState));
std::wstring wstrOut = L"LED state [";
wstrOut.append( myProtocol->GetDisplay( myDisplayPair.second).GetState( vState));
wstrOut.append( L"]");
LogIt( wstrOut);
}  // end if using global states
}

else  // Not a LED, so must be a text display 
		{
			// Need the cursor position
			std::pair <int, int> myCursorPair;
			bool blnCursorFromLeft = myDisplayPair.first.IsCursorFromLeft();

			myCursorPair = GetCursorInfo( myDisplayPair.second, myMessage, &myDisplayPair.first, myProtocol);
			int nCursorPosition = myCursorPair.first;
			int nCursorOffset = myCursorPair.second;
			int nDisplayLength = myDisplayPair.first.GetDisplayLength();
			int nLineCount = myDisplayPair.first.GetLineCount();
									
			if (nCursorPosition > nDisplayLength)
{
	// Display incorrectly identified, so allow a retry
	LogIt(L"Forcing display redetection.");
	throw RB_NO_DISPLAY_DEFINITION;
}
				
				if (GetDisplayText( myDisplayPair.first.GetLabel()).empty())
		{
// Initialise and Size the display if necessary
					std::wstring wstrTemp;
					wstrTemp.clear();
					wstrTemp.assign(L" ", nDisplayLength);
					SetDisplayText( myDisplayPair.first.GetLabel(), wstrTemp, blnCursorFromLeft, myDisplayPair.first.IsLEDLamp(), nDisplayLength, nLineCount, myProtocol, myDisplayPair.first.GetCharacterTranslationID());
}	  // end if display is empty

				std::wstring wstrNewText = DecodeText( myMessage, myProtocol, &myDisplayPair.first);

				if (wstrNewText.empty())
				{
					LogIt( L"DecodeText returns empty");
					return;
				}
				else
				{
					LogIt(L"wstrNewText = " + wstrNewText);
				}

if (nCursorOffset == NO_CURSOR)  // No cursor, so update the whole display
{
	SetDisplayText( myDisplayPair.first.GetLabel(), wstrNewText, blnCursorFromLeft, myDisplayPair.first.IsLEDLamp(), nDisplayLength, nLineCount, myProtocol, myDisplayPair.first.GetCharacterTranslationID());
	}
else  // cursor position provided
{
	SetDisplayText(myDisplayPair.first.GetLabel(), nCursorPosition, wstrNewText, blnCursorFromLeft, myDisplayPair.first.IsLEDLamp(), nDisplayLength, nLineCount, myProtocol, myDisplayPair.first.GetCharacterTranslationID());
	}  // end if valid cursor position
}  // end if LED	
}


std::pair <int, int> MIDISurface::GetCursorInfo( std::string strHash, std::vector <unsigned char> myMessage, DisplayDefinition * myDisplay, SurfaceProtocol * pProtocol)
{
	bool blnCursorFromLeft = myDisplay->IsCursorFromLeft();
	std::pair <int, int> myPair;

	int nCursorOffset = -1;			
int nCursorPosition = -1;

if (strHash.substr( 0, 1).compare( strSysExHashPrefix) == 0)
				{
		// SysEx-based display, so calculate the offset for the cursor position byte - we're assuming that it is immediately after the address bytes (may not be safe)
int nHeaderLength = pProtocol->GetSysExHeader().size();
int nAddressLength = myDisplay->GetSysExAddressBytes().size();
nCursorOffset = nHeaderLength + nAddressLength;
nCursorPosition = myMessage.at( nCursorOffset);
			}

if (strHash.substr( 0, 1).compare( strNibbleHashPrefix) == 0)
				{
					// Count the delimiters to get to the range indicator
nCursorOffset = 0;

for (unsigned int i = 0; i < strHash.length(); i++)
					{
						if (strHash.at( i) == HASH_DELIMITER)
						{
							nCursorOffset++;
						}
						else if (strHash.at( i) == NIBBLE_RANGE_MARKER)
						{
nCursorOffset++;
break;
						}
					}  // end for i

// Work through the message, nibble by nibble, to find the cursor position
int nNibble = 0;

for (unsigned int j = 0; j < myMessage.size(); j++)
	{
unsigned char nByte = myMessage.at( j);

for (unsigned k = 1; k <= 2; k++)
{
if (k == 1)  // high nibble
{
nNibble++;

if (nNibble == nCursorOffset)
{
int nHighNibble = nByte >> 4;		
nCursorPosition = nHighNibble;
break;
}
}
else  // k == 2  // low nibble
{
nNibble++;

if (nNibble == nCursorOffset)
{
int 	nLowNibble = nByte & MIDI_CHN_MASK;	
nCursorPosition = nLowNibble;
break;
}
}
}  // end for k

if (nCursorPosition > -1)
{
break;
}
}  // end for j

switch (nCursorPosition)
	{
case 0xA:
	{
	nCursorPosition = 0;
	}
	break;

case 0xB:
	{
nCursorPosition = 1;
	}
	break;

default:
	{}
	break;
}  // end switch
}  // end if nibbled

myPair = std::make_pair( nCursorPosition, nCursorOffset);
return myPair;
}


void MIDISurface::ProcessLiveModeMessage( std::pair <MessageDefinition, std::string> myHardwarePair, SurfaceProtocol * pProtocol, AppConfig * pAppConfig, unsigned int nControlValue, std::vector <unsigned char> vBuffer)
{
std::wstring wstrOut( wstrEmpty);

if (pAppConfig->GetAppName().empty())  // No application, so use default behaviour
			{
				// Log it
std::wstring wstrHardwareInfo = L"Default Hardware identified: ";
wstrHardwareInfo.append( myHardwarePair.first.GetLabel());
LogIt( wstrHardwareInfo);
	
SurfaceAction myAction;
					myAction.SetSurfaceActionType( ReadControlLabel);

					try
						{
							wstrOut = GetActionString( myAction, myHardwarePair.first, myHardwarePair.second, nControlValue, pProtocol, pAppConfig, vBuffer);
					}
		catch( int nError)
		{
if (nError == RB_NO_CONTROL_DEFINITION)
	{}  // if nError > 0
else
		{}
		}  // end catch
		}
			
else  // Process any defined application configuration entry for the current screen reader
		{
			// Log it
std::wstring wstrHardwareInfo = L"App Config hardware identified: ";
wstrHardwareInfo.append( pAppConfig->GetControlDefinition( myHardwarePair.second).GetLabel());
LogIt( wstrHardwareInfo);
	
SurfaceActions mySurfaceActions; 

try
{
	mySurfaceActions = pAppConfig->GetControlDefinition( myHardwarePair.second).GetSurfaceActions( MyProduct);
}
catch ( ...)
	    {        		}  // end catch

// Figure out what to announce
if ((mySurfaceActions.GetLiveModeActionCount() == 0)
|| (GetWidgetMode() == ID_ID_MODE))
{
				// Announce the default label 
								SurfaceAction myAction;
					myAction.SetSurfaceActionType( ReadControlLabel);

					try
						{
							wstrOut = GetActionString( myAction, pAppConfig->GetControlDefinition( myHardwarePair.second), myHardwarePair.second, nControlValue, pProtocol, pAppConfig, vBuffer);
					wstrOut.append( wstrSpace);
					}
		catch ( RBException & myException)
	    {
#ifdef __WINDOWS__ 
OutputDebugString( myException.what());
#endif
}  // end catch
}  // end if no actions or ID mode

// Continue to process any actions
for (int i = 0; i < mySurfaceActions.GetLiveModeActionCount(); i++)
			{
				std::wstring wstrTemp = GetActionString( mySurfaceActions.GetLiveModeAction( i), pAppConfig->GetControlDefinition( myHardwarePair.second), myHardwarePair.second, nControlValue, pProtocol, pAppConfig, vBuffer);

if (wstrTemp.empty() == false)
{
wstrOut.append( wstrTemp); 
wstrOut.append( wstrSpace);
}
}   // end for
}  // end if application configuration found

// Speak the identified action strings
Speak( wstrOut, true);  
}


std::pair <DisplayDefinition, std::string> MIDISurface::IdentifyDisplay( std::vector <unsigned char> vMessageBuffer, AppConfig * pAppConfig)
{
// Try and identify the display based on the buffered data
		MIDI myMIDI( vMessageBuffer, false);

// Get the protocol information so that we can decode correctly 
std::string strProtocolID = GetProtocolID();
SurfaceProtocol myProtocol = pMyProtocols->GetProtocol( strProtocolID);

std::pair <DisplayDefinition, std::string> myDisplayPair;
DisplayDefinition myDisplay;

// Check for any matching nibble-addressed display
std::string strHash = wxString( myMIDI.NibbledShortMIDIHash( vMessageBuffer, (vMessageBuffer.size() *2))).ToStdString();

try
	{
myDisplayPair = myProtocol.GetNibbleDisplay( strHash);
						return myDisplayPair;
}  // end try
catch( int nError)
	{
		if (nError == RB_NO_DISPLAY_DEFINITION)
{
	strHash.clear();
}
else  // Unknown error
	{
		strHash.clear();
		}
}  // end nibble try/catch

strHash = myMIDI.MIDIHash();

if	(strHash.empty())
	{
		throw RB_NO_DISPLAY_DEFINITION;  // invalid hash
return myDisplayPair;
}

if (strHash.substr( 0,1).compare( strSysExHashPrefix) == 0)
			{
				// SysEx message, so our hash may be longer than the stored value due to trailing command and data bytes 
try
	{
		myDisplay = myProtocol.GetSysExDisplay( strHash);
myDisplayPair.first = myDisplay;
// Use the hash in the display definition, as this will not include trailing data and system exclusive end bytes
// myDisplayPair.second = strHash;
myDisplayPair.second = myProtocol.GetDisplayHash( myDisplay.GetLabel());
		return myDisplayPair;
}  // end try
		catch( int nError)
{
if (nError == RB_NO_DISPLAY_DEFINITION)
{
throw RB_NO_DISPLAY_DEFINITION;
return myDisplayPair;
}  // end if nError
else
{
throw RB_NO_DISPLAY_DEFINITION;
return myDisplayPair;
}
		}  // end if sysex catch
						}  // end if SysEx hash

else  // short message, so we can search for an exact match
{
try
	{
		myDisplay = myProtocol.GetDisplay( strHash);
myDisplayPair.first = myDisplay;
myDisplayPair.second = strHash;
		return myDisplayPair;
}  // end try
		catch( ...)
{
throw RB_NO_DISPLAY_DEFINITION;
		return myDisplayPair;
		}  // end if short message catch
}  // end short message hash

return myDisplayPair;
}


std::pair <MessageDefinition, std::string> MIDISurface::IdentifyHardwareControl( std::vector <unsigned char> vMessageBuffer, SurfaceProtocol * pProtocol)
{
// Try and identify the control based on the buffered data
std::pair <MessageDefinition, std::string> myHardwarePair;
MessageDefinition myControlDefinition;
MIDI myMIDI( vMessageBuffer, false);
bool blnFound = false;

// Check for any matching nibble-addressed hardware controls
std::string strHash = myMIDI.NibbledShortMIDIHash( vMessageBuffer, (vMessageBuffer.size() *2));

try
	{
		myHardwarePair = pProtocol->GetNibbleHardwareControl( strHash);
return myHardwarePair;
}  // end try
catch( int nError)
	{
		if (nError == RB_NO_CONTROL_DEFINITION)
{
	strHash.clear();
}
else  // Unknown error
	{  
		strHash.clear();
		}
}  // end nibble try/catch

				// Try to generate a non-nibbled hash value in order to identify which hardware control generated the message
strHash = myMIDI.MIDIHash();

try
	{
		myControlDefinition = pProtocol->GetHardwareControl( strHash);
		myHardwarePair = std::make_pair( myControlDefinition, strHash);
return myHardwarePair;
}  // end try
catch( int nError)
	{
		if (nError == RB_NO_CONTROL_DEFINITION)
{
	strHash.clear();
myHardwarePair = std::make_pair( myControlDefinition, strHash);
return myHardwarePair;
		}
else  // Unknown error
	{  
strHash.clear();
myHardwarePair = std::make_pair( myControlDefinition, strHash);
				return myHardwarePair;
		}
}  // end nibble try/catch
}


bool MIDISurface::ProcessIDModeMessage( std::string strHash, MessageDefinition myMessageDefinition, SurfaceProtocol * pProtocol, AppConfig * pAppConfig, unsigned int nControlState, std::vector <unsigned char> vBuffer)
{
bool blnFound = false;
std::wstring wstrOut( wstrEmpty);

				// Announce the label 
SurfaceAction myAction;
					myAction.SetSurfaceActionType( ReadControlLabel);
					
					if (pAppConfig->GetAppName().empty() == false)
					{
// Modify behaviour if the app config contains an override value
						try
	{
		myMessageDefinition = pAppConfig->GetControlDefinition(strHash);					
}
catch( ...)
{}
					}  // end if app config

					try
						{
							wstrOut = GetActionString( myAction, myMessageDefinition, strHash, nControlState, pProtocol, pAppConfig, vBuffer);

							// Speak the name of the identified display
	Speak( wstrOut, true);  
blnFound = true;
					}
		catch( int nError)
		{
if (nError == RB_NO_CONTROL_DEFINITION)
	{
return blnFound;
}  // if nError 
else
		{
return blnFound;
}
		}  // end catch

		return blnFound;
}


bool MIDISurface::ProcessConfigModeMessage( std::string strHash, MessageDefinition myMessageDefinition, SurfaceProtocol * pProtocol, AppConfig * pAppConfig, unsigned int nControlState, std::vector <unsigned char> vBuffer)
{
bool blnFound = false;

return blnFound;
}


void MIDISurface::ProcessScreenReaderModeMessage( std::string strHash, MessageDefinition myMessageDefinition, SurfaceProtocol * pProtocol, AppConfig * pAppConfig, unsigned int nControlValue, std::vector <unsigned char> vBuffer)
{
std::wstring wstrOut( wstrEmpty);

if (pAppConfig->GetAppName().empty())  // No application, so use default behaviour
			{
				SurfaceAction myAction;
					myAction.SetSurfaceActionType( ReadControlLabel);

					try
						{
							wstrOut = GetActionString( myAction, myMessageDefinition, strHash, nControlValue, pProtocol, pAppConfig, vBuffer);
					}
		catch( int nError)
		{
if (nError == RB_NO_CONTROL_DEFINITION)
	{}  // if nError > 0
else
		{}
		}  // end catch
		}
			else  // Process any defined application configuration entry for the current screen reader
		{
			SurfaceActions mySurfaceActions; 

try
{
	mySurfaceActions = pAppConfig->GetControlDefinition( strHash).GetSurfaceActions( MyProduct);
}
catch ( ...)
	    {        		}  // end catch

if (mySurfaceActions.GetScreenReaderModeActionCount() == 0)
{
				// Announce the default label 
								SurfaceAction myAction;
					myAction.SetSurfaceActionType( ReadControlLabel);

					try
						{
							wstrOut = GetActionString( myAction, pAppConfig->GetControlDefinition( strHash), strHash, nControlValue, pProtocol, pAppConfig, vBuffer);
					}
		catch ( RBException & myException)
	    {
#ifdef __WINDOWS__ 
OutputDebugString( myException.what());
#endif
}  // end catch
}

for (int i = 0; i < mySurfaceActions.GetScreenReaderModeActionCount(); i++)
			{
				std::wstring wstrTemp = GetActionString( mySurfaceActions.GetScreenReaderModeAction( i), pAppConfig->GetControlDefinition( strHash), strHash, nControlValue, pProtocol, pAppConfig, vBuffer);

if (wstrTemp.empty() == false)
{
wstrOut.append( wstrTemp); 
wstrOut.append( wstrSpace);
}
}   // end for
}  // end if application configuration found

// Speak the identified action strings
Speak( wstrOut, true);  
}


void MIDISurface::ProcessDisplayActions()
{
	/*
	for (unsigned int i = 0; i < qActions.size(); i++)
	{
ActionRequest myActionRequest;
myActionRequest = qActions.front();
		
switch ( myActionRequest.Action.GetSurfaceActionType())
{
case ReadDisplay:
	{
	ProcessReadDisplay( myActionRequest.Action, myActionRequest.AppConfigPointer, myActionRequest.ControlState);	
}
	break;

case ReadDisplayLine:
	{
	ProcessReadDisplayLine( myActionRequest.Action, myActionRequest.AppConfigPointer);	
}
	break;

case 	ReadDisplayStrip:
{
	ProcessReadDisplayStrip( myActionRequest.Action, myActionRequest.AppConfigPointer);	
}
	break;

case ReadDisplaySection:
{
	ProcessReadDisplaySection( myActionRequest.Action, myActionRequest.AppConfigPointer);	
}
	break;

default:
	{}
	break;
}  // end switch

qActions.pop();
	}  // end for
*/
}


std::wstring MIDISurface::GetMIDIInfoString( bool blnIsEncrypted, SurfaceAction myAction, AppConfig * pAppConfig, std::vector <unsigned char> vBuffer)
{
if (blnIsEncrypted)   // Not allowed to reveal the MIDI data for encrypted protocols
{
return wstrEmpty;;
}

// Output analysis of the MIDI message
std::wstring wstrOut;
MIDI myMIDI;

if (myMIDI.IsValidSysExHeader( vBuffer))
{
return wstrOut;
}

if (vBuffer.size() == SHORT_MIDI_MESSAGE_LENGTH)
	{
		unsigned char nCommand = (unsigned char) myMIDI.ShortMsgCommandNibble( vBuffer);
		unsigned char nChannel = (unsigned char) myMIDI.ShortMsgChannelNibble( vBuffer);
unsigned char nData1 = (unsigned char) myMIDI.ShortMsgData1( vBuffer);
	unsigned char nData2 = (unsigned char) myMIDI.ShortMsgData2( vBuffer);
 
	// Construct the output string
	wstrOut = myMIDI.StatusCommandString( vBuffer);
wstrOut.append( wstrSpacedComma);
wstrOut.append( wstrChannel);
wstrOut.append( boost::lexical_cast <std::wstring> (static_cast<unsigned int> ( nChannel)));
wstrOut.append( wstrSpacedComma);

if (nCommand == MIDI_CMD_CONTROL_CHANGE)
{
	wstrOut.append( myMIDI.ControllerString( nData1));
}
else
{
wstrOut.append( wstrData1);
}

wstrOut.append( boost::lexical_cast <std::wstring> (static_cast<unsigned int> ( nData1)));
wstrOut.append( wstrSpacedComma);
wstrOut.append( wstrData2);
wstrOut.append( boost::lexical_cast <std::wstring> (static_cast<unsigned int> ( nData2)));
return wstrOut;
}
else   // Compound message = RPN, NRPN or bank select
{
	return wstrEmpty;
}
}


void MIDISurface::SendHardwareMessage( std::vector <unsigned char> * pMessage)
{
try
	{
		m_HardwareMIDIOut->sendMessage( pMessage);
	// Sleep( 10);
}
catch ( RtMidiError &errMsg)
{
std::string strLog = "Hardware send error: ";
strLog.append( errMsg.getMessage());
strLog.append( BytesToHex( *pMessage));
LogIt( strLog);
}
}


void MIDISurface::SendDisplayMessage( std::vector <unsigned char> * pMessage)
{
try
	{
		Sleep( 10);
m_DisplayMIDIOut->sendMessage( pMessage);
}
catch ( RtMidiError &errMsg)
{
std::string strLog = "Display send error: ";
strLog.append( errMsg.getMessage());
strLog.append( BytesToHex( *pMessage));
LogIt( strLog);
}
}


bool MIDISurface::IsValidReadDisplayDelay( int nDelay)
	{
return ((nDelay >= MIN_READ_DISPLAY_DELAY) 
	&& (nDelay <= MAX_READ_DISPLAY_DELAY));
}


int MIDISurface::GetReadDisplayDelay()
{
return nReadDisplayDelay;
}


bool MIDISurface::SetReadDisplayDelay( int nNewDelay)
	{
if (IsValidReadDisplayDelay( nNewDelay))	
		{
			nReadDisplayDelay = nNewDelay;
			return true;
}
else
{
return false;
}
}


	std::wstring MIDISurface::GetAppConfigName()
{
	return wstrAppConfigName;
}


void MIDISurface::SetAppConfigName( std::wstring wstrNewAppConfigName)
{
	wstrAppConfigName = wstrNewAppConfigName;
}


std::wstring MIDISurface::GetProtocolName()
{
	return wstrProtocolName;
}


void MIDISurface::SetProtocolName( std::wstring wstrNewProtocolName)
{
	wstrProtocolName = wstrNewProtocolName;
}

	

bool MIDISurface::StopHardwareProcessing()
{
	bool blnResult = true;

	try
	{
		m_HardwareMIDIIn->cancelCallback();
	}
	catch( ...)
	{
	blnResult = false;
	}

	return blnResult;
}


bool MIDISurface::StopDisplayProcessing()
{
	bool blnResult = true;

	try
	{
		m_DisplayMIDIIn	->cancelCallback();
	}
	catch( ...)
	{
	blnResult = false;
	}

	return blnResult;
}


bool MIDISurface::ResumeHardwareProcessing()
{
	bool blnResult = true;

	try
	{
		m_HardwareMIDIIn->setCallback( &HardwareCallback, (void*) this);
	}
	catch( ...)
	{
	blnResult = false;
	}

	return blnResult;
}


bool MIDISurface::ResumeDisplayProcessing()
{
	bool blnResult = true;

	try
	{
		m_DisplayMIDIIn	->setCallback( &HardwareCallback, (void*) this);
	}
	catch( ...)
	{
	blnResult = false;
	}

	return blnResult;
}


void MIDISurface::LogIt( std::string strMessage)
{
	SurfaceProtocol myProtocol;
	
	try
		{
			myProtocol = pMyProtocols->GetProtocol( this->GetProtocolID());
	}
	catch( ...)
	{
		return;
	}

/*
if (myProtocol.IsEncrypted())
{
	BOOST_LOG_SEV( lg, boost::log::trivial::info) << Encrypt( strMessage);
}
else
{
		*/
	BOOST_LOG_SEV( lg, boost::log::trivial::info) << strMessage;
/*
}
*/
}


void MIDISurface::LogIt( std::wstring wstrMessage)
{
	std::string strMessage( wstrMessage.begin(), wstrMessage.end());
this->LogIt( strMessage);
}


std::wstring MIDISurface::UniCoded( std::vector <unsigned char> vBytes)
{
	std::wstring wstrOut;
unsigned int nHighByte = 0;
unsigned int nLowByte = 0;
unsigned int nSize = vBytes.size();

if ((nSize % 4) > 0)
{
	std::string strMsg = "Vbytes.size() = ";
	strMsg.append( boost::lexical_cast <std::string> (nSize));
	strMsg.append( ", which is not a multiple of 4");
	LogIt( strMsg);
	return wstrOut;
}

for (unsigned int i = 0; i < nSize; i++)
	{
unsigned int nNibblePos = i % 4;   // Assume 4 nibbles used to encode one character, so need to figure out which nibble we're looking at

switch( nNibblePos)
{
case HByteMSNibble:
{
nHighByte = (unsigned int) nHighByte + (vBytes.at( i) * 16);
}
break;

case HByteLSNibble:
{
nHighByte = nHighByte + (unsigned int) vBytes.at( i);
} 
break;

case LByteMSNibble:
{
nLowByte = (unsigned int) nLowByte + (vBytes.at( i) * 16);
}
break;

// case LByteLSNibble:
default:
{
nLowByte = nLowByte + (unsigned int) vBytes.at( i);
long double nCharCode = (nHighByte * 256) + nLowByte;

if (nCharCode == 0)
	{
wstrOut.append( wstrSpace);
}
else
{
		try
			{
				wchar_t myWChar = static_cast<wchar_t>(nCharCode); 
wstrOut.push_back( myWChar);
		}
catch( ...)
{
	std::wstring wstrMsg = L"Static cast fails for value";
wstrMsg.append( boost::lexical_cast <std::wstring> (nCharCode));
LogIt( wstrMsg);
wstrOut.append( wstrSpace);
}
}  // end if nCharCode not = 0

nHighByte = 0;
nLowByte = 0;
}
break;
}  // end switch
	}  // end for
	
	return wstrOut;
}


// Callbacks

	void HardwareCallback(double deltatime, std::vector< unsigned char > *message, void *pSurface) 
{
MIDISurface * pMySurface = (MIDISurface*) pSurface;
	std::string strLog = "Hardware received ";
strLog.append( BytesToHex( * message));
pMySurface->LogIt( strLog);
	
	// Pass the message along to the DAW
if ((pMySurface->GetWidgetMode() == ID_LIVE_MODE)
	|| (pMySurface->GetWidgetMode() == ID_CONFIG_MODE))
{
	pMySurface->SendHardwareMessage( message);
}			
		pMySurface->AnalyseHardwareMessage( * message);    
}


	void DisplayCallback(double deltatime, std::vector< unsigned char > *message, void *pSurface)
{
MIDISurface * pMySurface = (MIDISurface*) pSurface;

std::string strLog = "Display received ";
strLog.append( BytesToHex( * message));
pMySurface->LogIt( strLog);

pMySurface->SendDisplayMessage( message);
			pMySurface->AnalyseDisplayMessage( * message);    
}



