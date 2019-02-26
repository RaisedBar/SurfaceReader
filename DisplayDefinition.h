// DisplayDefinition.h
//DisplayDefinition Class.


// #define _SCL_SECURE_NO_WARNINGS

#ifndef DISPLAYDEF_H
#define DISPLAYDEF_H

#pragma once

#include "MessageDefinition.h"
#include "MIDI.h"

class DisplayDefinition: public MessageDefinition
{
public:
	DisplayDefinition();
	DisplayDefinition( bool blnIsLED);
		~DisplayDefinition();

bool IsLEDLamp();
void IsLEDLamp( bool blnIsLED);
 int GetLineCount();
void SetLineCount( int nLines);
 int GetDisplayLength();
void SetDisplayLength( int nLength);
 int GetStripCount();
void SetStripCount(int nStrips);
int GetSysExCursorOffset();
void SetSysExCursorOffset( int nNewCursorOffset);
bool IsCursorFromLeft();
void SetCursorFromLeft( bool blnLefty);
int GetNibbledCursorPosition( std::string wstrHash, unsigned char nData2);
bool IsUniCode();
void SetUniCode( bool blnUniCoded);
std::string GetCharacterTranslationID();
void SetCharacterTranslationID( std::string strTableID);
std::string GetLinkedDisplayHash();
void SetLinkedDisplayHash( std::string strLinkedHash);

private:
	friend std::ostream & operator<<( std::ostream &os, const DisplayDefinition &DD);
friend std::istream & operator>>( std::istream &is, const DisplayDefinition &DD);
friend class boost::serialization::access;
    
	template<class Archive>
void serialize( Archive & myArchive, const unsigned int version)
	{
const char * cLabelTag = "Label";
myArchive & boost::serialization::make_nvp( cLabelTag, wstrLabel);
const char * cAddressBytesTag = "SysExAddressBytes";
myArchive & boost::serialization::make_nvp( cAddressBytesTag, vSysExAddressBytes);
const char * cActionsTag = "Actions";
myArchive & boost::serialization::make_nvp( cActionsTag, myActions);
const char * cStatesTag = "StateNames";
myArchive & boost::serialization::make_nvp( cStatesTag, StateLabels);
const char * cLineCountTag = "LineCount";
myArchive & boost::serialization::make_nvp( cLineCountTag, nDisplayLineCount);
const char * cLineLengthTag = "LineLength";
myArchive & boost::serialization::make_nvp( cLineLengthTag, nDisplayLength);
const char * cStripCountTag = "StripCount";
myArchive & boost::serialization::make_nvp( cStripCountTag, nStripCount);
const char * cIsLEDLampTag = "LEDLamp";
myArchive & boost::serialization::make_nvp( cIsLEDLampTag, blnIsLEDLamp);
const char * cTranslationTag = "TranslationID";
myArchive & boost::serialization::make_nvp( cTranslationTag, strTranslationID);
const char * cCursorOffsetTag = "CursorOffset";
myArchive & boost::serialization::make_nvp( cCursorOffsetTag, nCursorOffset);

if(version > 0)
            {
const char * cCursorFromLeftTag = "CursorFromLeft";
myArchive & boost::serialization::make_nvp( cCursorFromLeftTag, blnCursorFromLeft);
}

if(version > 1)
            {
const char * cUniCodeTag = "Unicode";
myArchive & boost::serialization::make_nvp( cUniCodeTag, blnIsUniCode);
}

if(version > 2)
            {
const char * cLinkedHashTag = "LinkedTo";
myArchive & boost::serialization::make_nvp( cLinkedHashTag, strLinkedDisplayHash);
}
}

// Internal storage
std::string strLinkedDisplayHash;
bool blnIsLEDLamp;
int nDisplayLineCount;
 int nDisplayLength;
 // Can this display be read as columns?
 int nStripCount;
// Cursor positioning
 bool blnCursorFromLeft;
 // The position of the cursor is indicated by the byte at this message offset
int nCursorOffset;
 int nCursorPos;
 // Handle unicode
  bool blnIsUniCode;
 // The ID for any required character translation table
std::string strTranslationID; 
 };

// Set the class version for serialisation
BOOST_CLASS_VERSION(DisplayDefinition, 3);
#endif   // DISPLAYDEF_H