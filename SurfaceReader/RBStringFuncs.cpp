// RBStringFuncs.cpp

#pragma warning(disable:4996)
#include "RBStringFuncs.h"


// Implementations

bool IsAllHexChars(std::string strIn)
{
	std::string strTemp;

	strTemp = StripSpaces(strIn);

	if (strTemp.empty())
	{
		return false;
	}

	boost::algorithm::to_upper(strTemp);
    return boost::all(strTemp, boost::is_any_of("1234567890ABCDEF"));
}


boost::filesystem::path AppDataPath()
{
#ifdef __WINDOWS__
	boost::filesystem::path myPath = wxStandardPaths::Get().GetConfigDir().ToStdWstring();
#else
	boost::filesystem::path myPath =wxStandardPaths::Get().GetExecutablePath().ToStdWstring();
#endif
		
		if (myPath.has_filename())
	{
		myPath.remove_filename();
	}

#ifdef __WINDOWS__
	// Add the company name
myPath /=wstrRBCompanyName;
	
	// check for directory existence
if (!boost::filesystem::exists(myPath))
	{
		// Create the required directory
		try
		{
		boost::filesystem::create_directory(myPath);
		}
		catch(const boost::filesystem::filesystem_error& err)
		{
				wxMessageBox( err.code().message(), wstrErrorTitle, wxOK | wxICON_EXCLAMATION);
			}
		}
	
	myPath /=wstrAppTitle;

	if (!boost::filesystem::exists(myPath))
	{
		// Create the required directory
		try 
			{
				boost::filesystem::create_directory(myPath);
			}
		catch(const boost::filesystem::filesystem_error& err)
		{
			wxMessageBox( err.code().message(), wstrErrorTitle, wxOK | wxICON_EXCLAMATION);
		}
	}
#endif
	
	return myPath;
}


boost::filesystem::path HelpPath()
{
	boost::filesystem::path myPath = wxStandardPaths::Get().GetExecutablePath().ToStdWstring();

	if (myPath.has_filename())
	{
		myPath.remove_filename();
	}

#ifdef __WXOSX_COCOA__   
	myPath /= wstrMacHelpDir;

	if (!boost::filesystem::exists(myPath))
	{
		boost::filesystem::create_directory(myPath);
	}
#endif

	return myPath;
}


boost::filesystem::path AppConfigPath()
{
	boost::filesystem::path myPath = AppDataPath();
myPath /=wstrAppConfigDir;

if (!boost::filesystem::exists(myPath))
	{
		// Create the required directory
try
		{
			boost::filesystem::create_directory(myPath);
		}
		catch(const boost::filesystem::filesystem_error& err)
		{
			wxMessageBox( err.code().message(), wstrErrorTitle, wxOK | wxICON_EXCLAMATION);
		}
}

	return myPath;
}


boost::filesystem::path SpeechPath()
{
#ifdef __WINDOWS__
	boost::filesystem::path myPath = wxStandardPaths::Get().GetExecutablePath().ToStdWstring();
#else
	boost::filesystem::path myPath = wxStandardPaths::Get().GetExecutablePath().ToStdWstring();

	if (myPath.has_filename())
	{
		myPath.remove_filename();
	}

	myPath /= wstrSpeechDir;

	if (!boost::filesystem::exists(myPath))
	{
		boost::filesystem::create_directory(myPath);
	}
#endif

	return myPath;
}


boost::filesystem::path ProtocolPath()
{
	boost::filesystem::path myPath = AppDataPath();
	myPath /= wstrProtocolDir;

	if (!boost::filesystem::exists(myPath))
	{
		try
		{
			// Create the required directory
			boost::filesystem::create_directory(myPath);
		}
		catch (const boost::filesystem::filesystem_error& err)
		{
			wxMessageBox(err.code().message(), wstrErrorTitle, wxOK | wxICON_EXCLAMATION);
		}
	}

	return myPath;
}


boost::filesystem::path SurfacePath()
{
	boost::filesystem::path myPath = AppDataPath();
myPath /=wstrSurfaceDir;

if (!boost::filesystem::exists(myPath))
	{
		try
		{
			// Create the required directory
		boost::filesystem::create_directory(myPath);
	}
		catch(const boost::filesystem::filesystem_error& err)
		{
			wxMessageBox( err.code().message(), wstrErrorTitle, wxOK | wxICON_EXCLAMATION);
		}
	}
	
	return myPath;
	}


boost::filesystem::path LogPath()
{
	boost::filesystem::path myPath = AppDataPath();
	myPath /= wstrLogDir;

	if (!boost::filesystem::exists(myPath))
	{
		// Create the required directory
		try
		{
			boost::filesystem::create_directory(myPath);
		}
		catch (const boost::filesystem::filesystem_error& err)
		{
			wxMessageBox(err.code().message(), wstrErrorTitle, wxOK | wxICON_EXCLAMATION);
		}
	}

	return myPath;
}


std::wstring PathToURL(std::wstring wstrPath)
{
	std::wstring wstrURL;
	char cSpace = 32;

	for (unsigned int i = 0; i < wstrPath.length(); i++)
	{
		if (wstrPath.at(i) != cSpace)
		{
			wstrURL.append(wstrPath.substr(i, 1));
		}
		else
		{
			wstrURL.append(L"%20%");
		}
	}
	return wstrURL;
}


std::string StripSpaces(std::string strIn)
{
	std::string strOut;
	char cSpace = 32;

	for (unsigned int i = 0; i < strIn.length(); i++)
	{
		if (strIn.at(i) != cSpace)
		{
			strOut.append(strIn.substr(i, 1));
		}
	}
	return strOut;

	// std::string strEmpty( "");
	//std::string strEmpty("");
	//std::string strOut(strIn);
	//boost::replace_all(strOut, strSpace, strEmpty);
	//return strOut;
}


std::wstring StripWSpaces(std::wstring wstrIn)
{
	std::wstring wstrOut;
	char cSpace = 32;

	for (unsigned int i = 0; i < wstrIn.length(); i++)
	{
		if (wstrIn.at(i) != cSpace)
		{
			wstrOut.append(wstrIn.substr(i, 1));
		}
	}
	return wstrOut;
}


std::string StripNonAlphaNums(std::string strIn)
{
	std::string strOut = strIn;

	boost::regex non_alnum( "[^[:alnum:]]");
	boost::algorithm::erase_all_regex( strOut, non_alnum);
	return strOut;
	// return strIn;
}

bool IsAllHexWChars(std::wstring wstrIn)
{
	std::wstring wstrTemp = StripWSpaces(wstrIn);

	if (wstrTemp.empty())
	{
		return false;
	}

	boost::algorithm::to_upper(wstrTemp);
	return (all(wstrTemp, boost::is_any_of("0123456789ABCDEF")));
}

bool IsSingleHexByteString(std::string strIn)
{
	std::string strTemp = StripSpaces(strIn);

	// We always want the user to specify a full 2-character hex byte
	if (strTemp.length() != 2)
	{
		return false;
	}
	else
	{
		// Check for valid characters
		return IsAllHexChars(strTemp);
	}
}


bool IsSingleHexByteWString(std::wstring wstrIn)
{
	std::wstring wstrTemp = StripWSpaces(wstrIn);

	// We always want the user to specify a full 2-character hex byte
	if (wstrTemp.length() != 2)
	{
		return false;
	}
	else
	{
		// Check for valid characters
		return IsAllHexWChars(wstrTemp);
	}
}


bool IsHexNibbleWString(std::wstring wstrIn)
{
	std::wstring wstrTemp = StripWSpaces(wstrIn);
	boost::algorithm::to_upper(wstrTemp);

	// We always want the user to specify a single hex digit
	if (wstrTemp.length() != 1)
	{
		return false;
	}
	else
	{
		// Check for valid characters
		return IsAllHexWChars(wstrTemp);
	}
}


bool IsHexNibbleString(std::string strIn)
{
	std::string strTemp = StripSpaces(strIn);
	boost::algorithm::to_upper(strTemp);

	// We always want the user to specify a single hex digit
	if (strTemp.length() != 1)
	{
		return false;
	}
	else
	{
		// Check for valid characters
		return IsAllHexChars(strTemp);
	}
}


bool IsHexNibbleRange(std::string strIn)
{
	std::string strTemp = StripSpaces(strIn);

	// We always want the user to specify the range prefix, plus 2 hex digits
	if (strTemp.length() != 3)
	{
		return false;
	}

	// Check for valid characters
	boost::algorithm::to_upper(strTemp);

	if ((strTemp.substr(0, 1).compare(NIBBLE_RANGE_MARKER) == 0)
		&& (IsAllHexChars(strTemp.substr(strTemp.length() - 3, 2))))
	{
		return true;
	}
	else
	{
		return false;
	}
}


bool IsWHexNibbleRange(std::wstring wstrIn)
{
	std::wstring wstrTemp = StripWSpaces(wstrIn);

	// We always want the user to specify the range prefix, plus 2 hex digits
	if (wstrTemp.length() != 3)
	{
		return false;
	}

	// Check for valid characters
	boost::algorithm::to_upper(wstrTemp);
	std::wstring wstrNIBBLE_RANGE_MARKER(NIBBLE_RANGE_MARKER.begin(), NIBBLE_RANGE_MARKER.end());

	if ((wstrTemp.substr(0, 1).compare(wstrNIBBLE_RANGE_MARKER) == 0)
		&& (IsAllHexWChars(wstrTemp.substr(wstrTemp.length() - 3, 2))))
	{
		return true;
	}
	else
	{
		return false;
	}
}


bool IsNibbleWildCard(std::string strIn)
{
	std::string strTemp = StripSpaces(strIn);

	// We always want the user to specify a single hex digit
	if (strTemp.length() != 1)
	{
		return false;
	}
	else
	{
		// Check for valid character
		boost::algorithm::to_upper(strTemp);

		if (strTemp.substr(0, 1).compare(NIBBLE_WILD_CARD_MARKER) == 0)
		{
			return true;
		}
		else
		{
			return false;
		}
	}
}


bool IsWNibbleWildCard(std::wstring wstrIn)
{
	std::wstring wstrTemp = StripWSpaces(wstrIn);

	// We always want the user to specify a single hex digit
	if (wstrTemp.length() != 1)
	{
		return false;
	}
	else
	{
		// Check for valid character
		boost::algorithm::to_upper(wstrTemp);
		std::wstring wstrNIBBLE_WILD_CARD_MARKER(NIBBLE_WILD_CARD_MARKER.begin(), NIBBLE_WILD_CARD_MARKER.end());

		if (wstrTemp.substr(0, 1).compare(wstrNIBBLE_WILD_CARD_MARKER) == 0)
		{
			return true;
		}
		else
		{
			return false;
		}
	}
}


bool IsStringOfHexBytes(std::string strIn)
{
	std::string strTemp = StripSpaces(strIn);

	// Check for a single digit
	if (strTemp.length() == 1)
	{
		// Check for valid character
		return IsAllHexChars(strTemp);
	}

	// Check that we've got an even number of characters, as each byte should be 2 characters long
	if (strTemp.length() % 2 == 1)
	{
		return false;
	}
	else
	{
		// Check for valid characters
		return IsAllHexChars(strTemp);
	}  // if odd/even
}


/*
std::vector <unsigned char> HexStringToBytes( std::string strIn)
{
std::vector <unsigned char> vOut;

try
{
boost::algorithm::unhex( strIn.begin(), strIn.end(), vOut.begin());
}
catch( ...)
{}

return vOut;
}
*/


std::string ByteToHex(unsigned int nByte)
{
	std::stringstream stream;
	stream << std::hex << nByte;
	std::string strResult(stream.str());

	if (strResult.length() == 1)
	{
		strResult.insert(0, "0");
	}

	return strResult;
}


std::string BytesToHex(std::vector <unsigned char> vBytes)
{
	std::string strResult;
	std::vector <unsigned char>::iterator it;

	for (it = vBytes.begin(); it != vBytes.end(); it++)
	{
		strResult.append(ByteToHex(*it));
	}

	return strResult;
}


int IntFromHex(const std::string &strIn)
{
	std::vector <unsigned char> vOut;
	vOut.resize(1);

	try
	{
		boost::algorithm::unhex(strIn.begin(), strIn.end(), vOut.begin());
	}
	catch (...)
	{
		vOut.push_back(-1);
	}

	return vOut.at(0);
}


int IntFromWHex(const std::wstring &wstrIn)
{
	std::vector <unsigned char> vOut;

	try
	{
		boost::algorithm::unhex(wstrIn.begin(), wstrIn.end(), vOut.begin());
	}
	catch (...)
	{
		vOut.push_back(-1);
	}

	return vOut.at(0);
}


unsigned int CharCount(std::wstring wxstrIn, wxUniChar wxucChar)
{
	unsigned int nResult = 0;

	for (unsigned int i = 0; i < wxstrIn.length(); i++)
	{
		if (wxstrIn.at(i) == wxucChar)
		{
			nResult++;
		}  // end if
	}  // end for

	return nResult;
}


std::string NibbleToHex(unsigned int nNibble)
{
	std::stringstream stream;
	stream << std::hex << nNibble;
	std::string strResult(stream.str());
	return strResult;
}


std::vector <unsigned char> GetSysExAddressBytesFromHash(std::string strMsgHash, unsigned int nHeaderLength)
{
	std::vector <unsigned char> vSysEx;

	if (strMsgHash.substr(0, 1).compare(strSysExHashPrefix) != 0)
	{
		// Not a system exclusive message, so return an empty vector
		return vSysEx;
	}

	std::string strByte;

	// Ignore the first character ("X"), which is the message type identifier
	for (unsigned int i = 1; i < strMsgHash.length(); i++)
	{
		std::string strChar = strMsgHash.substr(i, 1);

		if (strChar.compare(HASH_DELIMITER) == 0)
		{
			vSysEx.push_back(boost::lexical_cast <int> (strByte));
			strByte.clear();
		}
		else
		{
			strByte.append(strChar);
		}
	}  // end for

	// Abandon the header bytes
	std::vector <unsigned char> vAddresses;

	for (unsigned int i = nHeaderLength; i < vSysEx.size(); i++)
	{
		vAddresses.push_back(vSysEx.at(i));
	}

	return vAddresses;
}


/*
std::string ConvertBSTRToMBS(BSTR bstr)
{
	int wslen = ::SysStringLen(bstr);
	return ConvertWCSToMBS((wchar_t*)bstr, wslen);
}
*/

//  std::string ConvertWCSToMBS(const wchar_t* pstr, long wslen)
// {
// 	int len = ::WideCharToMultiByte(CP_ACP, 0, pstr, wslen, NULL, 0, NULL, NULL);

// 	std::string dblstr(len, '\0');
// 	len = ::WideCharToMultiByte(CP_ACP, 0 /* no flags */,
// 		pstr, wslen /* not necessary NULL-terminated */,
// 		&dblstr[0], len,
// 		NULL, NULL /* no default char */);

// 	return dblstr;
// }


// BSTR ConvertMBSToBSTR(const std::string& str)
// {
// 	int wslen = ::MultiByteToWideChar(CP_ACP, 0 /* no flags */,
// 		str.data(), str.length(),
// 		NULL, 0);

// 	BSTR wsdata = ::SysAllocStringLen(NULL, wslen);
// 	::MultiByteToWideChar(CP_ACP, 0 /* no flags */,
// 		str.data(), str.length(),
// 		wsdata, wslen);
// 	return wsdata;
// }
