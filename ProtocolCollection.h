// ProtocolCollection.h

// #define _SCL_SECURE_NO_WARNINGS

#ifndef PROTOCOL_COLLECTION_H
#define PROTOCOL_COLLECTION_H

#pragma once

#include "Protocol.h"

//ProtocolCollection class, holding Protocols.
class	ProtocolCollection
	{
private:
	std::map <std::string, SurfaceProtocol> myProtocols;
	
	friend std::ostream & operator<<( std::ostream &os, const ProtocolCollection &C);
friend std::istream & operator>>( std::istream &is, const ProtocolCollection &C);
friend class boost::serialization::access;
	template<class Archive> 
	void serialize(Archive & ar, const unsigned int version)
	{
//serialize the whole collection of protocols.
		const char* ProtocolCollectionName = "Protocols";
		ar & boost::serialization::make_nvp(ProtocolCollectionName, myProtocols); 
		}

	public:
	ProtocolCollection();
	
	bool IsProtocolNameUnique(std::wstring wstrName);
	int Add(SurfaceProtocol myNewProtocol);
	void Remove(std::string strProtocolID);
	
	// Expose the start of the map for iteration
	std::map <std::string, SurfaceProtocol>::iterator begin();
		// Expose the end of the map for error handling
		std::map <std::string, SurfaceProtocol>::iterator end();
// Reports whether a given protocol name already exists in the map by returning its protocol ID
		std::string FindIDForProtocolName(std::wstring myProtocolName);
	// Indicates whether a specified protocol exists in the collection
		bool ProtocolExists(std::string strProtocolID);
		// Returns a protocol, based on its ID
		SurfaceProtocol GetProtocol(std::string strProtocolID);
	// Returns a protocol, based on its location in the map
		SurfaceProtocol GetProtocol(int nIndex);
	//Count.
		int count();
	
		// End of class
};

#endif