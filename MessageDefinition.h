// MessageDefinition.h
//MessageDefinition Class.


// #define _SCL_SECURE_NO_WARNINGS

#ifndef MSGDEF_H
#define MSGDEF_H

#pragma once


#include "SurfaceActions.h"

class MessageDefinition
{
public:
	MessageDefinition();
	MessageDefinition( std::wstring wstrNewLabel);
	~MessageDefinition();

std::wstring GetLabel();
void SetLabel( std::wstring wstrLabel);
std::vector <unsigned char> GetSysExAddressBytes();
void SetSysExAddressBytes( std::vector <unsigned char> vMyAddressBytes);
int GetStateCount();
std::vector <std::wstring> GetStateLabels();
std::vector <unsigned char> GetStateBytes( int nItem);
std::wstring GetState( std::vector <unsigned char> vStateBytes);
void SetState( std::vector <unsigned char> vStateBytes, std::wstring wstrStateLabel);
bool DeleteState( std::vector <unsigned char> vStateBytes);
bool GetMIDIModifier();
void SetMIDIModifier( bool blnShift);
bool GetButtonPressed();
void SetButtonPressed( bool blnPressedState);

SurfaceActions GetSurfaceActions( ActiveProduct ap);
void SetSurfaceActions( ActiveProduct ap,SurfaceActions mySurfaceActions); 
void SetLiveModeAction( ActiveProduct myProduct, SurfaceAction myAction); 
void SetScreenReaderModeAction( ActiveProduct myProduct, SurfaceAction myAction); 

protected:
	friend std::ostream & operator<<( std::ostream &os, const MessageDefinition &M);
friend std::istream & operator>>( std::istream &is, const MessageDefinition&M);
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
const char * cMIDIModifierTag = "Modifier";
myArchive & boost::serialization::make_nvp( cMIDIModifierTag, blnMIDIModifier);
}
		
// Label for the hardware control or display:
std::wstring wstrLabel;
// SysEx address bytes
std::vector <unsigned char> vSysExAddressBytes;
// Actions to be performed when a given control is changed, indexed on the access product
std::map < ActiveProduct,SurfaceActions> myActions;

// Defined states for this control:
std::map<std::vector <unsigned char>, std::wstring> StateLabels;
std::vector <unsigned char> vCurrentState;

// Is this a modifier button that transmits its own MIDI message?
bool blnMIDIModifier;

// Is this a button that is being held down?
bool blnPressed;
};


#endif   // MSGDEF_H
