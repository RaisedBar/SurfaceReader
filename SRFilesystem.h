// SRFilesystem.h
// The one place that chooses SurfaceReader's filesystem library.
// Raised Bar Ltd.
// http://www.raisedbar.net

#ifndef SR_FILESYSTEM_H
#define SR_FILESYSTEM_H

#pragma once

// Code says fs::path, fs::exists and so on.
// The code is still written against the Filesystem TS (std::experimental::filesystem),
// which the v143 toolset only provides through this header. Moving to C++17's
// std::filesystem means including <filesystem>, changing the alias below, and
// building with /std:c++17 (-std=c++17 in Xcode, whose current SDKs no longer
// have the Filesystem TS).
#include <experimental/filesystem>

namespace fs = std::experimental::filesystem;

#endif  // SR_FILESYSTEM_H
