// UiaAnnouncer.cpp
// Speaks text through the user's screen reader by raising UI Automation notification events.
// Raised Bar Ltd.
// http://www.raisedbar.net

#include "UiaAnnouncer.h"

#ifdef _WIN32

// atlcore.h casts FARPROC to function pointers internally (warning C4191)
#pragma warning( push)
#pragma warning( disable: 4191)
#include <atlbase.h>
#pragma warning( pop)
#include <UIAutomation.h>

// The UIA functions are looked up at run time so SurfaceReader still starts on
// versions of Windows that don't have them (UiaRaiseNotificationEvent needs Windows 10 1709).
typedef HRESULT (WINAPI * UiaRaiseNotificationEventFunc)( IRawElementProviderSimple * provider, NotificationKind notificationKind, NotificationProcessing notificationProcessing, BSTR displayString, BSTR activityId);
typedef HRESULT (WINAPI * UiaHostProviderFromHwndFunc)( HWND hwnd, IRawElementProviderSimple ** ppProvider);
typedef BOOL (WINAPI * UiaClientsAreListeningFunc)( void);
typedef HRESULT (WINAPI * UiaDisconnectProviderFunc)( IRawElementProviderSimple * pProvider);

// Screen readers can use this to group or filter SurfaceReader's announcements
const wchar_t * const wstrAnnouncementActivityID = L"SurfaceReader.SurfaceUpdate";


// The smallest UIA provider that can raise notifications: it adds nothing of its own,
// and UIA takes every property from the window it is hosted in.
class UiaNotificationProvider : public IRawElementProviderSimple
{
public:
	UiaNotificationProvider( HWND hWindow, UiaHostProviderFromHwndFunc pHostFunc)
		: nRefCount( 1),
		hMyWindow( hWindow),
		pHostProviderFromHwnd( pHostFunc)
	{}

	void SetWindow( HWND hWindow)
	{
		hMyWindow = hWindow;
	}

	// IUnknown
	IFACEMETHODIMP_(ULONG) AddRef()
	{
		return InterlockedIncrement( &nRefCount);
	}

	IFACEMETHODIMP_(ULONG) Release()
	{
		LONG nCount = InterlockedDecrement( &nRefCount);
		if (nCount == 0)
		{
			delete this;
		}
		return nCount;
	}

	IFACEMETHODIMP QueryInterface( REFIID riid, void ** ppInterface)
	{
		if (ppInterface == NULL)
		{
			return E_POINTER;
		}

		if ((riid == __uuidof( IUnknown))
			|| (riid == __uuidof( IRawElementProviderSimple)))
		{
			*ppInterface = static_cast <IRawElementProviderSimple *> (this);
			AddRef();
			return S_OK;
		}

		*ppInterface = NULL;
		return E_NOINTERFACE;
	}

	// IRawElementProviderSimple
	IFACEMETHODIMP get_ProviderOptions( ProviderOptions * pOptions)
	{
		if (pOptions == NULL)
		{
			return E_INVALIDARG;
		}
		*pOptions = static_cast <ProviderOptions> (ProviderOptions_ServerSideProvider | ProviderOptions_UseComThreading);
		return S_OK;
	}

	IFACEMETHODIMP GetPatternProvider( PATTERNID patternId, IUnknown ** ppPattern)
	{
		if (ppPattern == NULL)
		{
			return E_INVALIDARG;
		}
		*ppPattern = NULL;
		return S_OK;
	}

	IFACEMETHODIMP GetPropertyValue( PROPERTYID propertyId, VARIANT * pValue)
	{
		if (pValue == NULL)
		{
			return E_INVALIDARG;
		}
		pValue->vt = VT_EMPTY;  // use the host window's value
		return S_OK;
	}

	IFACEMETHODIMP get_HostRawElementProvider( IRawElementProviderSimple ** ppProvider)
	{
		if (ppProvider == NULL)
		{
			return E_INVALIDARG;
		}
		*ppProvider = NULL;

		if ((hMyWindow == NULL)
			|| (pHostProviderFromHwnd == NULL))
		{
			return S_OK;
		}
		return pHostProviderFromHwnd( hMyWindow, ppProvider);
	}

private:
	~UiaNotificationProvider()
	{}

	LONG nRefCount;
	HWND hMyWindow;
	UiaHostProviderFromHwndFunc pHostProviderFromHwnd;
};


UiaAnnouncer::UiaAnnouncer()
	: hUiaCore( NULL),
	pRaiseNotificationEvent( NULL),
	pHostProviderFromHwnd( NULL),
	pClientsAreListening( NULL),
	pDisconnectProvider( NULL),
	pProvider( NULL)
{
	// Load only from System32, never from the application folder
	hUiaCore = LoadLibraryExW( L"UIAutomationCore.dll", NULL, LOAD_LIBRARY_SEARCH_SYSTEM32);

	if (hUiaCore != NULL)
	{
		pRaiseNotificationEvent = GetProcAddress( hUiaCore, "UiaRaiseNotificationEvent");
		pHostProviderFromHwnd = GetProcAddress( hUiaCore, "UiaHostProviderFromHwnd");
		pClientsAreListening = GetProcAddress( hUiaCore, "UiaClientsAreListening");
		pDisconnectProvider = GetProcAddress( hUiaCore, "UiaDisconnectProvider");
	}
}


UiaAnnouncer::~UiaAnnouncer()
{
	if (pProvider != NULL)
	{
		// Tell UIA to drop any references it still holds, then release ours
		if (pDisconnectProvider != NULL)
		{
			reinterpret_cast <UiaDisconnectProviderFunc> (pDisconnectProvider)( pProvider);
		}
		pProvider->Release();
		pProvider = NULL;
	}

	// UIAutomationCore.dll is deliberately not freed: UIA may still be using it on other
	// threads, and the system keeps it loaded for the life of the process anyway.
}


void UiaAnnouncer::SetWindow( HWND hWindow)
{
	if ((pRaiseNotificationEvent == NULL)
		|| (pHostProviderFromHwnd == NULL))
	{
		return;
	}

	if (pProvider == NULL)
	{
		pProvider = new UiaNotificationProvider( hWindow, reinterpret_cast <UiaHostProviderFromHwndFunc> (pHostProviderFromHwnd));
	}
	else
	{
		pProvider->SetWindow( hWindow);
	}
}


bool UiaAnnouncer::IsAvailable() const
{
	return ((pRaiseNotificationEvent != NULL)
		&& (pProvider != NULL));
}


HRESULT UiaAnnouncer::Announce( const std::wstring & wstrText, bool blnInterrupt)
{
	if ((IsAvailable() == false)
		|| (wstrText.empty()))
	{
		return S_FALSE;
	}

	// Nothing to do when no screen reader (or other UIA client) is running
	if ((pClientsAreListening != NULL)
		&& (reinterpret_cast <UiaClientsAreListeningFunc> (pClientsAreListening)() == FALSE))
	{
		return S_FALSE;
	}

	CComBSTR bstrText( wstrText.c_str());
	CComBSTR bstrActivityID( wstrAnnouncementActivityID);

	if ((bstrText == NULL)
		|| (bstrActivityID == NULL))
	{
		return E_OUTOFMEMORY;
	}

	// ImportantMostRecent replaces anything of ours still queued, so a fast-moving
	// control can't build up a backlog; ImportantAll queues every message.
	NotificationProcessing myProcessing = blnInterrupt ? NotificationProcessing_ImportantMostRecent : NotificationProcessing_ImportantAll;

	return reinterpret_cast <UiaRaiseNotificationEventFunc> (pRaiseNotificationEvent)( pProvider, NotificationKind_Other, myProcessing, bstrText, bstrActivityID);
}

#endif  // _WIN32
