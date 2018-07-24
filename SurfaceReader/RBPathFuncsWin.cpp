//RBPathFuncs.cpp: path functions source file.
//SurfaceReader includes.
#include "SRConstants.h"
#include "RBPathFuncs.h"
#include "RBPathErrorException.h"

//standard c++ includes.
#include <string>

//Windows includes.
#define WIN32_LEAN_AND_MEAN 
#include <windows.h>
#include <shellapi.h>
#include <ShlObj.h>
#include <comdef.h>

//WiX includes.
#include <dutil.h>
#include <shelutil.h>
namespace RaisedBar
{
	namespace PathFunctions
	{
		using namespace std::experimental::filesystem;
		using std::wstring;

		//Create an anonymous namespace.
		namespace {
			path GetExecutablePath()
			{
				wchar_t buf[MAX_PATH];

				const int bytes = GetModuleFileName(nullptr, buf, _MAX_PATH);

				if (bytes == 0)
				{
					throw new RBPathErrorException(L"test");
				}

				return path(buf, buf + bytes);
			}
		}// End anonymous namespace.

		path AppDataPath()
		{
			HRESULT hr = S_OK;
			LPWSTR wixAppDataFolderPath = nullptr;
			hr = ShelGetFolder(&wixAppDataFolderPath, CSIDL_COMMON_APPDATA);
			if (hr != S_OK)
			{
				//We have a failure, so convert the returned HRESULT in to an error message and throw an RBPathErrorException.
				_com_error err(hr);
				wstring errorMessage = err.ErrorMessage();
				throw RBPathErrorException(errorMessage);
			}
			
			path appDataPath = wixAppDataFolderPath;
			//Add the company name.
			appDataPath /= wstrRBCompanyName;
			//Add the app title.
			appDataPath /= wstrAppTitle;
			return appDataPath;
		}

		path HelpPath()
		{
			try
			{
				//Obtain the executable path.
				path executablePath = GetExecutablePath();
				return executablePath.remove_filename();
			}
			catch (const RBPathErrorException& e)
			{
				throw e;
			}
		}

		path AppConfigPath()
		{
			try
			{
				//Obtain the AppData path.
				path appDataPath = AppDataPath();
return 				appDataPath /= wstrAppConfigDir;
			}
			catch (const RBPathErrorException& e)
			{
				throw e;
			}
		}

		path SpeechPath()
		{
			try
			{
				//Obtain the executable path.
				path executablePath = GetExecutablePath();
				return executablePath.remove_filename();
			}
			catch (const RBPathErrorException& e)
			{
				throw e;
			}
		}

		path ProtocolPath()
		{
			try
			{
				//Obtain the AppData path.
				path appDataPath = AppDataPath();
				return 				appDataPath /= wstrProtocolDir;
			}
			catch (const RBPathErrorException& e)
			{
				throw e;
			}
		}

		path SurfacePath()
		{
			try 
			{				
				//Obtain the AppData path.				
				path appDataPath = AppDataPath();				
				return 				appDataPath /= wstrSurfaceDir;
			}			
			catch (const RBPathErrorException& e)			
			{				
				throw e;			
			}
		}

		path LogPath()
		{
			try
			{
				//Obtain the AppData path.				
				path appDataPath = AppDataPath();
				return 				appDataPath /= wstrLogDir;
			}
			catch (const RBPathErrorException& e)
			{
				throw e;
			}
		}

		void EnsurePathExists(path& path)
		{
			if (!exists(path))
			{
				create_directory(path);
			}
		}
	}
}