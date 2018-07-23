// SpeechParameters.h
// 

#ifndef SPEECH_PARAMETERS_H
#define SPEECH_PARAMETERS_H

#pragma once

#ifdef __WXOSX_COCOA__   

#include <string>

#include "SRConstants.h"
#include "RBEnums.h"

#include <boost/serialization/access.hpp>
#include <boost/serialization/nvp.hpp>

#include <wx/msw/winundef.h>


class SpeechParameters
{
public:
	SpeechParameters()
	:
wstrVoiceName( wstrDefaultVoiceName),
fRate( 200),
fVolume( 0.5)
	{};

std::wstring GetVoice()
{
	return wstrVoiceName;
};

	void SetVoice( std::wstring wstrNewVoiceName)
{
	wstrVoiceName = wstrNewVoiceName;
};

float GetRate()
{
	return fRate;
};

void SetRate( float fNewRate)
{
	fRate = fNewRate;
};

float GetVolume()
	{
		return fVolume;
};

void SetVolume( float fNewVolume) 
	{
		fVolume = fNewVolume;
};

private:
friend std::ostream & operator<<( std::ostream &os, const SpeechParameters &SP);
friend std::istream & operator>>( std::istream &is, const SpeechParameters &SP);
friend class boost::serialization::access;
template<class Archive>
void serialize(Archive & ar, const unsigned int version)
{
try
	{
		const char* cVoiceNameTag ="VoiceName";
ar & boost::serialization::make_nvp(cVoiceNameTag, wstrVoiceName);
const char* cRateTag ="Rate";
ar & boost::serialization::make_nvp(cRateTag, fRate);
const char* cVolumeTag ="Volume";
ar & boost::serialization::make_nvp(cVolumeTag, fVolume);
}  // end try
catch( ...)
{
throw ID_LOAD_DATA_EXCEPTION;
}  // end catch
}
	
	// Internal storage
std::wstring wstrVoiceName;
float fRate, fVolume;
};
#endif  // if cocoa
#endif  // define
