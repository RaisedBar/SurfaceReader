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
2. Open `SurfaceReader.sln` and build. MSBuild runs `vcpkg install` for the `x64-windows-static` triplet and links the libraries automatically.

SurfaceReader for Windows is 64-bit only. The project uses the v143 toolset and the static CRT (`/MT`, `/MTd`), matching the static triplet, so it needs no Visual C++ runtime. It builds into `bin\Debug` or `bin\Release`, and copies the screen reader DLLs and help file there so it can run straight away.

#### Windows MIDI Services helper

On Windows 11 25H2 and later, SurfaceReader creates its own ports for the DAW through Windows MIDI Services, so loopMIDI isn't needed. A small helper, `SRMidiServices`, manages the ports in its own process, so a problem in the preview Windows MIDI Services SDK can't take SurfaceReader down. It's built automatically as part of SurfaceReader and copied to `MidiServices\` next to `SurfaceReader.exe`, with `Windows.Devices.Midi2.dll` and `.pri` for development.

The installer ships the helper but not Microsoft's `Windows.Devices.Midi2.dll` and `.pri`: the preview API may only be redistributed under Microsoft's preview "go-live" terms. Installed copies of SurfaceReader therefore use Windows MIDI Services once Windows includes the API itself, and loopMIDI ports until then.

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

#### Installer

`installer\` holds the Windows installer, a WiX Toolset 5 project that the .NET SDK builds (NuGet fetches WiX itself, so it doesn't need installing). After the one-time setup above:

```
powershell -ExecutionPolicy Bypass -File installer\Build-Installer.ps1
```

This builds SurfaceReader (Release, x64), then `installer\bin\x64\Release\SurfaceReader-<version>-x64.msi`. The version comes from `SRVersion.h`, which also sets the About box and the executable's version: raise it for each release, or the new installer won't replace the old one.

The installer:

- puts SurfaceReader in `Program Files\Raised Bar\SurfaceReader`, with Start menu shortcuts to SurfaceReader and its help, and a desktop shortcut;
- puts the supplied protocols and app configurations in `ProgramData\Raised Bar\SurfaceReader` and lets all users change that folder;
- replaces any earlier SurfaceReader, including the old 32-bit one, keeping protocols the user has changed.

### macOS (Xcode)

1. Clone vcpkg to `~/vcpkg`. If it lives somewhere else, change the `VCPKG_ROOT` user-defined build setting on the project.
2. Build. The **Install vcpkg dependencies** build phase runs `vcpkg install` for each architecture being built (`arm64-osx`, `x64-osx`) into `vcpkg_installed/`. It then writes a linker file list of every library vcpkg installed. The header path and linker flags come from the `VCPKG_*` build settings.

Both projects link every library vcpkg installs, so adding a dependency only means adding it to `vcpkg.json`.

## Settings file

SurfaceReader reads `SurfaceReader.json` from its data folder when it starts (`ProgramData\Raised Bar\SurfaceReader` on Windows), creating it with the defaults if it is missing:

```json
{
    "speech": {
        "settleTimeSeconds": 0.2
    }
}
```

- `speech.settleTimeSeconds`: how long a fader, V-Pot, jog wheel or other continuous control must stay still before SurfaceReader announces it, so moving one isn't announced at every step. Buttons and fader touches are announced at once. 0 announces every change; values from 0 to 5 are accepted, and anything else keeps the default of 0.2.
