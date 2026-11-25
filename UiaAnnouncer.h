// UiaAnnouncer.h
// Speaks text through the user's screen reader by raising UI Automation notification events.
// Raised Bar Ltd.
// http://www.raisedbar.net

#ifndef UIA_ANNOUNCER_H
#define UIA_ANNOUNCER_H

#pragma once

#ifdef _WIN32

#include <windows.h>
#include <string>

class UiaNotificationProvider;

/* UI Automation notification events (Windows 10 version 1709 and later) are read out by
Narrator, NVDA, JAWS and other UIA-aware screen readers, in the user's own voice and settings.
Screen readers may ignore notifications from an application that is not in the foreground,
so RBSpeech uses this for screen readers it has no direct API for, and as a fallback.
On older Windows the UIA functions don't exist and IsAvailable() returns false. */

class UiaAnnouncer
{
public:
	UiaAnnouncer();
	~UiaAnnouncer();

	/// <summary> The window the notifications are raised from (the application's main frame). </summary>
	void SetWindow( HWND hWindow);

	/// <summary> True when this version of Windows supports UIA notifications and a window has been set. </summary>
	bool IsAvailable() const;

	/// <summary> Ask the screen reader to speak the text. </summary>
	/// <param name="blnInterrupt">True replaces anything from SurfaceReader that is still waiting to be spoken; false queues the text.</param>
	/// <returns> S_OK if the notification was raised, S_FALSE if no UIA client is listening or UIA notifications are unavailable, or an error code. </returns>
	HRESULT Announce( const std::wstring & wstrText, bool blnInterrupt);

private:
	UiaAnnouncer( const UiaAnnouncer &) = delete;
	UiaAnnouncer & operator=( const UiaAnnouncer &) = delete;

	HMODULE hUiaCore;
	FARPROC pRaiseNotificationEvent;
	FARPROC pHostProviderFromHwnd;
	FARPROC pClientsAreListening;
	FARPROC pDisconnectProvider;
	UiaNotificationProvider * pProvider;  // COM object; this class holds one reference
};

#endif  // _WIN32
#endif  // UIA_ANNOUNCER_H
