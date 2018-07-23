
#ifdef DOLAPIDLL		// if we are compiling dolapi.dll
#define DECL_DOLAPIDLL _declspec(dllexport)
#else				// else this file is included by a client
#define DECL_DOLAPIDLL _declspec(dllimport)
#endif

// return codes from DolAccess_GetSystem
#define DOLACCESS_NONE 0
#define DOLACCESS_HAL 1
#define DOLACCESS_LUNAR 2
#define DOLACCESS_SUPERNOVA 4
#define DOLACCESS_LUNARPLUS 8

//return codes from functions other than DolAccess_GetSystem
#define DOLACCESS_SUCCESS						0
#define DOLACCESS_FAIL_NO_SERVER				1
#define DOLACCESS_FAIL_INVALID_ID				2
#define DOLACCESS_FAIL_BAD_LEN					3
#define DOLACCESS_FAIL_INVALID_ACTION			4
#define DOLACCESS_FAIL_NOT_SUPPORTED_BY_SERVER	5
#define DOLACCESS_FAIL_BAD_STRING				6
#define DOLACCESS_FAIL_TIMED_OUT				7
#define DOLACCESS_FAIL_READ_ONLY				8
#define DOLACCESS_FAIL_SERVER_BUSY				9

#ifdef __cplusplus
extern "C" {
#endif

DECL_DOLAPIDLL DWORD WINAPI DolAccess_GetSystem();

DECL_DOLAPIDLL DWORD WINAPI DolAccess_GetVal(
	VOID *Val,	//pointer to memory where the data will be stored by the function
	DWORD Len,	//length of Val in bytes
	DWORD Id	//identification number of the parameter
	);

DECL_DOLAPIDLL DWORD WINAPI DolAccess_SetVal(
	VOID *Val,	//pointer to the value
	DWORD Len,	//length of Val in bytes
	DWORD Id	//identification number of the parameter
	);

DECL_DOLAPIDLL DWORD WINAPI DolAccess_Action(
	DWORD Id	//identification number of the action
	);

DECL_DOLAPIDLL DWORD WINAPI DolAccess_Command(
	VOID *Par,	//pointer to the parameter(s) needed by the command 
	DWORD Len,	//length of Parameter in bytes
	DWORD Id	//identification number of the command 
	);


#ifdef __cplusplus
}
#endif


// custom id's for DolAccess_SetVal 
#define MAG_VP_PRIM 0x80000000
#define MAG_VP_SEC 0x80000001


// id's for DolAccess_Command 
#define DOLAPI_COMMAND_SPEAK 1
#define DOLAPI_COMMAND_LOAD_SCRIPT 2
#define DOLAPI_COMMAND_UNLOAD_SCRIPT 3
#define DOLAPI_COMMAND_SCRIPT_FUNCTION 4

