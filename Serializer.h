//Serializer.h
// Serialization/DeSerialization for the mdr.
//Copyright (©) S R Farrow/T A Burgess, 2010-2012, All Rights Reserved.

#ifndef MDRSERIALIZER_H
#define MDRSERIALIZER_H

#pragma once

//Includes:
//Standard c++ headers.
#include <stdlib.h>
#include <string>
#include <iostream>
#include <fstream>
#include <experimental/filesystem>

//internal
#include "SRConstants.h"
#include "RBException.h"
// #include "FileCorrecter.h"

//boost filesystem headers.
// #define BOOST_FILESYSTEM_VERSION 3
// #include <boost/filesystem/fstream.hpp>
// #include <boost/filesystem.hpp>

//boost sharedPtr headers.
// #include <boost/shared_ptr.hpp>
//Boost xml serialization headers.
#include <boost/archive/xml_wiarchive.hpp>
#include <boost/archive/xml_woarchive.hpp>

//Data type boost serialization headers.
#include <boost/config.hpp>
#include <boost/serialization/string.hpp>
#include <boost/Serialization/Vector.hpp>
#include <boost/Serialization/Map.hpp>
#include <boost/serialization/shared_ptr.hpp>
#include <boost/Serialization/nvp.hpp>


//crypto++.
#include <base64.h>
#include <files.h>
#include <filters.h>

#include <wx/msw/winundef.h> 

//Template method for serializing specific data types.

template <class SerializingType>
void SaveData(SerializingType Data, std::experimental::filesystem::path myFile, bool blnEncrypt)
 {
//Check file existance.
	 if ((exists( myFile))    
&& (is_regular_file( myFile)))
	 {
	//Delete the existing file and proceed only if successful
		 		 try
{
	remove( myFile);
			 }
catch (const std::experimental::filesystem::filesystem_error &error)
{
	throw RBException(error.what());
	return;
	}
	 }
	 
	std::experimental::filesystem::path pTempFileName = myFile;
	 
	try
	{
		pTempFileName.replace_extension( wstrTEMPORARY_FILE_EXTENSION);
}
catch (const std::experimental::filesystem::filesystem_error &error)
{
	throw RBException(error.what());
	return;
}

	 try
{
		 //Create a WOfstream to serialize the data.
		 wofstream OutputStream(pTempFileName);
		 		 boost::archive::xml_woarchive Archive(OutputStream); //create an archive and assign the file stream.
		Archive << BOOST_SERIALIZATION_NVP(Data);
}
catch (const std::experimental::filesystem::filesystem_error &error)
{
	throw RBException(error.what());
	return;
}

//encrypt if necessary.
if (blnEncrypt)
{ 
	try
	{
		CryptoPP::FileSource(pTempFileName.generic_string().c_str(), true, new CryptoPP::Base64Encoder(new CryptoPP::FileSink( myFile.generic_string().c_str())));
		remove(pTempFileName);
	}
	catch (const std::experimental::filesystem::filesystem_error &error)
	{
		remove(pTempFileName);
		throw RBException(error.what());
				return;
	}
} //end encrypt.
else  //Not encrypted, so just rename.
{
		try
	{
		rename(pTempFileName, myFile);
	}
	catch (const std::experimental::filesystem::filesystem_error &error)
	{
		throw RBException(error.what());
		return;
	}
}
}


//load data. 
//takes a filename, and loads the data returning the serializing type in the template.
template <class SerializingType>
SerializingType LoadData(const std::experimental::filesystem::path& myFile, bool blnIsEncrypted)
 {
	SerializingType Data;
	
	//Check file existance.
	 if ((! exists( myFile)) || (! is_regular_file( myFile)))
{ //file doesn't exist.
		 std::wstring wstrError = myFile.generic_wstring();
		 throw RBException(wstrError);
	 }
	
	 std::experimental::filesystem::path ProcessingPath =myFile;	
	 	 if (blnIsEncrypted)
		{ //the file is encrypted.
		ProcessingPath.replace_extension(strTEMPORARY_FILE_EXTENSION);
		CryptoPP::FileSource(myFile.generic_string().c_str(), true, new CryptoPP::Base64Decoder(new CryptoPP::FileSink( ProcessingPath.generic_string().c_str())));
	 } //end decryption.
	
	 	 		 				 /*
								 // Fix malformed files caused by change in Boost 1.66.0 and later
		 FileCorrecter fc;
		 fc.FixFileWithInvalidSerialization(ProcessingPath.generic_string().c_str());

// Check that the fix worked
		 if (fc.IsFileSerializationInvalid(ProcessingPath.generic_string().c_str()))
		 {
			 throw RBException( std::string( "Unable to fix file: ").append(ProcessingPath.generic_string().c_str()));
		 }
		 */

		 		 try
			{
				{
					wifstream WInputStream(ProcessingPath.generic_wstring());
					boost::archive::xml_wiarchive archive(WInputStream);
					archive >> BOOST_SERIALIZATION_NVP(Data);
				}

if (blnIsEncrypted)
				{
		// Destroy the unencrypted temporary file
	remove(ProcessingPath);
				} //end path removal.
			}
			catch (boost::archive::archive_exception & myException)
			{
								if (blnIsEncrypted)
				{
									// Destroy the unencrypted temporary file
									remove(ProcessingPath);
				} //end path removal.
				throw RBException(wstrFileLoadExceptionError);
			} // end catch

		return Data;
}

#endif
