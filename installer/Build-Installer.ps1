<#
.SYNOPSIS
Builds SurfaceReader (Release, x64) and its installer.

.DESCRIPTION
1. Restores the Windows MIDI Services helper's NuGet packages (see README.md for the one-time SDK download).
2. Builds SurfaceReader.sln for Release x64 with MSBuild, found through vswhere.
3. Builds installer\SurfaceReader.wixproj with the .NET SDK.
The installer is written to installer\bin\x64\Release\SurfaceReader-<version>-x64.msi.

.PARAMETER SkipBuild
Only build the installer, from the Release build already in bin\Release.
#>
param(
    [switch] $SkipBuild
)

$ErrorActionPreference = 'Stop'
$repo = Split-Path -Parent $PSScriptRoot

if (-not $SkipBuild) {
    $vswhere = Join-Path ${env:ProgramFiles(x86)} 'Microsoft Visual Studio\Installer\vswhere.exe'
    if (-not (Test-Path $vswhere)) { throw 'Visual Studio 2022 with the C++ workload is needed (vswhere.exe not found).' }
    $msbuild = & $vswhere -latest -requires Microsoft.Component.MSBuild -find 'MSBuild\**\Bin\amd64\MSBuild.exe' | Select-Object -First 1
    if (-not $msbuild) { throw 'MSBuild was not found.' }

    & $msbuild (Join-Path $repo 'SRMidiServices\SRMidiServices.vcxproj') -t:Restore -p:RestorePackagesConfig=true -p:Platform=x64 -nologo -v:minimal
    if ($LASTEXITCODE -ne 0) { throw 'Restoring the Windows MIDI Services packages failed (see README.md).' }

    & $msbuild (Join-Path $repo 'SurfaceReader.sln') -p:Configuration=Release -p:Platform=x64 -m -nologo -v:minimal
    if ($LASTEXITCODE -ne 0) { throw 'Building SurfaceReader failed.' }
}

& dotnet build (Join-Path $PSScriptRoot 'SurfaceReader.wixproj') -c Release -nologo -v:minimal
if ($LASTEXITCODE -ne 0) { throw 'Building the installer failed.' }

Get-ChildItem (Join-Path $PSScriptRoot 'bin') -Recurse -Filter *.msi | Sort-Object LastWriteTime | Select-Object -Last 1 | ForEach-Object { "Installer: $($_.FullName)" }
