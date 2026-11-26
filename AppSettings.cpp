// AppSettings.cpp
// Settings read from SurfaceReader.json, in SurfaceReader's data folder.
// Raised Bar Ltd.
// http://www.raisedbar.net

#include "AppSettings.h"
#include "RBStringFuncs.h"
#include "SRConstants.h"

#include <atomic>
#include <cmath>
#include <fstream>

#include <boost/property_tree/json_parser.hpp>
#include <boost/property_tree/ptree.hpp>

namespace AppSettings
{
	namespace
	{
		std::atomic <int> nSettleTimeMilliseconds( (int) std::lround( dDefaultSettleTimeSeconds * 1000.0));

		// Written by hand rather than with write_json, which would store the number as a string
		const char * szDefaultFile =
			"{\n"
			"    \"speech\": {\n"
			"        \"settleTimeSeconds\": 0.2\n"
			"    }\n"
			"}\n";
	}


	void Load()
	{
		Load( AppDataPath() / wstrSettingsFileName);
	}


	void Load( const fs::path & myFile)
	{
		std::error_code myError;

		if (fs::exists( myFile, myError) == false)
		{
			// Create it with the defaults, so there's a file to edit. Not being able to is not an error:
			// the defaults apply.
			std::ofstream myOut( myFile);
			if (myOut)
			{
				myOut << szDefaultFile;
			}
			return;
		}

		boost::property_tree::ptree myTree;
		try
		{
			std::ifstream myIn( myFile);
			boost::property_tree::read_json( myIn, myTree);
		}
		catch (const boost::property_tree::ptree_error &)
		{
			// Not valid JSON: keep the defaults
			return;
		}

		boost::optional <double> dSettle = myTree.get_optional <double>( "speech.settleTimeSeconds");
		if (dSettle && std::isfinite( *dSettle) && (*dSettle >= 0.0) && (*dSettle <= dMaxSettleTimeSeconds))
		{
			nSettleTimeMilliseconds = (int) std::lround( *dSettle * 1000.0);
		}
	}


	int GetSettleTimeMilliseconds()
	{
		return nSettleTimeMilliseconds;
	}
}
