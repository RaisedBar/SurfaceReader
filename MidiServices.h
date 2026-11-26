// MidiServices.h
// Windows MIDI Services loopback ports, created through the SRMidiServices helper.
// Raised Bar Ltd.
// http://www.raisedbar.net

#ifndef SR_MIDI_SERVICES_H
#define SR_MIDI_SERVICES_H

#pragma once

#include <string>
#include <vector>

/* On Windows 11 25H2 and later, Windows MIDI Services can create loopback ports, as the Mac's CoreMIDI
can create virtual ports, so SurfaceReader no longer needs loopMIDI.

Each port SurfaceReader shares with a DAW is a loopback pair:
- side B has the name the DAW uses, for example "Mackie hardware", or the old loopMIDI port's name;
- side A is SurfaceReader's own end, the same name followed by " (SR)".
Classic MIDI 1.0 (WinMM) port names are cut to 31 characters, so names are fitted to that length, with the
" (SR)" added after shortening: otherwise both ends of a long name would look the same.
Whatever SurfaceReader sends to side A arrives at side B and vice versa, so the DAW sees one input and
one output with the shared name, exactly like a loopMIDI port. SurfaceReader still sends and receives
through RtMidi, on the ordinary MIDI 1.0 ports the service creates for each side.

The Windows MIDI Services API is 64-bit only, so the 32-bit SurfaceReader asks the 64-bit
SRMidiServices.exe helper (in the MidiServices folder next to SurfaceReader.exe) to manage the loopbacks.
Loopbacks are created for the current Windows session (and also saved, where the PC has a Windows MIDI
Services configuration file), so SurfaceReader recreates any that are missing each time it starts. */

namespace MidiServices
{
	struct Loopback
	{
		std::string strAssociationID;
		std::string strNameA;   // SurfaceReader's end
		std::string strNameB;   // the DAW's end
		bool blnSaved = false;
	};

	// True if Windows MIDI Services and its loopbacks are available. Checked once, then remembered.
	bool IsAvailable();

	// " (SR)"
	extern const char * const szSurfaceReaderSuffix;

	// WinMM's limit for a port name, in UTF-16 characters
	const size_t nMaxPortName = 31;

	// Shorten a UTF-8 name to at most nMaxLength UTF-16 characters, without splitting a character
	std::string FitPortName( const std::string & strName, size_t nMaxLength = nMaxPortName);

	// "Mackie hardware" -> "Mackie hardware (SR)", fitted to the length limit
	std::string SurfaceReaderPortName( const std::string & strDawPortName);

	// All loopbacks that exist now. Returns false if the helper couldn't be run or failed.
	bool ListLoopbacks( std::vector <Loopback> & vLoopbacks);

	// Create the loopback for a DAW port name (sides "<name> (SR)" and "<name>", fitted to the length limit)
	bool CreateLoopback( const std::string & strDawPortName, std::string & strError);

	bool RemoveLoopback( const std::string & strAssociationID, std::string & strError);
}

#endif  // SR_MIDI_SERVICES_H
