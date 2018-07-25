//Serializer.h
// Serialization/DeSerialization for the mdr.
//Copyright (©) S R Farrow/T A Burgess, 2010-2012, All Rights Reserved.

// #define _SCL_SECURE_NO_WARNINGS

#ifndef MDRSERIALIZER_H
#define MDRSERIALIZER_H

#pragma once

//Includes:
//Standard c++ headers.
#include <stdlib.h>
#include <string>
#include <iostream>
#include <fstream>
//internal
#include "SRConstants.h"
#include "RBException.h"

//boost filesystem headers.
#include <filesystem>

//boost sharedPtr headers.
#include <boost/shared_ptr.hpp>
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
	 if ((std::experimental::filesystem::exists( myFile))
&& (std::experimental::filesystem::is_regular_file( myFile)))
	 {
	//Delete the existing file and proceed only if successful
		 		 try
{
					 std::experimental::filesystem::remove( myFile);
			 }
catch (const std::experimental::filesystem::filesystem_error &error)
{
	wxMessageBox(error.what(), wstrErrorTitle, wxOK | wxICON_ERROR);
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
	wxMessageBox(error.what(), wstrErrorTitle, wxOK | wxICON_ERROR);
	return;
}

	 //Now create a WOfstream to serialize the data.
std::wofstream OutputStream(pTempFileName);
boost::archive::xml_woarchive Archive(OutputStream); //create an archive and assign the file stream.

try
{
	Archive << BOOST_SERIALIZATION_NVP(Data);
OutputStream.close();
}
catch (const std::experimental::filesystem::filesystem_error &error)
{
	wxMessageBox(error.what(), wstrErrorTitle, wxOK | wxICON_ERROR);
	return;
}

//encrypt if necessary.
if (blnEncrypt)
{ 
	try
	{
		CryptoPP::FileSource(pTempFileName.generic_string().c_str(), true, new CryptoPP::Base64Encoder(new CryptoPP::FileSink( myFile.generic_string().c_str())));
		std::experimental::filesystem::remove( pTempFileName);
	}
	catch (const std::experimental::filesystem::filesystem_error &error)
	{
		wxMessageBox(error.what(), wstrErrorTitle, wxOK | wxICON_ERROR);
		return;
	}
} //end encrypt.
else
{ //rename.
	try
	{
		std::experimental::filesystem::rename( pTempFileName, myFile);
	}
	catch (const std::experimental::filesystem::filesystem_error &error)
	{
		wxMessageBox(error.what(), wstrErrorTitle, wxOK | wxICON_ERROR);
		return;
	}
} //end rename.
}


//load data. 
//takes a filename, and loads the data returning the serializing type in the template.
template <class SerializingType>
SerializingType LoadData(const std::experimental::filesystem::path& myFile, bool blnIsEncrypted)
 {
	 	 //Check file existance.
	 if ((!std::experimental::filesystem::exists( myFile)) || (!std::experimental::filesystem::is_regular_file( myFile)))
{ //file doesn't exist.
				throw RBException( wstrFileDoesNotExistError);
	 }
	 std::experimental::filesystem::path ProcessingPath =myFile;
	 if (blnIsEncrypted)
		{ //the file is encrypted.
		ProcessingPath.replace_extension(strTEMPORARY_FILE_EXTENSION);
		CryptoPP::FileSource(myFile.generic_string().c_str(), true, new CryptoPP::Base64Decoder(new CryptoPP::FileSink( ProcessingPath.generic_string().c_str())));
	 } //end encryption.
		std::wifstream WInputStream(ProcessingPath.generic_wstring());
	 boost::archive::xml_wiarchive archive( WInputStream);
	 SerializingType Data;
try
	{
archive >> BOOST_SERIALIZATION_NVP(Data);
						WInputStream.close();
						if (blnIsEncrypted)
						{
							std::experimental::filesystem::remove(ProcessingPath);
						} //end path removal.
						}
    catch( ...)
		{
				WInputStream.close();
				if (blnIsEncrypted)
						{
remove(ProcessingPath);
						} //end path removal.
				throw RBException( wstrFileLoadExceptionError);
	}
return Data;
}
#endif