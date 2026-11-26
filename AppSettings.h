// AppSettings.h
// Settings read from SurfaceReader.json, in SurfaceReader's data folder.
// Raised Bar Ltd.
// http://www.raisedbar.net

#ifndef SR_APP_SETTINGS_H
#define SR_APP_SETTINGS_H

#pragma once

#include "SRFilesystem.h"

/* SurfaceReader.json holds settings meant to be changed by editing the file, for example:

{
    "speech": {
        "settleTimeSeconds": 0.2
    }
}

If the file is missing it is created with the defaults. A setting that is missing or invalid keeps its default,
so a damaged file never stops SurfaceReader starting. The file is read once, when SurfaceReader starts. */

namespace AppSettings
{
	// Default time, in seconds, that a fader or encoder must stay still before it is announced
	const double dDefaultSettleTimeSeconds = 0.2;
	// The longest settle time accepted, so a typing slip can't silence the controls
	const double dMaxSettleTimeSeconds = 5.0;

	// Read SurfaceReader.json, creating it with the defaults if it doesn't exist. Call once at startup.
	void Load();
	// The same, for a given file (for tests)
	void Load( const fs::path & myFile);

	// How long, in milliseconds, a fader or encoder must stay still before it is announced (0 = at once).
	// Safe to call from any thread.
	int GetSettleTimeMilliseconds();
}

#endif  // SR_APP_SETTINGS_H
