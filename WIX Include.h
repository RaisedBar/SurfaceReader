#ifndef WIXINCLUDE_H
#define WIXINCLUDE_H
#pragma once

// Error-handling macros in the style of the WiX toolset's dutil library.
// SurfaceReader used to take these from the WiX 3.11 SDK (dutil.h and dutil.lib), which is not
// available from vcpkg; they are now defined here, with the few dutil helpers the code used.
// Each macro sets the result if needed, traces the message and jumps to the function's LExit label.

#ifdef _WIN32
#include <windows.h>

// Trace a failure to the debugger output in debug builds; does nothing in release builds.
inline void RBExitTrace( HRESULT hr, const char * szMessage)
{
#ifdef _DEBUG
	char szBuffer[ 512];
	_snprintf_s( szBuffer, sizeof( szBuffer), _TRUNCATE, "SurfaceReader: 0x%08lX %s\n", (unsigned long) hr, szMessage);
	OutputDebugStringA( szBuffer);
#else
	(void) hr;
	(void) szMessage;
#endif
}

// Replacement for dutil's ProcWow64: is the process a 32-bit process running on 64-bit Windows?
inline HRESULT ProcWow64( HANDLE hProcess, BOOL * pfWow64)
{
	*pfWow64 = FALSE;
	if (IsWow64Process( hProcess, pfWow64) == FALSE)
	{
		return HRESULT_FROM_WIN32( GetLastError());
	}
	return S_OK;
}
#else
inline void RBExitTrace( HRESULT, const char *)
{}
#endif

#define ExitTrace(x, s)   RBExitTrace( x, s)

#define ExitOnFailure(x, s)   if (FAILED(x)) { ExitTrace(x, s);  goto LExit; }
#define ExitOnRootFailure(x, s)   ExitOnFailure(x, s)
#define ExitOnNull(p, x, e, s)   if (NULL == p) { x = e; ExitTrace(x, s);  goto LExit; }
#define ExitOnNotNull(p, x, e, s)   if (NULL != p) { x = e; ExitTrace(x, s);  goto LExit; }

// Exit on false and exit if values are equal or not equal to a given value.
#define ExitOnFalse(v, x, e, s)   if (v ==false) { x = e; ExitTrace(x, s);  goto LExit; }
#define ExitOnTrue(v, x, e, s)   if (v ==true) { x = e; ExitTrace(x, s);  goto LExit; }
#define ExitOnSpecificValue(p, v, x, e, s)   if (v == p) { x = e; ExitTrace(x, s);  goto LExit; }
#define ExitIfValueGreaterThanSuppliedValue(p, v, x, e, s)   if (v > p) { x = e; ExitTrace(x, s);  goto LExit; }
#define ExitIfValueLessThanOrEqualToSuppliedValue(p, v, x, e, s)   if (v <= p) { x = e; ExitTrace(x, s);  goto LExit; }
#define ExitIfValueLessThanSuppliedValue(p, v, x, e, s)   if (v < p) { x = e; ExitTrace(x, s);  goto LExit; }
#define ExitIfValueNotEqualToSuppliedValue(p, v, x, e, s)   if (v != p) { x = e; ExitTrace(x, s);  goto LExit; }
#define ExitFunctionWithMessage(x, s)   { ExitTrace(x, s);  goto LExit; }
#define ExitFunctionWithMessageAndStatusCode(x, e, s)  { x = e; ExitTrace(x, s);  goto LExit; }
#endif
