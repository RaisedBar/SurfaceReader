// SurfaceAction.cpp
//Response to surface events

#define _HAS_STD_BYTE 0

#include "SurfaceAction.h"


SurfaceAction::SurfaceAction()
{
mySurfaceActionType =NoAction;
myScreenReaderActionType = ID_SCRIPT;
myFunctionType = ID_TYPE_NONE;
vActionParameters.clear();
}


SurfaceAction::~SurfaceAction()
{}


std::wstring SurfaceAction::GetSurfaceActionTypeDescription( SurfaceActionType myNewSurfaceActionType)
{
	switch (myNewSurfaceActionType)
{
case NoAction:
	{
		return wstrNoAction;
	}
	break;

case ReadControlLabel:
{
		return wstrReadControlLabelAction;
	}
	break; 

case ReadControlState:
{
		return wstrReadControlStateAction;
	}
	break;

case ReadControlLabelPlusState:
{
		return wstrReadControlNamePlusStateAction;
	}
	break;

case ReadDisplay:
{
	return wstrReadDisplayAction;
}
break;

case ReadDisplayLine:
{
	return wstrReadDisplayLineAction;
}
break;

case ReadDisplayStrip:
{
	return wstrReadDisplayStripAction;
}
break;

case ReadDisplaySection:
{
	return wstrReadDisplaySectionAction;
}
break;

case MIDIInfo:
	{
			return wstrMIDIInfoAction;
	}
	break;

case ChangeSurfaceMode:
{
		return wstrChangeSurfaceModeAction;
	}
	break;

case RunHotSpot:
{
		return wstrRunHotSpotAction;
	}
	break;

case RunScreenReaderFunction:
{
		return wstrScreenReaderAction;
	}
	break;

	case SpeakString:
{
		return wstrSpeakStringAction;
	}
	break;

	case ReadLEDIfOn:
		{
				return wstrReadLEDIfOnAction;
	}
		break;

	case ReadLEDIfOff:
		{
		return wstrReadLEDIfOffAction;
	}
		break;

		default:
	{
	std::wstring wstrEmpty;
	wstrEmpty.clear();
	return wstrEmpty;
	}
	break;
	} // end switch
}


SurfaceActionType SurfaceAction::GetSurfaceActionType() const
 {
return mySurfaceActionType;
 }

 
void SurfaceAction::SetSurfaceActionType(SurfaceActionType NewType)
{
	mySurfaceActionType =NewType;
}

 
 bool SurfaceAction::DoesIndexExist(unsigned int nIndex)
{
	if ((nIndex > -1) && (nIndex < vActionParameters.size()))
		{
			return true;
	}
	else
	{
		return false;
	}
}


void SurfaceAction::SetParameter(ActionParameterDataType DataType, std::wstring wstrMyValue)
{
	std::pair <ActionParameterDataType, std::wstring> myParameter;
	myParameter = std::make_pair(DataType, wstrMyValue);
	vActionParameters.push_back( myParameter);
}


std::pair <ActionParameterDataType, std::wstring> SurfaceAction::GetParameter(unsigned int nIndex) const
{
	if ((nIndex < 0) || (nIndex >= vActionParameters.size()))
	{
throw ID_NO_ACTION_PARAMETER;	
}
return vActionParameters.at( nIndex);
}


std::pair <std::wstring, FunctionType> SurfaceAction::GetScreenReaderActionCall()
{
std::wstring wstrCall;
wstrCall.clear();

if ((mySurfaceActionType == RunHotSpot)
	|| (mySurfaceActionType == RunScreenReaderFunction))
{
// std::vector <std::pair <ActionParameterDataType, std::wstring> > vActionParameters;

	for (unsigned int i = 0; i < vActionParameters.size(); i++)
	{
ActionParameterDataType myParameterType = vActionParameters.at( i).first;		
		
		// Add a space separator if not the first parameter:
if (i > 0)
	{
		wstrCall.append( wstrSpace);		
}

switch (myParameterType)
		{
		case Type_Int:
			case Type_Bool:
				case Type_DolphinReturnCode:
{
				wstrCall.append( vActionParameters.at( i).second);
			}
			break;

		case 	Type_String:
						case Type_WString:
						{
							wstrCall.append( wstrDoubleQuotes);
							wstrCall.append( vActionParameters.at( i).second);
							wstrCall.append( wstrDoubleQuotes);
}
			break;

							default:
			{}
		break;
		}  // end switch
	}  // end for
}  // end if a screen-reader action

std::pair <std::wstring, FunctionType> myPair;
myPair = std::make_pair( wstrCall, myFunctionType);
return myPair;
}


unsigned int SurfaceAction::GetParameterCount()
{
	return vActionParameters.size();
}


ActionInfoType SurfaceAction::GetActionInfoType()
{
return myActionInfoType;
}


void SurfaceAction::SetActionInfoType( ActionInfoType myNewActionInfoType)
{
myActionInfoType = myNewActionInfoType;
}


ScreenReaderActionType SurfaceAction::GetScreenReaderActionType()
{
	return myScreenReaderActionType;
}


void SurfaceAction::SetScreenReaderActionType( ScreenReaderActionType myNewScreenReaderActionType)
{
	myScreenReaderActionType = myNewScreenReaderActionType;
}

