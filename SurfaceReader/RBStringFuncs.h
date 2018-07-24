// RBStringFuncs.h

#ifndef RB_STRING_FUNCS_H
#define RB_STRING_FUNCS_H

#pragma once

#include <boost/algorithm/string.hpp>
#include <boost/algorithm/hex.hpp>
#include <boost/filesystem.hpp>
#include <boost/regex.hpp>
#include <boost/algorithm/string/regex.hpp>
#include <boost/lexical_cast.hpp>
using boost::lexical_cast;
using boost::bad_lexical_cast;
// using namespace boost;

#include <string>
//#include <algorithm>
//#include <vector>
//#include <iomanip>
using namespace std;


//#include <Windows.h>
//#include <wx/msw/winundef.h> 

#include "SRConstants.h"

#include <wx/stdpaths.h>
#include <wx/msgdlg.h>
#include <wx/msw/winundef.h> 


// String-handling functions
//.remove when tim updates things.
boost::filesystem::path AppDataPath();
boost::filesystem::path HelpPath();
boost::filesystem::path AppConfigPath();
boost::filesystem::path SpeechPath();
boost::filesystem::path ProtocolPath();
boost::filesystem::path SurfacePath();
boost::filesystem::path LogPath();
std::wstring PathToURL( std::wstring wxstrPath);

std::wstring StripWSpaces( std::wstring wstrIn);
std::string StripNonAlphaNums( std::string strIn);
std::string StripSpaces(std::string strIn);
bool IsAllHexChars(std::string strIn);
bool IsAllHexWChars(std::wstring wstrIn);
bool IsSingleHexByteString( std::string strIn);	
bool IsSingleHexByteWString( std::wstring wstrIn);	
bool IsHexNibbleString( std::string strIn);
bool IsHexNibbleWString( std::wstring wstrIn);
bool IsStringOfHexBytes( std::wstring strIn);		
bool IsWHexNibbleRange( std::wstring wstrIn);
bool IsHexNibbleRange( std::string strIn);
bool IsNibbleWildCard( std::string strIn);
bool IsWNibbleWildCard( std::wstring wstrIn);
std::vector <unsigned char> HexStringToBytes( std::string strIn);
std::string ByteToHex( unsigned int nByte);
std::string BytesToHex( std::vector <unsigned char> vBytes);	
std::string NibbleToHex( unsigned int nNibble);
int IntFromHex(const std::string &wstrIn); 
int IntFromWHex(const std::wstring &wstrIn); 
unsigned int CharCount( std::wstring wxstrIn, wxUniChar wxucChar);
std::vector <unsigned char> GetSysExAddressBytesFromHash(std::string strMsgHash, unsigned int nHeaderLength);
// std::string ConvertBSTRToMBS(BSTR bstr);
std::string ConvertWCSToMBS(const wchar_t* pstr, long wslen);
// BSTR ConvertMBSToBSTR(const std::string& str);
#endif // RB_STRING_FUNCS_H
