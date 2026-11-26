// SRVersion.h
// SurfaceReader's version, in one place: the About box, the executable's version resource and the installer
// (which reads it from the executable) all take it from here. Change it before building a release.
// Raised Bar Ltd.
// http://www.raisedbar.net

#ifndef SR_VERSION_H
#define SR_VERSION_H

#define SR_VERSION_MAJOR 1
#define SR_VERSION_MINOR 3
#define SR_VERSION_PATCH 0

#define SR_STRINGIFY2( x) #x
#define SR_STRINGIFY( x) SR_STRINGIFY2( x)
#define SR_VERSION_STRING SR_STRINGIFY( SR_VERSION_MAJOR) "." SR_STRINGIFY( SR_VERSION_MINOR) "." SR_STRINGIFY( SR_VERSION_PATCH)

#endif  // SR_VERSION_H
