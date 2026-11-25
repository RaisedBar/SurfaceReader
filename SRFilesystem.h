// SRFilesystem.h
// The one place that chooses SurfaceReader's filesystem library.
// Raised Bar Ltd.
// http://www.raisedbar.net

#ifndef SR_FILESYSTEM_H
#define SR_FILESYSTEM_H

#pragma once

// Code says fs::path, fs::exists and so on.
#include <filesystem>

namespace fs = std::filesystem;

#endif  // SR_FILESYSTEM_H
