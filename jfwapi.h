#ifndef JFWAPI_H_INCLUDED
#define JFWAPI_H_INCLUDED
#ifndef JFWAPI
#define JFWAPI __declspec(dllimport)
#endif
#ifdef __cplusplus
extern "C"
{				/* Assume C declarations for C++ */
#endif				/* __cplusplus */
  JFWAPI BOOL WINAPI JFWRunScript (LPCTSTR lpszScriptName);
  JFWAPI BOOL WINAPI JFWSayString (LPCTSTR lpszStrinToSpeak, BOOL bInterrupt);
  JFWAPI BOOL WINAPI JFWStopSpeech ();

#ifdef __cplusplus
}				/* End of extern "C" { */
#endif				/* __cplusplus */

#endif				/* ifndefJFWAPI_H_INCLUDED */
