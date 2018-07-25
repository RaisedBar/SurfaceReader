// RBStringFuncs.cpp

#include "RBStringFuncs.h"
#include <algorithm>
#include <boost/locale.hpp>

using namespace std;
using namespace boost::locale::conv;

// Implementations

bool IsAllHexChars(wstring strIn)
{
	wstring strTemp = StripSpaces(strIn);
	return all_of(begin(strTemp), end(strTemp), isxdigit);
	}

wstring PathToURL(wstring wstrPath)
{
	replace_if(begin(wstrPath), end(wstrPath), isspace, '%20%');
	return wstrPath;
		}

wstring StripSpaces(wstring strIn)
{
remove_if(begin(strIn), end(strIn), isspace);
return strIn;
}

wstring StripNonAlphaNums(wstring strIn)
{
	remove_if(begin(strIn), end(strIn), [](wchar_t ch) {return !isalnum(ch); });
	return strIn;
}

bool IsSingleHexByteString(wstring strIn)
{
	wstring strTemp = StripSpaces(strIn);

	// We always want the user to specify a full 2-character hex byte so return if this isn't the case or if not all characters are valid hex.
	return strTemp.length() != 2 || IsAllHexChars(strTemp);
	}

bool IsHexNibbleString(wstring strIn)
{
	wstring strTemp = StripSpaces(strIn);
	
	// We always want the user to specify a single hex digit so return if this isn't the case or not all the characters in the string are valid hex.
	return strTemp.length() != 1 || IsAllHexChars(strTemp);
}

bool IsHexNibbleRange(wstring strIn)
{
	wstring strTemp = StripSpaces(strIn);

	// We always want the user to specify the range prefix, plus 2 hex digits, so return if the length is not equal to 3, the first character is not a nibble range marker or all the characters are not hex.
	return strTemp.length() != 1 || IsAllHexChars(strTemp);
	}

bool IsNibbleWildCard(wstring strIn)
{
	wstring strTemp = StripSpaces(strIn);

	// We always want the user to specify a single hex digit so return if more than one hex digit is specified or the first character is not equal to the nibble wildcard marker.
	return strTemp.length() != 1 || strTemp.substr(0, 1).compare(NIBBLE_WILD_CARD_MARKER);;
	}

bool IsStringOfHexBytes(wstring strIn)
{
	wstring strTemp = StripSpaces(strIn);
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

wstring ByteToHex(unsigned int nByte)
{
	wstringstream stream;
	stream << hex << nByte;
	wstring strResult(stream.str());

	if (strResult.length() == 1)
	{
		strResult.insert(0, L"0");
	}

	return strResult;
}

wstring BytesToHex(vector <unsigned char> vBytes)
{
	wstring strResult;
	copy(begin(vBytes), end(vBytes), begin(strResult));
	return strResult;
}

int IntFromHex(const wstring &strIn)
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

unsigned int CharCount(wstring wxstrIn, wchar_t wxucChar)
{
	return count_if(begin(wxstrIn), end(wxstrIn), [wxucChar](wchar_t ch) { return wxucChar == ch; });
	}

wstring NibbleToHex(unsigned int nNibble)
{
	wstringstream stream;
	stream << hex << nNibble;
	return stream.str();
}

vector <unsigned char> GetSysExAddressBytesFromHash(wstring strMsgHash, unsigned int nHeaderLength)
{
	std::vector <unsigned char> vSysEx;

	if (strMsgHash.substr(0, 1).compare(strSysExHashPrefix) != 0)
	{
		// Not a system exclusive message, so return an empty vector
		return vSysEx;
	}

	wstring strByte;

	// Ignore the first character ("X"), which is the message type identifier
	for (unsigned int i = 1; i < strMsgHash.length(); i++)
	{
		wstring strChar = strMsgHash.substr(i, 1);

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

wstring NarrowToWideString(string stringToConvert)
{
	if (stringToConvert.empty())
	{
		return L"";
	}
	
	return utf_to_utf<wchar_t>(stringToConvert);
}

string WideToNarrowString(wstring stringToConvert)
{
	if (stringToConvert.empty())
	{
		return "";
	}

	return utf_to_utf<char>(stringToConvert);
}