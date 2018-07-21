// RBSpeech.mm
//RBSpeech implementation for Mac

#include "RBSpeech.h"
#include <AppKit/NSSpeechSynthesizer.h>
#include <foundation/foundation.h>


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


HRESULT RBSpeech::GetActiveProduct(ActiveProduct& Product, DolphinProduct& SpecificDolphinProduct)
{
	Product = ID_MAC;
	SpecificDolphinProduct = ID_NOPRODUCT;
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
	 
	 
HRESULT RBSpeech::Speak( wstring wstrText, DolphinReturnCode& ReturnCode)
{
 if (IsMuted() == S_OK)
	{
 		return S_OK;
	}
	else
	{
		[impl->synth startSpeakingString: wxCFStringRef( wstrText).AsNSString()];
	}
	return S_OK;
}

HRESULT RBSpeech::Speak( std::wstring wstrText, bool blnSilence, DolphinReturnCode& ReturnCode)
{
	if (IsMuted() == S_OK)
	{
		return S_OK;
	}
	else
	{
		if (blnSilence == true)
		{
 			[impl->synth stopSpeaking];
		}
		
		[impl->synth startSpeakingString: wxCFStringRef( wstrText).AsNSString()];
	}
	return S_OK;
}

HRESULT RBSpeech::Silence(DolphinReturnCode& ReturnCode)
{
	if (IsMuted())
	{
		return S_OK;
	}
	
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

std::vector <JAWSParameter> AsJawsParameterVector( const boost::any  operand)
{
	std::vector <JAWSParameter> myParameters;
	
	try
	{
		myParameters = any_cast<std::vector <JAWSParameter> >( operand);
		return myParameters;
	}
	catch( const boost::bad_any_cast &)
	{
		return myParameters;
	}
}

int AsInt( const boost::any  operand)
{
    try
	{
		int nOut = any_cast<int>( operand);
		return nOut;
	}
	catch( const boost::bad_any_cast &)
	{
		return -1;
	}
}


std::wstring AsWString( const boost::any operand)
{
	try
	{
		std::wstring wstrOut = any_cast<std::wstring>( operand);
		return wstrOut;
	}
	catch( const boost::bad_any_cast &)
	{
		return wstrEmpty;
	}
}


bool IsWString(const boost::any & operand)
{
    return operand.type() == typeid( std::wstring);
}


bool IsInt(const boost::any & operand)
{
    return operand.type() == typeid(int);
}


bool IsJawsParameterVector(const boost::any & operand)
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

