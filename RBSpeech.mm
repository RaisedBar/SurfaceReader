// RBSpeech.mm
//RBSpeech implementation for Mac

#include "RBSpeech.h"
#include <AppKit/AppKit.h>   // NSSpeechSynthesizer, NSWorkspace, NSAccessibility
#include <Foundation/Foundation.h>


/* the indirection avoids declaring the variable of type
 NSSpeechSynthesizer* (which is an Obj-C type) in the header */

struct SpeechSynthesizerImpl
{
	NSSpeechSynthesizer* synth;
};

RBSpeech::RBSpeech()
:// instantiate the private implementation (PIMPL) object
impl(new SpeechSynthesizerImpl()),
blnMutedSpeech( false)
{
	// instantiate the wrapped synthesizer
	impl->synth = [[NSSpeechSynthesizer alloc] init];
	
	wxstrVoiceName = wstrDefaultVoiceName;
	fRate = 300.0;
	fVolume = 0.75;
	
SetVoice ( wxstrVoiceName);
SetRate( fRate);
SetVolume( fVolume);
}


RBSpeech::RBSpeech( const wxString wxstrVoice):
// instantiate the private implementation (PIMPL) object
impl(new SpeechSynthesizerImpl()),
blnMutedSpeech( false)
{
	// instantiate the wrapped synthesizer
	impl->synth = [[NSSpeechSynthesizer alloc] initWithVoice: wxCFStringRef( wxstrVoice).AsNSString()];
}


RBSpeech::~RBSpeech()
{
	[impl->synth release];
	delete impl;
}


float RBSpeech::GetRate() const
{
	return [impl->synth rate];
}


void RBSpeech::SetRate( float myRate)
{
	fRate = myRate;
	[impl->synth setRate:fRate];
}


float RBSpeech::GetVolume() const
{
	return [impl->synth volume];
}


void RBSpeech::SetVolume( float myVolume)
{
	fVolume = myVolume;
	[impl->synth setVolume:fVolume];
}


unsigned long RBSpeech::GetVoiceCount()
{
	 NSAutoreleasePool * pool = [[NSAutoreleasePool alloc] init];
/* 	 NSArray * aVoices = [[NSSpeechSynthesizer alloc] init]; */
	NSArray * aVoices;
	aVoices = [NSSpeechSynthesizer availableVoices];
	 unsigned long nCount = [aVoices count];
	 [pool release];
	return nCount;
}


wxString RBSpeech::GetVoice( unsigned int nVoiceIndex)
{
	wxString wxstrVoice;
	 NSAutoreleasePool * pool = [[NSAutoreleasePool alloc] init];
	//  NSArray *aVoices = [[NSSpeechSynthesizer alloc] init];
	NSArray *aVoices;
	 aVoices = [NSSpeechSynthesizer availableVoices];

	NSString * nsstrVoice = [aVoices objectAtIndex:nVoiceIndex];
	// long nVoiceNumber =[aVoices
	
	 wxstrVoice =  wxCFStringRef::AsString( nsstrVoice);
	 wxstrVoiceName = wxstrVoice;
	 [pool release];
	 return wxstrVoice;
	 }
	 
	 
	 wxString RBSpeech::GetVoice() const
	 {
	 // return wxCFStringRef( [impl->synth voice]).AsString();
	 return wxstrVoiceName;
	 }
	 

	 void RBSpeech::SetVoice( wxString wxstrVoice)
	 {
	 wxstrVoiceName = wxstrVoice;
	 [impl->synth setVoice:wxCFStringRef( wxstrVoice).AsNSString()];
	 }
	 
	 
	 bool RBSpeech::LoadSpeechParameters()
	 {
	 bool blnResult = false;
	 SpeechParameters myParameters;
	 path myPath( SpeechPath());
		 
		 try
		 {
			 if (! exists( myPath))    // does the path actually exist?
			 {
				 return blnResult;
			 }  // if exists

			 if (! is_directory( myPath))      // myPath must be a directory
			 {
				 return blnResult;
			 }  // if directory
		 }  // end try
		 catch (const filesystem_error &error)
		 {
			 wxMessageBox( error.what(), wstrErrorTitle, wxOK | wxICON_ERROR);
			 return blnResult;
		 }

		 // Directory exists so process the desired file
		 myPath /= wstrSpeechConfigFileName;

		 try
	 {
	 myParameters = LoadData <SpeechParameters> (myPath, false);
		 blnResult = true;
	 }
	 catch( ...)
	 {
	 return blnResult;
	 }
	 
	 if (blnResult)
	 {
	 SetVoice( myParameters.GetVoice());
	 SetRate( myParameters.GetRate());
	 SetVolume( myParameters.GetVolume());
	 }
	 
	return blnResult;
}


bool RBSpeech::SaveSpeechParameters()
{
	bool blnResult = false;
	 SpeechParameters myParameters;
	path myPath( SpeechPath());
	
	try
	{
		if (! exists( myPath))    // does the path actually exist?
		{
			return blnResult;
		}  // if exists
		
		if (! is_directory( myPath))      // myPath must be a directory
		{
			return blnResult;
		}  // if directory
	}  // end try
	catch (const filesystem_error &error)
	{
		wxMessageBox( error.what(), wstrErrorTitle, wxOK | wxICON_ERROR);
		return blnResult;
	}
	
	// Directory exists so process the desired file
	myPath /= wstrSpeechConfigFileName;
	
	 myParameters.SetVoice( wxstrVoiceName.ToStdWstring());
	 myParameters.SetRate( fRate);
	 myParameters.SetVolume( fVolume);
	 
	 try
	 {
	 SaveData <SpeechParameters> ( myParameters, myPath, false);
		 blnResult = true;
		 return blnResult;
	 }
	 catch( ...)
	 {
		 return blnResult;	 }
}


HRESULT RBSpeech::GetActiveProduct(ActiveProduct& Product)
{
	Product = ID_MAC;
	return S_OK;
}


	 HRESULT RBSpeech::IsMuted()
	 {
	 if (blnMutedSpeech == true)
	 {
	 return S_OK;
	 }
	 else
	 {
	 return S_FALSE;
	 }
	 }
	 
	 
	 void RBSpeech::Mute( bool blnMute)
	 {
	 blnMutedSpeech = blnMute;
	 }
	 
	 
// True when VoiceOver is running (macOS 10.13 and later can tell us)
static bool IsVoiceOverRunning()
{
	if (@available(macOS 10.13, *))
	{
		return [[NSWorkspace sharedWorkspace] isVoiceOverEnabled];
	}
	return false;
}


// Ask VoiceOver to speak the text. VoiceOver reads it in the user's own voice and
// settings and shows it on a braille display, instead of a second synthesizer
// talking over VoiceOver. High priority interrupts what VoiceOver is saying.
static void PostVoiceOverAnnouncement( const std::wstring & wstrText, bool blnInterrupt)
{
	wxCFStringRef cfText( wstrText);  // keeps the NSString alive until the notification is posted
	NSDictionary * info = @{
		NSAccessibilityAnnouncementKey: cfText.AsNSString(),
		NSAccessibilityPriorityKey: @(blnInterrupt ? NSAccessibilityPriorityHigh : NSAccessibilityPriorityMedium)
	};
	NSAccessibilityPostNotificationWithUserInfo( NSApp, NSAccessibilityAnnouncementRequestedNotification, info);
}


HRESULT RBSpeech::Speak( std::wstring wstrText)
{
	return Speak( wstrText, false);
}

HRESULT RBSpeech::Speak( std::wstring wstrText, bool blnSilence)
{
	if (IsMuted() == S_OK)
	{
		return S_OK;
	}

	if (wstrText.empty())
	{
		return S_FALSE;
	}

	if (IsVoiceOverRunning())
	{
		PostVoiceOverAnnouncement( wstrText, blnSilence);
		return S_OK;
	}

	// No VoiceOver: speak with our own synthesizer
	if (blnSilence == true)
	{
		[impl->synth stopSpeaking];
	}

	[impl->synth startSpeakingString: wxCFStringRef( wstrText).AsNSString()];
	return S_OK;
}

HRESULT RBSpeech::Silence()
{
	// IsMuted returns S_OK (0) when muted, so this used to test the wrong way round
	// and never stopped speech while unmuted
	if (IsMuted() == S_OK)
	{
		return S_OK;
	}

	// VoiceOver has no "stop announcement" request; a new high-priority announcement replaces it
	[impl->synth stopSpeaking];
	return S_OK;
}


HRESULT RBSpeech::GetAvailableActions(AvailableActionsType& ActionInformation)
{
	return S_OK;
}


HRESULT RBSpeech::ExecuteAction(std::wstring Action, ScreenReaderActionType Type)
{
	return S_OK;
}


	 HRESULT RBSpeech::ExecuteHotSpot(std::wstring Set, std::wstring SpotName)
	 {
	 return S_OK;
	 }
	 
	 
	 void RBSpeech::ClearHscFile()
	 {}
	 
	 
	 void RBSpeech::SetHscFile(wxString File)
	 {}
	 
	 
	 HRESULT RBSpeech::ListHotSpotsInSet(std::wstring SetName, std::vector<std::string>& Spots)
	 {
	 return S_OK;
	 }
	 
	 
	 void RBSpeech::SetFirstJsdFile(wxString File)
	 {}
	 
	 
	 wxString RBSpeech::GetFirstJsdFile(void)
	 {
	 return wxEmptyString;
	 }
	 
	 
	 void RBSpeech::ClearJsdFile()
	 {}


// Variant-handling

std::vector <JAWSParameter> AsJawsParameterVector( const std::any  operand)
{
	std::vector <JAWSParameter> myParameters;
	
	try
	{
		myParameters = std::any_cast<std::vector <JAWSParameter> >( operand);
		return myParameters;
	}
	catch( const std::bad_any_cast &)
	{
		return myParameters;
	}
}

int AsInt( const std::any  operand)
{
    try
	{
		int nOut = std::any_cast<int>( operand);
		return nOut;
	}
	catch( const std::bad_any_cast &)
	{
		return -1;
	}
}


std::wstring AsWString( const std::any operand)
{
	try
	{
		std::wstring wstrOut = std::any_cast<std::wstring>( operand);
		return wstrOut;
	}
	catch( const std::bad_any_cast &)
	{
		return wstrEmpty;
	}
}


bool IsWString(const std::any & operand)
{
    return operand.type() == typeid( std::wstring);
}


bool IsInt(const std::any & operand)
{
    return operand.type() == typeid(int);
}


bool IsJawsParameterVector(const std::any & operand)
{
    return operand.type() == typeid( std::vector <JAWSParameter>);
}


int RBSpeech::GetVoiceIndex( wxString wxstrVoiceName)
{
	NSArray * aVoices;
	aVoices = [NSSpeechSynthesizer availableVoices];

	NSInteger nIndex = [aVoices indexOfObject: wxCFStringRef( wxstrVoiceName).AsNSString()];

	if (nIndex == NSNotFound)
	{
		return -1;
	}
	else
	{
		return nIndex;
	}
}

