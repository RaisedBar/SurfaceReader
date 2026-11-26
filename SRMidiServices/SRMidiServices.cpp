// SRMidiServices.cpp
// Command-line helper that manages Windows MIDI Services loopback ports for SurfaceReader.
// Raised Bar Ltd.
// http://www.raisedbar.net
//
// SurfaceReader's Windows build is 32-bit, and the Windows MIDI Services API (Windows.Devices.Midi2)
// is only available for 64-bit processes, so SurfaceReader runs this helper to create and remove its
// loopback ports. The loopbacks belong to the MIDI service, not to this process, so they stay after
// the helper exits. SurfaceReader then uses them through the ordinary MIDI 1.0 (WinMM) ports that the
// service creates for each loopback endpoint, exactly as it used loopMIDI ports.
//
// Usage (all output is UTF-8, one record per line, fields separated by tabs):
//   SRMidiServices detect
//       available <0|1> <reason>      reason: ok, os-too-old, api-missing, service-unavailable, loopback-unavailable
//       systemapi <0|1>               1 if Windows' own copy of the API is in use, 0 if this app's copy
//   SRMidiServices list
//       loopback <association id> <saved 0|1> <endpoint A name> <endpoint B name>   (one line per loopback)
//   SRMidiServices create <unique id> <endpoint A name> <endpoint B name> [--save]
//       created <association id> <saved 0|1>
//   SRMidiServices remove <association id>
//       removed <association id>
//   Errors: error <message>
//
// Exit codes: 0 success, 1 the operation failed, 2 bad arguments, 3 Windows MIDI Services isn't available.

#include <windows.h>
#include <combaseapi.h>   // StringFromGUID2, CLSIDFromString (left out by WIN32_LEAN_AND_MEAN)
#include <winrt/Windows.Foundation.h>
#include <winrt/Windows.Foundation.Collections.h>
#include <winrt/Windows.Devices.Midi2.h>
#include <winrt/Windows.Devices.Midi2.ServiceConfig.h>
#include <winrt/Windows.Devices.Midi2.Transports.Loopback.h>

#include <cstdio>
#include <set>
#include <string>
#include <vector>

using namespace winrt::Windows::Devices::Midi2;
using namespace winrt::Windows::Devices::Midi2::ServiceConfig;
using namespace winrt::Windows::Devices::Midi2::Transports::Loopback;

namespace
{
	const int EXIT_OK = 0;
	const int EXIT_FAILED = 1;
	const int EXIT_USAGE = 2;
	const int EXIT_UNAVAILABLE = 3;

	// Windows MIDI Services needs Windows 11 25H2 (build 26200) or later
	const DWORD MIN_WINDOWS_BUILD = 26200;

	std::string ToUtf8( const std::wstring & wstrText)
	{
		if (wstrText.empty())
		{
			return std::string();
		}
		int nLength = WideCharToMultiByte( CP_UTF8, 0, wstrText.data(), (int) wstrText.size(), nullptr, 0, nullptr, nullptr);
		std::string strOut( nLength, '\0');
		WideCharToMultiByte( CP_UTF8, 0, wstrText.data(), (int) wstrText.size(), &strOut[ 0], nLength, nullptr, nullptr);
		return strOut;
	}

	// Tabs and line breaks would break the record format; names never need them
	std::wstring Clean( std::wstring wstrText)
	{
		for (wchar_t & c : wstrText)
		{
			if ((c == L'\t') || (c == L'\r') || (c == L'\n'))
			{
				c = L' ';
			}
		}
		return wstrText;
	}

	void WriteRecord( const std::vector <std::wstring> & vFields)
	{
		std::wstring wstrLine;
		for (size_t i = 0; i < vFields.size(); i++)
		{
			if (i > 0)
			{
				wstrLine += L'\t';
			}
			wstrLine += Clean( vFields[ i]);
		}
		std::string strLine = ToUtf8( wstrLine) + "\n";
		std::fwrite( strLine.data(), 1, strLine.size(), stdout);
		std::fflush( stdout);
	}

	std::wstring GuidToString( const winrt::guid & myGuid)
	{
		wchar_t szGuid[ 64] = L"";
		StringFromGUID2( reinterpret_cast <const GUID &> (myGuid), szGuid, 64);
		return szGuid;
	}

	bool StringToGuid( const std::wstring & wstrGuid, winrt::guid & myGuid)
	{
		GUID myRawGuid {};
		if (FAILED( CLSIDFromString( wstrGuid.c_str(), &myRawGuid)))
		{
			return false;
		}
		myGuid = reinterpret_cast <const winrt::guid &> (myRawGuid);
		return true;
	}

	DWORD WindowsBuild()
	{
		// GetVersionEx lies to unmanifested apps; RtlGetVersion doesn't
		typedef LONG (WINAPI * RtlGetVersionFunc)( PRTL_OSVERSIONINFOW);
		HMODULE hNtDll = GetModuleHandleW( L"ntdll.dll");
		RtlGetVersionFunc pRtlGetVersion = hNtDll ? reinterpret_cast <RtlGetVersionFunc> (GetProcAddress( hNtDll, "RtlGetVersion")) : nullptr;
		RTL_OSVERSIONINFOW myVersion { sizeof( myVersion) };
		if ((pRtlGetVersion == nullptr) || (pRtlGetVersion( &myVersion) != 0))
		{
			return 0;
		}
		return myVersion.dwBuildNumber;
	}

	// The three startup checks Microsoft recommends: the Windows version, whether the API can be
	// loaded at all, and whether the service (and here, its loopback transport) is usable.
	int Detect( bool blnReport)
	{
		if (WindowsBuild() < MIN_WINDOWS_BUILD)
		{
			if (blnReport) WriteRecord( { L"available", L"0", L"os-too-old" });
			return EXIT_UNAVAILABLE;
		}

		bool blnSystemApi = false;
		try
		{
			blnSystemApi = MidiApi::IsSystemProvided();
		}
		catch ( ...)
		{
			// The API's DLL couldn't be found or loaded
			if (blnReport) WriteRecord( { L"available", L"0", L"api-missing" });
			return EXIT_UNAVAILABLE;
		}

		try
		{
			if (MidiApi::EnsureServiceAvailable() == false)
			{
				if (blnReport) WriteRecord( { L"available", L"0", L"service-unavailable" });
				return EXIT_UNAVAILABLE;
			}
			if (MidiLoopbackManager::IsTransportAvailable() == false)
			{
				if (blnReport) WriteRecord( { L"available", L"0", L"loopback-unavailable" });
				return EXIT_UNAVAILABLE;
			}
		}
		catch ( ...)
		{
			if (blnReport) WriteRecord( { L"available", L"0", L"service-unavailable" });
			return EXIT_UNAVAILABLE;
		}

		if (blnReport)
		{
			WriteRecord( { L"available", L"1", L"ok" });
			WriteRecord( { L"systemapi", blnSystemApi ? L"1" : L"0" });
		}
		return EXIT_OK;
	}

	int List()
	{
		std::set <std::wstring> mySaved;
		for (auto const & mySavedEntry : MidiLoopbackManager::GetSavedLoopbackEntries())
		{
			mySaved.insert( GuidToString( mySavedEntry.AssociationId()));
		}

		for (auto const & myEntry : MidiLoopbackManager::GetActiveLoopbackEntries())
		{
			std::wstring wstrID = GuidToString( myEntry.AssociationId());
			WriteRecord( { L"loopback", wstrID, mySaved.count( wstrID) ? L"1" : L"0",
				std::wstring( myEntry.EndpointA().Name()), std::wstring( myEntry.EndpointB().Name()) });
		}
		return EXIT_OK;
	}

	int Create( const std::wstring & wstrUniqueID, const std::wstring & wstrNameA, const std::wstring & wstrNameB, bool blnSave)
	{
		MidiLoopbackEndpointDefinition myDefinitionA;
		myDefinitionA.Name( wstrNameA);
		myDefinitionA.Description( L"SurfaceReader's end of this loopback");
		myDefinitionA.UniqueId( wstrUniqueID);

		MidiLoopbackEndpointDefinition myDefinitionB;
		myDefinitionB.Name( wstrNameB);
		myDefinitionB.Description( L"Created by SurfaceReader for use by your DAW");
		myDefinitionB.UniqueId( wstrUniqueID);

		MidiLoopbackCreationConfig myConfig( myDefinitionA, myDefinitionB);
		auto myResponse = MidiLoopbackManager::CreateTransientLoopback( myConfig);

		if (myResponse.Success() == false)
		{
			WriteRecord( { L"error", std::wstring( myResponse.ErrorMessage()) });
			return EXIT_FAILED;
		}

		// Keep it after a restart of the MIDI service or Windows
		bool blnSaved = false;
		if (blnSave)
		{
			// A PC that has never saved any MIDI settings has no configuration file yet
			try
			{
				auto myEnsureResponse = MidiServiceTransportPluginConfigManager::EnsureConfigurationFile();
				if (myEnsureResponse.Success() == false)
				{
					WriteRecord( { L"warning", L"no configuration file: " + std::wstring( myEnsureResponse.ErrorMessage()) });
				}
			}
			catch ( winrt::hresult_error const & myError)
			{
				WriteRecord( { L"warning", L"no configuration file: " + std::wstring( myError.message()) });
			}

			auto mySaveResponse = MidiServiceTransportPluginConfigManager::SaveUpdate( myConfig);
			blnSaved = mySaveResponse.Success();
			if (blnSaved == false)
			{
				WriteRecord( { L"warning", L"not saved: " + std::wstring( mySaveResponse.ErrorMessage()) });
			}
		}

		WriteRecord( { L"created", GuidToString( myResponse.CreatedLoopbackEntry().AssociationId()), blnSaved ? L"1" : L"0" });
		return EXIT_OK;
	}

	int Remove( const std::wstring & wstrAssociationID)
	{
		winrt::guid myAssociationID;
		if (StringToGuid( wstrAssociationID, myAssociationID) == false)
		{
			WriteRecord( { L"error", L"not an association id: " + wstrAssociationID });
			return EXIT_USAGE;
		}

		MidiLoopbackRemovalConfig myConfig( myAssociationID);
		auto myResponse = MidiLoopbackManager::RemoveTransientLoopback( myConfig);

		// Also drop it from the configuration file, if it was saved there
		MidiServiceTransportPluginConfigManager::SaveUpdate( myConfig);

		if (myResponse.Success() == false)
		{
			WriteRecord( { L"error", std::wstring( myResponse.ErrorMessage()) });
			return EXIT_FAILED;
		}

		WriteRecord( { L"removed", GuidToString( myAssociationID) });
		return EXIT_OK;
	}
}


int wmain( int argc, wchar_t ** argv)
{
	if (argc < 2)
	{
		WriteRecord( { L"error", L"usage: SRMidiServices detect | list | create <unique id> <name A> <name B> [--save] | remove <association id>" });
		return EXIT_USAGE;
	}

	std::wstring wstrCommand = argv[ 1];

	try
	{
		winrt::init_apartment();

		if (wstrCommand == L"detect")
		{
			return Detect( true);
		}

		// Every other command needs the service
		int nAvailable = Detect( false);
		if (nAvailable != EXIT_OK)
		{
			WriteRecord( { L"error", L"Windows MIDI Services isn't available" });
			return nAvailable;
		}

		if (wstrCommand == L"list")
		{
			return List();
		}
		if ((wstrCommand == L"create") && ((argc == 5) || ((argc == 6) && (std::wstring( argv[ 5]) == L"--save"))))
		{
			return Create( argv[ 2], argv[ 3], argv[ 4], argc == 6);
		}
		if ((wstrCommand == L"remove") && (argc == 3))
		{
			return Remove( argv[ 2]);
		}
	}
	catch ( winrt::hresult_error const & myError)
	{
		WriteRecord( { L"error", std::wstring( myError.message()) });
		return EXIT_FAILED;
	}
	catch ( ...)
	{
		WriteRecord( { L"error", L"unexpected failure" });
		return EXIT_FAILED;
	}

	WriteRecord( { L"error", L"unknown command or wrong arguments: " + wstrCommand });
	return EXIT_USAGE;
}
