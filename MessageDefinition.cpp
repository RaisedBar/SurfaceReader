// MessageDefinition.cpp
//Implementation of the MessageDefinition Class.

#define _HAS_STD_BYTE 0

#include "MessageDefinition.h"

	
MessageDefinition::MessageDefinition()
	{
// Label for this item
		wstrLabel.clear();
		// SysEx address bytes
		vSysExAddressBytes.clear();
// Actions to be performed when a given control is changed, indexed on the access product
		myActions.clear();
// Defined states for this control:
		StateLabels.clear();
// Is this a modifier button that transmits its own MIDI message?
blnMIDIModifier = false;
// Is this a button that is being held down?
blnPressed = false;
}


MessageDefinition::	MessageDefinition( std::wstring wstrNewLabel)
{
// Label for this item
		wstrLabel = wstrNewLabel;
		// SysEx address bytes
		vSysExAddressBytes.clear();
// Actions to be performed when a given control is changed, indexed on the access product
		myActions.clear();
// Defined states for this control:
		StateLabels.clear();
// Is this a modifier button that transmits its own MIDI message?
blnMIDIModifier = false;
// Is this a button that is being held down?
blnPressed = false;
}

	
MessageDefinition::~MessageDefinition()
	{}


std::wstring MessageDefinition::GetLabel()
{
	return wstrLabel;
}


void MessageDefinition::SetLabel( std::wstring wstrNewLabel)
{
	wstrLabel = wstrNewLabel;
}


std::vector <unsigned char> MessageDefinition::GetSysExAddressBytes()
{
return vSysExAddressBytes;
}


void MessageDefinition::SetSysExAddressBytes( std::vector <unsigned char> vMyAddressBytes)
{
	vSysExAddressBytes = vMyAddressBytes;
}


std::wstring MessageDefinition::GetState( std::vector <unsigned char> vStateBytes)
{
	std::map <std::vector <unsigned char>, std::wstring>::iterator it;
	
	it = StateLabels.find( vStateBytes);

	if (it != StateLabels.end())
	{
		return it->second;
	}
	else
	{
std::wstring wstrOut;

for (unsigned int i = 0; i < vStateBytes.size(); i++)
{
		wstrOut.append( boost::lexical_cast <std::wstring> ( vStateBytes.at( i)));
		}

return wstrOut;
		}
}


void MessageDefinition::SetState( std::vector <unsigned char> vStateBytes, std::wstring wstrStateLabel)
{
	std::map<std::vector <unsigned char>, std::wstring>::iterator it;
	it  = StateLabels.find( vStateBytes);

	if (it != StateLabels.end())
	{
		StateLabels.erase(it);
	}

	std::pair <std::vector <unsigned char>, std::wstring> myPair;
	myPair = std::make_pair( vStateBytes, wstrStateLabel);
	StateLabels.insert(myPair);
}


bool MessageDefinition::DeleteState( std::vector <unsigned char> vStateBytes)
{
std::map<std::vector <unsigned char>, std::wstring>::iterator it;
	it  = StateLabels.find( vStateBytes);

	if (it != StateLabels.end())
	{
		StateLabels.erase( it);
		return true;
	}
	else
	{
		return false;
	}
	}


SurfaceActions MessageDefinition::GetSurfaceActions( ActiveProduct ap)
{
std::map <ActiveProduct, SurfaceActions>::iterator it;
it = myActions.find( ap);

if (it != myActions.end())
	{
		return it->second;
}
else
{
throw RBException( wstrNoSurfaceActionsAvailableError);
	return it->second;
}
}


void MessageDefinition::SetSurfaceActions( ActiveProduct ap,SurfaceActions mySurfaceActions)
{
std::map < ActiveProduct,SurfaceActions> ::iterator it;
	it = myActions.find(ap);

		if (it != myActions.end())
		{
			myActions.erase( it);
		}

	std::pair <ActiveProduct, SurfaceActions> myPair;
	myPair = std::make_pair( ap, mySurfaceActions);
myActions.insert( myPair);
}


void MessageDefinition::SetLiveModeAction( ActiveProduct myProduct, SurfaceAction myAction)
{
	std::map <ActiveProduct, SurfaceActions>::iterator it;
	
it = myActions.find( myProduct);

if (it != myActions.end())
{
it->second.SetLiveModeAction( myAction);
}
else
{
std::pair <ActiveProduct, SurfaceActions> myPair;
SurfaceActions mySurfaceActions;

mySurfaceActions.SetLiveModeAction( myAction);
myPair = std::make_pair( myProduct, mySurfaceActions);
myActions.insert( myPair);
	}
}


void MessageDefinition::SetScreenReaderModeAction( ActiveProduct myProduct, SurfaceAction myAction)
{
	std::map <ActiveProduct, SurfaceActions>::iterator it;
	
it = myActions.find( myProduct);

if (it != myActions.end())
{
	it->second.SetScreenReaderModeAction( myAction);
}
else
{
std::pair <ActiveProduct, SurfaceActions> myPair;
SurfaceActions mySurfaceActions;

mySurfaceActions.SetLiveModeAction( myAction);
myPair = std::make_pair( myProduct, mySurfaceActions);
myActions.insert( myPair);
	}
}


int MessageDefinition::GetStateCount()
{
	return StateLabels.size();
}


std::vector <std::wstring> MessageDefinition::GetStateLabels()
{
	std::map <std::vector <unsigned char>, std::wstring>::iterator it;
std::vector <std::wstring> myStateLabels;

for (it = StateLabels.begin(); it != StateLabels.end(); it++)
{
		std::wstring wstrStateName = it->second;
	/*
	wstrStateName.append( wstrSpacedEquals);
	std::string strHexByte = BytesToHex( it->first);
	std::wstring wstrStateValue( strHexByte.begin(), strHexByte.end());
wstrStateName.append( wstrStateValue);
*/
		myStateLabels.push_back( wstrStateName); 
}

return myStateLabels;
}


std::vector <unsigned char> MessageDefinition::GetStateBytes( int nItem)
	{
std::map <std::vector <unsigned char>, std::wstring> ::iterator it;
std::vector <unsigned char> vBytes;
int i = 0;

for (it = StateLabels.begin(); it != StateLabels.end(); it++)
{
	if (i == nItem)
	{
		vBytes = it->first;
	break;
	}  // end if correct location
i++;
}  // end for

return vBytes;
}


void MessageDefinition::SetMIDIModifier( bool blnShift)
{
blnMIDIModifier = blnShift;
}


bool MessageDefinition::GetMIDIModifier()
{
return blnMIDIModifier;
}


bool MessageDefinition::GetButtonPressed()
{
return blnPressed;
}


void MessageDefinition::SetButtonPressed( bool blnPressedState)
{
	blnPressed = blnPressedState;
}


