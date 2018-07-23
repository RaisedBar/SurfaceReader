// SROptions.h
// 

#ifndef SURFACE_READER_OPTIONS_H
#define SURFACE_READER_OPTIONS_H

#pragma once

#include <boost/serialization/access.hpp>
#include <boost/serialization/nvp.hpp>

#include <wx/msw/winundef.h> 


class SurfaceReaderOptions
{
public:
	SurfaceReaderOptions()
	:
blnStartSized( true)
	{};

bool GetStartSized()
{
	return blnStartSized;
};

	void SetStartSized( bool blnNewStartSized)
{
	blnStartSized = blnNewStartSized;
};

private:
friend std::ostream & operator<<( std::ostream &os, const SurfaceReaderOptions &SRO);
friend std::istream & operator>>( std::istream &is, const SurfaceReaderOptions &SRO);
friend class boost::serialization::access;
template<class Archive>
void serialize(Archive & ar, const unsigned int version)
{
try
	{
		const char* cStartSizedTag ="StartSized";
ar & boost::serialization::make_nvp(cStartSizedTag, blnStartSized);
}  // end try
catch( ...)
{
throw ID_LOAD_DATA_EXCEPTION;
}  // end catch
}
	
	// Internal storage
bool blnStartSized;
};
#endif  // define