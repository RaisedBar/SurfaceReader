# SurfaceReader
The code for the SurfaceReader application

## Building

wxWidgets, Boost, Crypto++ and RtMidi come from [vcpkg](https://github.com/microsoft/vcpkg) in manifest mode. The libraries are listed in `vcpkg.json`, and both projects install them automatically the first time you build. Nothing else needs to be installed by hand, apart from the screen reader SDKs that are already in the repository.

### One-time setup

```
git clone https://github.com/microsoft/vcpkg
cd vcpkg
./bootstrap-vcpkg.sh        # bootstrap-vcpkg.bat on Windows
```

The first build compiles the libraries, which takes a while. Later builds reuse vcpkg's binary cache.

### Windows (Visual Studio 2022)

1. Set the `VCPKG_ROOT` environment variable to your vcpkg folder, or run `vcpkg integrate install` once.
2. Open `SurfaceReader.sln` and build. MSBuild runs `vcpkg install` for the static triplet that matches the platform (`x86-windows-static` or `x64-windows-static`) and links the libraries automatically.

The project uses the v143 toolset and the static CRT (`/MT`, `/MTd`), matching the static triplets.

### macOS (Xcode)

1. Clone vcpkg to `~/vcpkg`. If it lives somewhere else, change the `VCPKG_ROOT` user-defined build setting on the project.
2. Build. The **Install vcpkg dependencies** build phase runs `vcpkg install` for each architecture being built (`arm64-osx`, `x64-osx`) into `vcpkg_installed/`. It then writes a linker file list of every library vcpkg installed. The header path and linker flags come from the `VCPKG_*` build settings.

Both projects link every library vcpkg installs, so adding a dependency only means adding it to `vcpkg.json`.
