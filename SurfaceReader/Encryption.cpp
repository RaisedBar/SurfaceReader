//Encryption.cpp
// Encryption/decryption for strings

#include "Encryption.h"
//#include <wx/msw/winundef.h> 
 

std::string Encrypt( std::string strIn)
{
std::string strOut;	
       CryptoPP::StringSource(strIn, true, new CryptoPP::Base64Encoder(new CryptoPP::StringSink( strOut))); 
return strOut;
}


std::string Decrypt( std::string strIn)
 {
	 	 std::string strOut;	
       CryptoPP::StringSource(strIn, true, new CryptoPP::Base64Decoder(new CryptoPP::StringSink( strOut))); 
return strOut;
}
