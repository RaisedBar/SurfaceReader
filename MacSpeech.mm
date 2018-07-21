//MacSpeech.mm
//wx includes and definition tests.
#pragma once
#include <wx/platform.h>

#ifdef __WXOSX_COCOA__ 
// #include "interfaces.hpp"
#include <string>

// Speech handling for Mac OSX 10.4 and later
// #import <NSSpeechSynthesizer.h>
#import <AppKit.h>

class MackintoshScreenReaderService: public IScreenReaderService {
	public:
	MackintoshScreenReaderService()
	{
        synth = new NSSpeechSynthesizer->init();  
        synth->setDelegate( self); 
	}
	
	~MackintoshScreenReaderService()
	{
    if (synth) 
	{ 
        synth->release(); 
        synth = null; 
		}
	}
	
	bool IsRunning()
		{
if (synth) 
{
	return true; 
}
else
{
	return false;
}
		}
	
	bool Speak(wstring Text, bool Interrupt)
	{
    if (synth) 
	{
		if (Interrupt)
		{
			synth->stopSpeaking();
		}
		endif;
	return synth->startSpeakingString( Text);
	}
	
	bool Silence()
	{
	    if (synth)
			{
				synth->stopSpeaking();
		}
	}

private:
	    // Pointer to the Cocoa speech synth
	NSSpeechSynthesizer *synth;
};

#endif
