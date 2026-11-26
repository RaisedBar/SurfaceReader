// SROptions.h
// 

#ifndef SURFACE_READER_OPTIONS_H
#define SURFACE_READER_OPTIONS_H

#pragma once

#include <boost/serialization/access.hpp>
#include <boost/serialization/nvp.hpp>

#include <wx/msw/winundef.h> 


class SurfaceReaderOptions
{
public:
	SurfaceReaderOptions()
	:
blnStartSized( true),
blnMidiServicesSeen( false)
	{};

// Set once SurfaceReader has found Windows MIDI Services and replaced any loopMIDI ports
bool GetMidiServicesSeen()
{
	return blnMidiServicesSeen;
};

void SetMidiServicesSeen( bool blnSeen)
{
	blnMidiServicesSeen = blnSeen;
};

bool GetStartSized()
{
	return blnStartSized;
};

	void SetStartSized( bool blnNewStartSized)
{
	blnStartSized = blnNewStartSized;
};

private:
friend std::ostream & operator<<( std::ostream &os, const SurfaceReaderOptions &SRO);
friend std::istream & operator>>( std::istream &is, const SurfaceReaderOptions &SRO);
friend class boost::serialization::access;
template<class Archive>
void serialize(Archive & ar, const unsigned int version)
{
try
	{
		const char* cStartSizedTag ="StartSized";
ar & boost::serialization::make_nvp(cStartSizedTag, blnStartSized);

		if (version >= 1)
		{
			const char* cMidiServicesSeenTag ="MidiServicesSeen";
			ar & boost::serialization::make_nvp(cMidiServicesSeenTag, blnMidiServicesSeen);
		}
}  // end try
catch( ...)
{
throw ID_LOAD_DATA_EXCEPTION;
}  // end catch
}
	
	// Internal storage
bool blnStartSized;
bool blnMidiServicesSeen;
};

#include <boost/serialization/version.hpp>
BOOST_CLASS_VERSION(SurfaceReaderOptions, 1)
#endif  // define