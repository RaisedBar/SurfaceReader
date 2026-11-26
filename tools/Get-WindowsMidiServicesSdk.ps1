# Downloads the Windows MIDI Services API package (Windows.Devices.Midi2), which isn't on nuget.org yet,
# from Microsoft's official GitHub release into packages-local\, where nuget.config looks for it.
# Keep the version in step with SRMidiServices\packages.config.

param(
    [string] $Version = '0.99.92-preview.11',
    [string] $ReleaseTag = 'inbox-preview-11'
)

$ErrorActionPreference = 'Stop'
$root = Split-Path -Parent $PSScriptRoot
$folder = Join-Path $root 'packages-local'
$name = "Windows.Devices.Midi2.$Version.nupkg"
$target = Join-Path $folder $name

if (Test-Path $target) {
    Write-Host "$name is already in packages-local."
    return
}

New-Item -ItemType Directory -Force $folder | Out-Null
$url = "https://github.com/microsoft/MIDI/releases/download/$ReleaseTag/$name"
Write-Host "Downloading $url"
Invoke-WebRequest -Uri $url -OutFile $target -UseBasicParsing
Write-Host "Saved to $target"
