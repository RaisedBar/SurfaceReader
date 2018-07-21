//Encryption.h
// Encryption/decryption for strings

#ifndef ENCRYPTER_H
#define ENCRYPTER_H

#pragma once

//crypto++.
#include <base64.h>
#include <filters.h>
#include <wx/msw/winundef.h> 


std::string Encrypt( std::string strIn);
std::string Decrypt( std::string strIn);
#endif
