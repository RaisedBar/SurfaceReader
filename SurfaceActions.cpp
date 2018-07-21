// SurfaceActions.cpp
//Implementation of the SurfaceActions class


#include "SurfaceActions.h"


SurfaceActions::SurfaceActions()
{
	myLiveModeActions.clear();
	myScreenReaderModeActions.clear();
}


SurfaceActions::~SurfaceActions()
{}


void SurfaceActions::SetLiveModeAction( SurfaceAction myAction)
{
	myLiveModeActions.push_back( myAction);
}


bool SurfaceActions::SwapLiveModeActions( int nIndex1, int nIndex2)
{
if ((nIndex1 < 0)
	|| ( (unsigned int) nIndex1 >= myLiveModeActions.size())
	|| (nIndex2 <= nIndex1)
	|| ( (unsigned int) nIndex2 >= myLiveModeActions.size()))
{
return false;
}

SurfaceAction myTempAction = myLiveModeActions.at( nIndex1);
myLiveModeActions.at( nIndex1) = myLiveModeActions.at( nIndex2);
myLiveModeActions.at( nIndex2) = myTempAction;
return true;
}


bool SurfaceActions::SwapScreenReaderModeActions( int nIndex1, int nIndex2)
{
if ((nIndex1 < 0)
	|| ( (unsigned int) nIndex1 >= myScreenReaderModeActions.size())
	|| (nIndex2 <= nIndex1)
	|| ( (unsigned int) nIndex2 >= myScreenReaderModeActions.size()))
{
return false;
}

SurfaceAction myTempAction = myScreenReaderModeActions.at( nIndex1);
myScreenReaderModeActions.at( nIndex1) = myScreenReaderModeActions.at( nIndex2);
myScreenReaderModeActions.at( nIndex2) = myTempAction;
return true;
}


SurfaceAction SurfaceActions::GetLiveModeAction( unsigned int nIndex) const
{
if ((myLiveModeActions.size() > 0)
		&& (nIndex < myLiveModeActions.size()))
		{
			return myLiveModeActions[ nIndex];
	}
	else
{
throw RBException( wstrNoLiveModeActionError);
return SurfaceAction();
	}
}


void SurfaceActions::SetScreenReaderModeAction( SurfaceAction myAction)
{
	myScreenReaderModeActions.push_back( myAction);
}


SurfaceAction SurfaceActions::GetScreenReaderModeAction( unsigned int nIndex) const
{
if ((myScreenReaderModeActions.size() > 0)
	&& (nIndex < myScreenReaderModeActions.size()))
		{
			return myScreenReaderModeActions[ nIndex];
	}
	else
{
	throw RBException( wstrNoScreenReaderModeActionError);
return SurfaceAction();
	}
}


int SurfaceActions::GetLiveModeActionCount()
{
	return myLiveModeActions.size();
}


int SurfaceActions::GetScreenReaderModeActionCount()
{
	return myScreenReaderModeActions.size();
}


bool SurfaceActions::DeleteLiveModeAction( int nIndex)
{
	if ((nIndex >= 0)
		&& ( (unsigned int) nIndex < myLiveModeActions.size()))
	{
		myLiveModeActions.erase( myLiveModeActions.begin() + nIndex);
return true;
	}
	else
	{
		return false;
	}
}


bool SurfaceActions::DeleteScreenReaderModeAction( int nIndex)
{
	if ((nIndex >= 0)
		&& ( (unsigned int) nIndex < myLiveModeActions.size()))
	{
		myScreenReaderModeActions.erase( myScreenReaderModeActions.begin() + nIndex);
return true;
}
	else
	{
		return false;
	}
}



std::vector <std::wstring> SurfaceActions::DeleteScreenReaderActionTypes()
{
std::vector <std::wstring> vDeletedActions;
std::vector <SurfaceAction>::iterator it;

for (it = myLiveModeActions.begin(); it != myLiveModeActions.end(); it++)
{
	SurfaceActionType mySurfaceActionType = it->GetSurfaceActionType();

	if ((mySurfaceActionType == RunHotSpot)
|| (mySurfaceActionType == RunScreenReaderFunction))
	{
std::wstring wstrAction( wstrLiveMode);
wstrAction.append( wstrSpacedColon);
wstrAction.append( it->GetSurfaceActionTypeDescription( mySurfaceActionType));
vDeletedActions.push_back( wstrAction);
myLiveModeActions.erase( it);
	}
		}


for (it = myScreenReaderModeActions.begin(); it != myScreenReaderModeActions.end(); it++)
{
SurfaceActionType mySurfaceActionType = it->GetSurfaceActionType();

	if ((mySurfaceActionType == RunHotSpot)
|| (mySurfaceActionType == RunScreenReaderFunction))
	{
std::wstring wstrAction( wstrScreenReaderMode);
wstrAction.append( wstrSpacedColon);
wstrAction.append( it->GetSurfaceActionTypeDescription( mySurfaceActionType));
vDeletedActions.push_back( wstrAction);
myScreenReaderModeActions.erase( it);
		}
}

return vDeletedActions;
}

