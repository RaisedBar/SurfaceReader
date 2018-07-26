// RBStringFuncs.h

#ifndef RB_STRING_FUNCS_H
#define RB_STRING_FUNCS_H

#pragma once

#include <string>
#include <vector>
#include "SRConstants.h"



// String-handling functions
std::wstring PathToURL( std::wstring wxstrPath);
std::wstring StripNonAlphaNums( std::wstring strIn);
std::wstring StripSpaces(std::wstring strIn);
bool IsAllHexChars(std::wstring strIn);
bool IsSingleHexByteString( std::wstring strIn);	
bool IsHexNibbleString( std::wstring strIn);
bool IsStringOfHexBytes( std::wstring strIn);		
bool IsHexNibbleRange( std::wstring strIn);
bool IsNibbleWildCard( std::wstring strIn);
std::vector <unsigned char> HexStringToBytes( std::wstring strIn);
std::wstring ByteToHex( unsigned int nByte);
std::wstring BytesToHex( std::vector <unsigned char> vBytes);	
std::wstring NibbleToHex( unsigned int nNibble);
int IntFromHex(const std::wstring &wstrIn); 
unsigned int CharCount( std::wstring wxstrIn, wchar_t wxucChar);
std::vector <unsigned char> GetSysExAddressBytesFromHash(std::wstring strMsgHash, unsigned int nHeaderLength);
std::wstring NarrowToWideString(std::string stringToConvert);
std::string WideToNarrowString(std::wstring stringToConvert);
#endif // RB_STRING_FUNCS_H
