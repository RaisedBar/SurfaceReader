// MIDI.h 
// definition of the MIDI class


// #define _SCL_SECURE_NO_WARNINGS

#ifndef MIDI_H
#define MIDI_H

#pragma once

#include <string>
#include <vector>

#include "SRConstants.h"
#include "RBStringFuncs.h"

#include <wx/string.h>
#include <wx/msw/winundef.h>

#include <boost/lexical_cast.hpp>
using boost::bad_lexical_cast;


#define MIDI_CODE_MASK  0xf0
#define MIDI_CHN_MASK   0x0f
#define MIDI_REALTIME   0xf8
  #define MIDI_CHAN_MODE  0xfa 
#define MIDI_POLY_TOUCH 0xa0
#define MIDI_SYSEX 0xf0
#define MIDI_Q_FRAME	0xf1
#define MIDI_SONG_POINTER 0xf2
#define MIDI_SONG_SELECT 0xf3
#define MIDI_TUNE_REQ	0xf6
#define MIDI_EOX 0xf7
#define MIDI_TIME_CLOCK 0xf8
#define MIDI_START      0xfa
#define MIDI_CONTINUE	0xfb
#define MIDI_STOP       0xfc
#define MIDI_ACTIVE_SENSING 0xfe
#define MIDI_SYS_RESET  0xff

#define MIDI_ALL_SOUND_OFF 0x78
#define MIDI_RESET_CONTROLLERS 0x79
#define MIDI_LOCAL	0x7a
#define MIDI_ALL_OFF	0x7b
#define MIDI_OMNI_OFF	0x7c
#define MIDI_OMNI_ON	0x7d
#define MIDI_MONO_ON	0x7e
#define MIDI_POLY_ON	0x7f

// RPN and NRPN controller values:
// RPN 98 and 99, 
// NRPN  100 and 101

// Message sizing
const unsigned int SHORT_MIDI_MESSAGE_LENGTH = 3;
// Virtual ports
const int VP_PORT_ID = 1000;

// Location of ID bytes within a SysEx header:
const int DEFAULT_MANUFACTURER_POSITION = 1;
const int MIN_MIDI_CHANNEL = 1;
const int MAX_MIDI_CHANNEL = 16;
const int MIN_MIDI_STATUS = 0x80;
const int MAX_MIDI_STATUS = 0xFF;
const int MIN_MIDI_VOICE_STATUS = 0x80;
const int MAX_MIDI_VOICE_STATUS = 0xEF; 
const int NIBBLE_CURSOR_OFFSET = 3;


// Status byte voice command values:
const int MIDI_CMD_NOTE_OFF = 0x8;
const int MIDI_CMD_NOTE_ON = 0x9;
const int MIDI_CMD_POLY_AFTER_TOUCH = 0xA;
const int MIDI_CMD_CONTROL_CHANGE = 0xB;
const int MIDI_CMD_PROGRAM_CHANGE = 0xC;
const int MIDI_CMD_CHANNEL_PRESSURE = 0XD;
const int MIDI_CMD_PITCH_WHEEL = 0xE;

// Data bytes
const int MIN_MIDI_DATA_BYTE = 0x00;
const int MAX_MIDI_DATA_BYTE = 0x7F;

// Data1:
const unsigned char CC_BANK_SELECT_MSB = 0;
const unsigned char CC_MOD_WHEEL_MSB = 1;
const unsigned char CC_BREATH_MSB = 2;
const unsigned char CC_FOOT_MSB = 4;
const unsigned char CC_PORTAMENTO_TIME_MSB = 5;
const unsigned char CC_DATA_ENTRY_MSB = 6;
const unsigned char CC_VOLUME_MSB = 7;
const unsigned char CC_BALANCE_MSB = 8;
const unsigned char CC_PAN_MSB = 10;
const unsigned char CC_EXPRESSION_MSB = 11;
const unsigned char CC_EFFECT_1_MSB = 12;
const unsigned char CC_EFFECT_2_MSB = 13;
const unsigned char CC_GP_1_MSB = 16;
const unsigned char CC_GP_2_MSB = 17;
const unsigned char CC_GP_3_MSB = 18;
const unsigned char CC_GP_4_MSB = 19;
const unsigned char CC_BANK_SELECT_LSB = 32;
const unsigned char CC_MOD_WHEEL_LSB = 33;
const unsigned char CC_BREATH_LSB = 34;
const unsigned char CC_FOOT_LSB = 36;
const unsigned char CC_PORTAMENTO_TIME_LSB = 37;
const unsigned char CC_DATA_ENTRY_LSB = 38;
const unsigned char CC_VOLUME_LSB = 39;
const unsigned char CC_BALANCE_LSB = 40;
const unsigned char CC_PAN_LSB = 42;
const unsigned char CC_EXPRESSION_LSB = 43;
const unsigned char CC_EFFECT_1_LSB = 44;
const unsigned char CC_EFFECT_2_LSB = 45;
const unsigned char CC_GP_1_LSB = 48;
const unsigned char CC_GP_2_LSB = 49;
const unsigned char CC_GP_3_LSB = 50;
const unsigned char CC_GP_4_LSB = 51;
const unsigned char CC_SUSTAIN_PEDAL = 64;
const unsigned char CC_PORTAMENTO_TOGGLE = 65;
const unsigned char CC_SOSTENUTO_TOGGLE = 66;
const unsigned char CC_SOFT_PEDAL_TOGGLE = 67;
const unsigned char CC_LEGATO_FOOT_SWITCH = 68;
const unsigned char CC_HOLD_2 = 69;
const unsigned char CC_VARIATION = 70;
const unsigned char CC_TIMBRE = 71;
const unsigned char CC_RELEASE_TIME = 72;
const unsigned char CC_ATTACK_TIME = 73;
const unsigned char CC_BRIGHTNESS = 74;
const unsigned char DECAY_TIME = 75;
const unsigned char CC_VIBRATO_RATE = 76;
const unsigned char CC_VIBRATO_DEPTH = 77;
const unsigned char CC_VIBRATO_DELAY = 78;
const unsigned char CC_GP_5 = 80;
const unsigned char CC_GP_6 = 81;
const unsigned char CC_GP_7 = 82;
const unsigned char CC_GP_8 = 83;  
const unsigned char CC_PORTAMENTO_CTRL = 84;
const unsigned char CC_FX1_DEPTH = 91;
const unsigned char CC_FX2_DEPTH = 92;
const unsigned char CC_FX3_DEPTH = 93;
const unsigned char CC_FX4_DEPTH = 94;
const unsigned char CC_FX5_DEPTH = 95;
const unsigned char CC_DATA_INCREMENT = 96;
const unsigned char CC_DATA_DECREMENT = 97;
const unsigned char CC_NRPN_LSB = 98;
const unsigned char CC_NRPN_MSB = 99;
const unsigned char CC_RPN_LSB = 100;
const unsigned char CC_RPN_MSB = 101;
const unsigned char CC_ALL_SOUND_OFF = 120;
const unsigned char CC_RESET_ALL_CTRL = 121;
const unsigned char CC_LOCAL_CONTROL_TOGGLE = 122;
const unsigned char CC_ALL_NOTES_OFF = 123;
const unsigned char CC_OMNI_OFF = 124;
const unsigned char CC_OMNI_ON = 125;
const unsigned char CC_MONO_ON = 126;
const unsigned char CC_POLY_ON = 127;

// Data2:
const int MIN_MIDI_VELOCITY = 1;   // a value of 0 would be a note off
const int MIDI_NOTE_OFF_VELOCITY = 0;

class MIDI
{
public:
MIDI();
MIDI( std::vector <unsigned char> vMIDIEvents, bool blnSysEx);
~MIDI();

bool ByteFromHexString( std::wstring strIn);

std::vector <unsigned char> LastCompleteMessage();
int ShortMsgStatus( std::vector <unsigned char> myMessage);
int ShortMsgStatus( int nCommandNibble, int nChannelNibble);
std::wstring StatusCommandString( std::vector <unsigned char> vBuffer);
int ShortMsgChannelNibble( std::vector <unsigned char> myMessage);
	int ShortMsgCommandNibble( std::vector <unsigned char> myMessage);
int ShortMsgData1( std::vector <unsigned char> myMessage);
int ShortMsgData2( std::vector <unsigned char> myMessage);
int ShortMsgData1HighNibble( std::vector <unsigned char> myMessage);
int ShortMsgData1LowNibble( std::vector <unsigned char> myMessage);
std::wstring ControllerString( unsigned char nData1);

std::vector <unsigned char> SysExManufacturerID( std::vector <unsigned char> vMessages);
// void SysExManufacturerID( std::vector <unsigned char> vManufacturerID);

int SysExDeviceNumber();
// void SysExDeviceNumber();

std::vector <unsigned char> SysExAddressBytes( std::vector <unsigned char> vMessages);
std::vector <unsigned char> SysExValueBytes( std::vector <unsigned char> vMessages);

bool IsValidStatus( std::vector <unsigned char> myMessage);
bool IsValidChannelNumber( std::vector <unsigned char> myMessage);
bool IsValidData1Value( std::vector <unsigned char> myMessage);
bool IsValidData2Value( std::vector <unsigned char> myMessage);
bool IsValidSysEx( std::vector <unsigned char> vMessages);
bool IsValidSysExHeader( std::vector <unsigned char> vMyBytes);

std::string MIDIHash(); 
std::string NibbledShortMIDIHash( std::vector <unsigned char> myMessage, int nNibbles);
int NibbledMessageValue( std::string strHash, std::vector <unsigned char> vBuffer);

private:
std::string ShortMIDIHash( std::vector <unsigned char> myMessage);
std::string SysExHash( std::vector <unsigned char> myMessage);
	
unsigned int nAddressBytes;
std::vector <unsigned char> vMIDIBytes;
};

#endif  // MIDI_H
