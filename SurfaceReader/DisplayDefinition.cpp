// DisplayDefinition.cpp
//Implementation of the DisplayDefinition Class.


#include "DisplayDefinition.h"


DisplayDefinition::DisplayDefinition()
		:
MessageDefinition(),
	blnIsLEDLamp( false),
nDisplayLineCount( -1),
nDisplayLength( -1),
nStripCount( -1),
blnCursorFromLeft( true),
nCursorOffset( 0),
nCursorPos( 0),
blnIsUniCode( false),
strLinkedDisplayHash( strEmpty),
strTranslationID( strEmpty)
{}


DisplayDefinition::DisplayDefinition( bool blnIsLED)
		:
MessageDefinition(),
	blnIsLEDLamp( blnIsLED),
nDisplayLineCount( -1),
nDisplayLength( -1),
nStripCount( -1),
blnCursorFromLeft( true),
nCursorOffset( 0),
nCursorPos( 0),
blnIsUniCode( false),
strLinkedDisplayHash( strEmpty),
strTranslationID( strEmpty)
{}


DisplayDefinition::~DisplayDefinition()
	{}


bool DisplayDefinition::IsLEDLamp()
{
	return blnIsLEDLamp;
}


void DisplayDefinition::IsLEDLamp( bool blnIsLED)
{
	blnIsLEDLamp = blnIsLED;
}


	int DisplayDefinition::GetLineCount()
{
return nDisplayLineCount;
}


void DisplayDefinition::SetLineCount( int nLines)
{
nDisplayLineCount = nLines;
}


int DisplayDefinition::GetDisplayLength()
{
return nDisplayLength;
}


void DisplayDefinition::SetDisplayLength( int nLength)
{
nDisplayLength = nLength;
}


int DisplayDefinition::GetStripCount()
{
return nStripCount;
}


void DisplayDefinition::SetStripCount( int nStrips)
{
nStripCount= nStrips;
}


std::string DisplayDefinition::GetCharacterTranslationID()
{
return strTranslationID;
}


void DisplayDefinition::SetCharacterTranslationID( std::string strTableID)
{
strTranslationID = strTableID;
}


int DisplayDefinition::GetSysExCursorOffset()
{
	return nCursorOffset;
}


void DisplayDefinition::SetSysExCursorOffset( int nNewCursorOffset)
{
	nCursorOffset = nNewCursorOffset;
}


int DisplayDefinition::GetNibbledCursorPosition( std::string strHash, unsigned char nData2)
{
int nResult = -1;

if (strHash.substr( 0, 1).compare( strNibbleHashPrefix) == 0)
{
nResult = nData2 & MIDI_CHN_MASK;;
}

return nResult;
}


bool DisplayDefinition::IsCursorFromLeft()
{
	return blnCursorFromLeft;
}


void DisplayDefinition::SetCursorFromLeft( bool blnLefty)
{
	blnCursorFromLeft = blnLefty;
}


bool DisplayDefinition::IsUniCode()
{
return blnIsUniCode;
}


void DisplayDefinition::SetUniCode( bool blnUniCoded)
{
	blnIsUniCode = blnUniCoded;
}


std::string DisplayDefinition::GetLinkedDisplayHash()
{
return strLinkedDisplayHash;
}


void DisplayDefinition::SetLinkedDisplayHash( std::string strLinkedHash)
{
strLinkedDisplayHash = strLinkedHash;
}


