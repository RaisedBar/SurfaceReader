// MidiServices.cpp
// Windows MIDI Services loopback ports, created through the SRMidiServices helper.
// Raised Bar Ltd.
// http://www.raisedbar.net

#include "MidiServices.h"

#ifdef _WIN32

#include <windows.h>

#include <mutex>
#include <sstream>

namespace MidiServices
{
	const char * const szSurfaceReaderSuffix = " (SR)";

	namespace
	{
		const DWORD HELPER_TIMEOUT_MS = 15000;
		const DWORD EXIT_OK = 0;

		std::wstring Utf8ToWide( const std::string & strText)
		{
			if (strText.empty())
			{
				return std::wstring();
			}
			int nLength = MultiByteToWideChar( CP_UTF8, 0, strText.data(), (int) strText.size(), nullptr, 0);
			std::wstring wstrOut( nLength, L'\0');
			MultiByteToWideChar( CP_UTF8, 0, strText.data(), (int) strText.size(), &wstrOut[ 0], nLength);
			return wstrOut;
		}

		// <folder of SurfaceReader.exe>\MidiServices\SRMidiServices.exe
		std::wstring HelperPath()
		{
			wchar_t szModule[ MAX_PATH + 1] = L"";
			DWORD nLength = GetModuleFileNameW( nullptr, szModule, MAX_PATH + 1);
			if ((nLength == 0) || (nLength > MAX_PATH))
			{
				return std::wstring();
			}
			std::wstring wstrPath( szModule, nLength);
			wstrPath.erase( wstrPath.find_last_of( L"\\/") + 1);
			return wstrPath + L"MidiServices\\SRMidiServices.exe";
		}

		// Quote one argument for CommandLineToArgvW, which the helper's wmain uses
		std::wstring QuoteArgument( const std::wstring & wstrArgument)
		{
			std::wstring wstrOut = L"\"";
			size_t nBackslashes = 0;
			for (wchar_t c : wstrArgument)
			{
				if (c == L'\\')
				{
					nBackslashes++;
					continue;
				}
				if (c == L'"')
				{
					wstrOut.append( nBackslashes * 2 + 1, L'\\');
				}
				else
				{
					wstrOut.append( nBackslashes, L'\\');
				}
				nBackslashes = 0;
				wstrOut += c;
			}
			wstrOut.append( nBackslashes * 2, L'\\');
			wstrOut += L'"';
			return wstrOut;
		}

		// Run the helper without a window and collect its output, one record (tab-separated fields) per line
		bool RunHelper( const std::vector <std::wstring> & vArguments, std::vector <std::vector <std::string> > & vRecords, DWORD & nExitCode)
		{
			vRecords.clear();
			nExitCode = (DWORD) -1;

			std::wstring wstrHelper = HelperPath();
			if (wstrHelper.empty() || (GetFileAttributesW( wstrHelper.c_str()) == INVALID_FILE_ATTRIBUTES))
			{
				return false;
			}

			std::wstring wstrCommandLine = QuoteArgument( wstrHelper);
			for (const std::wstring & wstrArgument : vArguments)
			{
				wstrCommandLine += L' ';
				wstrCommandLine += QuoteArgument( wstrArgument);
			}

			SECURITY_ATTRIBUTES mySecurity { sizeof( mySecurity), nullptr, TRUE };
			HANDLE hRead = nullptr, hWrite = nullptr;
			if (CreatePipe( &hRead, &hWrite, &mySecurity, 0) == FALSE)
			{
				return false;
			}
			SetHandleInformation( hRead, HANDLE_FLAG_INHERIT, 0);

			STARTUPINFOW myStartup { sizeof( myStartup) };
			myStartup.dwFlags = STARTF_USESTDHANDLES;
			myStartup.hStdOutput = hWrite;
			myStartup.hStdError = hWrite;
			myStartup.hStdInput = nullptr;

			PROCESS_INFORMATION myProcess {};
			std::vector <wchar_t> vCommandLine( wstrCommandLine.begin(), wstrCommandLine.end());
			vCommandLine.push_back( L'\0');

			BOOL blnStarted = CreateProcessW( wstrHelper.c_str(), vCommandLine.data(), nullptr, nullptr, TRUE,
				CREATE_NO_WINDOW, nullptr, nullptr, &myStartup, &myProcess);
			CloseHandle( hWrite);  // so ReadFile ends when the helper exits

			if (blnStarted == FALSE)
			{
				CloseHandle( hRead);
				return false;
			}

			// Drain the output while waiting, with a deadline, so a helper that hangs can't hang SurfaceReader
			std::string strOutput;
			char szBuffer[ 4096];
			ULONGLONG nDeadline = GetTickCount64() + HELPER_TIMEOUT_MS;
			bool blnExited = false;

			while (true)
			{
				DWORD nAvailable = 0;
				while (PeekNamedPipe( hRead, nullptr, 0, nullptr, &nAvailable, nullptr) && (nAvailable > 0))
				{
					DWORD nRead = 0;
					if ((ReadFile( hRead, szBuffer, (DWORD) sizeof( szBuffer), &nRead, nullptr) == FALSE) || (nRead == 0))
					{
						break;
					}
					strOutput.append( szBuffer, nRead);
				}

				if (blnExited)
				{
					break;  // everything it wrote has been read
				}
				if (WaitForSingleObject( myProcess.hProcess, 20) == WAIT_OBJECT_0)
				{
					blnExited = true;  // one more pass to read what's left
					continue;
				}
				if (GetTickCount64() > nDeadline)
				{
					TerminateProcess( myProcess.hProcess, 1);
					WaitForSingleObject( myProcess.hProcess, 1000);
					break;
				}
			}
			CloseHandle( hRead);
			GetExitCodeProcess( myProcess.hProcess, &nExitCode);
			CloseHandle( myProcess.hProcess);
			CloseHandle( myProcess.hThread);

			std::istringstream myLines( strOutput);
			std::string strLine;
			while (std::getline( myLines, strLine))
			{
				if ((strLine.empty() == false) && (strLine.back() == '\r'))
				{
					strLine.pop_back();
				}
				std::vector <std::string> vFields;
				size_t nStart = 0, nTab;
				while ((nTab = strLine.find( '\t', nStart)) != std::string::npos)
				{
					vFields.push_back( strLine.substr( nStart, nTab - nStart));
					nStart = nTab + 1;
				}
				vFields.push_back( strLine.substr( nStart));
				vRecords.push_back( vFields);
			}
			return true;
		}

		std::string FirstError( const std::vector <std::vector <std::string> > & vRecords)
		{
			for (const auto & vRecord : vRecords)
			{
				if ((vRecord.size() >= 2) && (vRecord[ 0] == "error"))
				{
					return vRecord[ 1];
				}
			}
			return "the SRMidiServices helper failed";
		}

		// A stable identifier for a loopback, from the DAW port name: letters and digits only
		std::wstring UniqueIDFor( const std::string & strDawPortName)
		{
			unsigned long long nHash = 1469598103934665603ULL;  // FNV-1a
			for (unsigned char c : strDawPortName)
			{
				nHash ^= c;
				nHash *= 1099511628211ULL;
			}
			wchar_t szID[ 40];
			swprintf_s( szID, L"SurfaceReader%016llX", nHash);
			return szID;
		}
	}


	bool IsAvailable()
	{
		static std::once_flag myOnce;
		static bool blnAvailable = false;

		std::call_once( myOnce, []
		{
			std::vector <std::vector <std::string> > vRecords;
			DWORD nExitCode = 0;
			if (RunHelper( { L"detect" }, vRecords, nExitCode) && (nExitCode == EXIT_OK))
			{
				for (const auto & vRecord : vRecords)
				{
					if ((vRecord.size() >= 2) && (vRecord[ 0] == "available") && (vRecord[ 1] == "1"))
					{
						blnAvailable = true;
					}
				}
			}
		});
		return blnAvailable;
	}


	std::string FitPortName( const std::string & strName, size_t nMaxLength)
	{
		std::wstring wstrName = Utf8ToWide( strName);
		if (wstrName.size() <= nMaxLength)
		{
			return strName;
		}
		size_t nLength = nMaxLength;
		if ((nLength > 0) && IS_HIGH_SURROGATE( wstrName[ nLength - 1]))
		{
			nLength--;  // don't split a surrogate pair
		}
		wstrName.resize( nLength);
		while ((wstrName.empty() == false) && (wstrName.back() == L' '))
		{
			wstrName.pop_back();
		}
		int nBytes = WideCharToMultiByte( CP_UTF8, 0, wstrName.data(), (int) wstrName.size(), nullptr, 0, nullptr, nullptr);
		std::string strOut( nBytes, '\0');
		WideCharToMultiByte( CP_UTF8, 0, wstrName.data(), (int) wstrName.size(), &strOut[ 0], nBytes, nullptr, nullptr);
		return strOut;
	}


	std::string SurfaceReaderPortName( const std::string & strDawPortName)
	{
		// The marker goes on after shortening, so the two ends never look alike
		return FitPortName( strDawPortName, nMaxPortName - (sizeof( " (SR)") - 1)) + szSurfaceReaderSuffix;
	}


	bool ListLoopbacks( std::vector <Loopback> & vLoopbacks)
	{
		vLoopbacks.clear();
		std::vector <std::vector <std::string> > vRecords;
		DWORD nExitCode = 0;
		if ((RunHelper( { L"list" }, vRecords, nExitCode) == false) || (nExitCode != EXIT_OK))
		{
			return false;
		}

		for (const auto & vRecord : vRecords)
		{
			if ((vRecord.size() >= 5) && (vRecord[ 0] == "loopback"))
			{
				Loopback myLoopback;
				myLoopback.strAssociationID = vRecord[ 1];
				myLoopback.blnSaved = (vRecord[ 2] == "1");
				myLoopback.strNameA = vRecord[ 3];
				myLoopback.strNameB = vRecord[ 4];
				vLoopbacks.push_back( myLoopback);
			}
		}
		return true;
	}


	bool CreateLoopback( const std::string & strDawPortName, std::string & strError)
	{
		std::vector <std::vector <std::string> > vRecords;
		DWORD nExitCode = 0;
		if (RunHelper( { L"create", UniqueIDFor( strDawPortName), Utf8ToWide( SurfaceReaderPortName( strDawPortName)), Utf8ToWide( FitPortName( strDawPortName)), L"--save" },
			vRecords, nExitCode) == false)
		{
			strError = "the SRMidiServices helper couldn't be run";
			return false;
		}
		if (nExitCode != EXIT_OK)
		{
			strError = FirstError( vRecords);
			return false;
		}
		return true;
	}


	bool RemoveLoopback( const std::string & strAssociationID, std::string & strError)
	{
		std::vector <std::vector <std::string> > vRecords;
		DWORD nExitCode = 0;
		if (RunHelper( { L"remove", Utf8ToWide( strAssociationID) }, vRecords, nExitCode) == false)
		{
			strError = "the SRMidiServices helper couldn't be run";
			return false;
		}
		if (nExitCode != EXIT_OK)
		{
			strError = FirstError( vRecords);
			return false;
		}
		return true;
	}
}

#else  // not Windows: CoreMIDI provides virtual ports directly

namespace MidiServices
{
	const char * const szSurfaceReaderSuffix = " (SR)";
	bool IsAvailable() { return false; }
	std::string FitPortName( const std::string & strName, size_t) { return strName; }
	std::string SurfaceReaderPortName( const std::string & strDawPortName) { return strDawPortName + szSurfaceReaderSuffix; }
	bool ListLoopbacks( std::vector <Loopback> & vLoopbacks) { vLoopbacks.clear(); return false; }
	bool CreateLoopback( const std::string &, std::string & strError) { strError = "not supported"; return false; }
	bool RemoveLoopback( const std::string &, std::string & strError) { strError = "not supported"; return false; }
}

#endif  // _WIN32
