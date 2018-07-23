// SurfaceActions.h
//Responses to surface events


// #define _SCL_SECURE_NO_WARNINGS

#ifndef SURFACE_ACTIONS_H
#define SURFACE_ACTIONS_H

#pragma once

//#include <string>

//#include "RBException.hpp"
//#include "SRConstants.hpp"
#include "SurfaceAction.h"

//#include <boost/serialization/string.hpp>
//#include <boost/serialization/vector.hpp>
//#include <boost/serialization/utility.hpp>
//#include <boost/serialization/split_member.hpp>
//#include <boost/serialization/tracking.hpp>
//#include <boost/serialization/base_object.hpp>
//#include <boost/serialization/nvp.hpp>
//#include <boost/serialization/export.hpp>
//#include <boost/archive/xml_woarchive.hpp>
//#include <boost/archive/xml_wiarchive.hpp>

//#include <wx/msw/winundef.h> 


class SurfaceActions
{
public:
SurfaceActions();
~SurfaceActions();

void SetLiveModeAction( SurfaceAction myAction);
SurfaceAction GetLiveModeAction( unsigned int nIndex) const;
void SetScreenReaderModeAction( SurfaceAction myAction);
SurfaceAction GetScreenReaderModeAction( unsigned int nIndex) const;


int GetLiveModeActionCount();
int GetScreenReaderModeActionCount();

bool DeleteLiveModeAction( int nIndex);
bool DeleteScreenReaderModeAction( int nIndex);
std::vector <std::wstring> DeleteScreenReaderActionTypes();

bool SwapLiveModeActions( int nIndex1, int nIndex2);
bool SwapScreenReaderModeActions( int nIndex1, int nIndex2);

private:
friend std::ostream & operator<<( std::ostream &os, const SurfaceActions &SA);
friend std::istream & operator>>( std::istream &is, const SurfaceActions &SA);
friend class boost::serialization::access;
    
	template<class Archive>
void serialize( Archive & myArchive, const unsigned int version)
	{
const char * cLiveModeActionsTag= "LiveModeActions";
myArchive & boost::serialization::make_nvp( cLiveModeActionsTag, myLiveModeActions);
const char * cScreenReaderModeActionsTag= "ScreenReaderModeActions";
myArchive & boost::serialization::make_nvp( cScreenReaderModeActionsTag, myScreenReaderModeActions);
}
		
// Internal storage
	std::vector <SurfaceAction> myLiveModeActions;
	std::vector <SurfaceAction> myScreenReaderModeActions;
};

#endif

