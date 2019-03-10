// SurfaceAction.h
//Response to surface events


#ifndef SURFACE_ACTION_H
#define SURFACE_ACTION_H

#pragma once

#include "RBEnums.h"
#include "RBSpeech.h"

// Enumerations for the available action types
enum SurfaceActionType
{
		NoAction,
		ReadControlLabel,
		ReadControlState,
		ReadControlLabelPlusState,
ReadDisplay,
ReadDisplayLine,
ReadDisplayStrip,
ReadDisplaySection,
MIDIInfo,
ChangeSurfaceMode,
RunHotSpot,
RunScreenReaderFunction,
SpeakString,
ReadLEDIfOn,
ReadLEDIfOff,
first = NoAction,
last = ReadLEDIfOff
};


//Data types enum.
enum ActionParameterDataType 
{
	Type_Int, //integer.
	Type_String, //std::string.
	Type_WString, //std::wstring type.
	Type_Bool, //boolean datatype.
	Type_DolphinReturnCode, //Dolphin return code type.
};


class SurfaceAction
{
public:
SurfaceAction();
~SurfaceAction();

std::wstring GetSurfaceActionTypeDescription( SurfaceActionType myActionType);
 SurfaceActionType GetSurfaceActionType() const;
 void SetSurfaceActionType(SurfaceActionType NewType);
ScreenReaderActionType GetScreenReaderActionType();
void SetScreenReaderActionType( ScreenReaderActionType myNewScreenReaderActionType);
ActionInfoType GetActionInfoType();
void SetActionInfoType( ActionInfoType myNewActionInfoType);
FunctionType GetFunctionType();
void SetFunctionType( FunctionType myNewFunctionType);

 //functions to get/set values in the vector of action parameters
bool DoesIndexExist(unsigned int nIndex);
unsigned int GetParameterCount();
void SetParameter(ActionParameterDataType DataType, std::wstring wstrMyValue);
std::pair <ActionParameterDataType, std::wstring> GetParameter(unsigned int nIndex) const;
std::pair <std::wstring, FunctionType> GetScreenReaderActionCall();

private:
bool IsLiveModeNode();
bool IsScreenReaderModeNode();

friend std::ostream & operator<<( std::ostream &os, const SurfaceAction &SA);
friend std::istream & operator>>( std::istream &is, const SurfaceAction &SA);
friend class boost::serialization::access;
    
	template<class Archive>
void serialize( Archive & myArchive, const unsigned int version)
	{
const char * cSurfaceActionTypeTag = "SurfaceActionType";
myArchive & boost::serialization::make_nvp( cSurfaceActionTypeTag, mySurfaceActionType);
const char * cSurfaceActionParametersTag = "ActionParameters";
myArchive & boost::serialization::make_nvp( cSurfaceActionParametersTag, vActionParameters);

if(version > 0)
            {
const char * cScreenReaderActionTypeTag = "ScreenReaderActionType";
myArchive & boost::serialization::make_nvp( cScreenReaderActionTypeTag , myScreenReaderActionType);

/*
const char * cActionInfoTypeTag = "ActionInfoType";
myArchive & boost::serialization::make_nvp( cActionInfoTypeTag , myActionInfoType);
*/
}
}
		
// Internal storage
SurfaceActionType mySurfaceActionType; //Surface action type.
// ActiveProduct myScreenReader; //apply action to what product?
ScreenReaderActionType myScreenReaderActionType;  
ActionInfoType myActionInfoType;
FunctionType myFunctionType;  // Is this a script or a function, or neither? 

// Each entry in the vector has a string indicating the data type of the boost::any entry
std::vector <std::pair <ActionParameterDataType, std::wstring> > vActionParameters;
	};

// Set the class version for serialisation
BOOST_CLASS_VERSION(SurfaceAction, 1);
	
#endif