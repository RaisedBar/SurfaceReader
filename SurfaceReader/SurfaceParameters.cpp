//SurfaceParameters.cpp

#include "SurfaceParameters.h"


SurfaceParameters::SurfaceParameters()
	:
m_SurfaceName( wstrEmpty),
		m_ProtocolID( strEmpty),
	wstrProtocolName( wstrEmpty),
m_HardwareInID( -1),
m_HardwareOutID( -1),
m_DisplayInID( -1),
m_DisplayOutID( -1),
m_HardwareInName( strEmpty),
m_HardwareOutName( strEmpty),
m_DisplayInName( strEmpty),
m_DisplayOutName( strEmpty),
m_SurfaceMode( ID_LIVE_MODE),
m_ReadDisplayDelay( MIN_READ_DISPLAY_DELAY),
// m_MIDIBufferSize( 0),
wstrAppConfigName( wstrEmpty)
{}


		std::wstring SurfaceParameters::GetSurfaceName()
{
	return m_SurfaceName;
}

void SurfaceParameters::SetSurfaceName( std::wstring wstrSurfaceName)
{
	m_SurfaceName = wstrSurfaceName;
}

std::string SurfaceParameters::GetProtocolID()
{
	return m_ProtocolID;
}

void SurfaceParameters::SetProtocolID( std::string strNewProtocolID)
{
	m_ProtocolID = strNewProtocolID;
}


std::wstring SurfaceParameters::GetProtocolName()
	{
		return wstrProtocolName;
}


void SurfaceParameters::SetProtocolName( std::wstring wstrNewProtocolName)
{
	wstrProtocolName = wstrNewProtocolName;
}


int SurfaceParameters::GetHardwareInID() 
	{
		return m_HardwareInID;
}

void SurfaceParameters::SetHardwareInID( int nNewID) 
	{
		m_HardwareInID = nNewID;
}

int SurfaceParameters::GetHardwareOutID() 
	{
return m_HardwareOutID;		
}

void SurfaceParameters::SetHardwareOutID( int nNewID) 
	{
m_HardwareOutID = nNewID;		
}

int SurfaceParameters::GetDisplayInID() 
	{
return m_DisplayInID;
}

void SurfaceParameters::SetDisplayInID( int nNewID) 
	{
m_DisplayInID = nNewID;
}

int SurfaceParameters::GetDisplayOutID() 
	{
		return m_DisplayOutID;
}

void SurfaceParameters::SetDisplayOutID( int nNewID) 
	{
		m_DisplayOutID = nNewID;
}

std::string SurfaceParameters::GetHardwareInName()
	{
		return m_HardwareInName;
}

void SurfaceParameters::SetHardwareInName( std::string strNewName)
	{
		m_HardwareInName = strNewName;
}

std::string SurfaceParameters::GetHardwareOutName() 
{
	return m_HardwareOutName;
}

void SurfaceParameters::SetHardwareOutName( std::string strNewName) 
{
m_HardwareOutName = strNewName;
}

std::string SurfaceParameters::GetDisplayInName()
{
	return m_DisplayInName;
}

void SurfaceParameters::SetDisplayInName( std::string strNewName)
{
m_DisplayInName = strNewName;
}

std::string SurfaceParameters::GetDisplayOutName()
{
	return m_DisplayOutName;
}

void SurfaceParameters::SetDisplayOutName( std::string strNewName)
{
	m_DisplayOutName = strNewName;
}

std::pair <std::wstring, std::wstring> SurfaceParameters::GetAppConfigID()
	{
return m_AppConfigID;
}

void SurfaceParameters::SetAppConfigID( std::pair <std::wstring, std::wstring> myAppConfigID)
{
	m_AppConfigID = myAppConfigID;
}


std::wstring SurfaceParameters::GetAppConfigName()
{
	return wstrAppConfigName;
}


void SurfaceParameters::SetAppConfigName( std::wstring wstrNewAppConfigName)
{
	wstrAppConfigName = wstrNewAppConfigName;
}


Mode SurfaceParameters::GetSurfaceMode()
{
	return m_SurfaceMode;
}

void SurfaceParameters::SetSurfaceMode( Mode myMode)
{
m_SurfaceMode = myMode;
}


int SurfaceParameters::GetReadDisplayDelay()
{
return m_ReadDisplayDelay;
}


void SurfaceParameters::SetReadDisplayDelay( int nNewDelay)
	{
			m_ReadDisplayDelay = nNewDelay;
			}


			