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

#### Windows MIDI Services helper

On Windows 11 25H2 and later, SurfaceReader creates its own ports for the DAW through Windows MIDI Services, so loopMIDI isn't needed. The Windows MIDI Services API is 64-bit only, so a small x64 helper, `SRMidiServices`, manages the ports for the 32-bit SurfaceReader. It's built automatically as part of SurfaceReader and copied to `MidiServices\` next to `SurfaceReader.exe`, with `Windows.Devices.Midi2.dll` and `.pri`, which the installer has to include too.

The helper needs two NuGet packages. One is a preview that isn't on nuget.org yet, so before the first build:

```
powershell -ExecutionPolicy Bypass -File tools\Get-WindowsMidiServicesSdk.ps1
msbuild SRMidiServices\SRMidiServices.vcxproj -t:restore -p:RestorePackagesConfig=true
```

Visual Studio's own package restore works too, once the first command has been run. Without the helper, SurfaceReader builds and runs as before and uses loopMIDI ports.

How it works:

- Each DAW port is a Windows MIDI Services loopback. The DAW uses one end, named like the Mac's virtual ports (`<surface> hardware`, `<surface> display`); SurfaceReader uses the other end, the same name followed by ` (SR)`. Names are fitted to the 31-character limit of classic MIDI ports.
- The first time SurfaceReader finds Windows MIDI Services, it replaces the surfaces' loopMIDI ports with loopbacks of the same names, so DAW settings keep working. It asks the user to quit loopMIDI first if it's running, and explains the change afterwards.
- Loopbacks last until Windows restarts (or are kept, if the PC has a Windows MIDI Services configuration file), so SurfaceReader recreates any that are missing each time it starts, and removes the ones no surface uses.

### macOS (Xcode)

1. Clone vcpkg to `~/vcpkg`. If it lives somewhere else, change the `VCPKG_ROOT` user-defined build setting on the project.
2. Build. The **Install vcpkg dependencies** build phase runs `vcpkg install` for each architecture being built (`arm64-osx`, `x64-osx`) into `vcpkg_installed/`. It then writes a linker file list of every library vcpkg installed. The header path and linker flags come from the `VCPKG_*` build settings.

Both projects link every library vcpkg installs, so adding a dependency only means adding it to `vcpkg.json`.
