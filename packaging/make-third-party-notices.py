"""Write packaging/THIRD-PARTY-NOTICES.txt: the licences of the open source software in SurfaceReader's
Windows and macOS releases.

Run from the repository root after a Windows x64 build (it reads the licence files vcpkg installed):
    python packaging/make-third-party-notices.py

The libraries come from vcpkg.json and are the same on macOS; the screen reader and Windows MIDI Services
components are Windows only. Licence texts that vcpkg doesn't provide are kept in packaging/licences/.
"""
import os

ROOT = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
SHARE = os.path.join(ROOT, 'vcpkg_installed', 'x64-windows-static', 'x64-windows-static', 'share')
LICENCES = os.path.join(ROOT, 'packaging', 'licences')
OUT = os.path.join(ROOT, 'packaging', 'THIRD-PARTY-NOTICES.txt')

# vcpkg ports whose files are not part of SurfaceReader: build tools, and the OpenGL headers' pointer to the
# Windows SDK (wxWidgets' OpenGL support isn't used)
SKIP = {'boost-cmake', 'boost-uninstall', 'boost-headers', 'opengl', 'pkgconfig', 'doc', 'man'}

# Names to show for ports, where the port name isn't clear
NAMES = {
    'wxwidgets': 'wxWidgets', 'rtmidi': 'RtMidi', 'cryptopp': 'Crypto++', 'expat': 'Expat', 'libjpeg-turbo': 'libjpeg-turbo',
    'libpng': 'libpng', 'tiff': 'LibTIFF', 'libwebp': 'libwebp', 'zlib': 'zlib', 'pcre2': 'PCRE2', 'liblzma': 'XZ Utils (liblzma)',
    'nanosvg': 'NanoSVG', 'egl-registry': 'Khronos EGL headers', 'opengl-registry': 'Khronos OpenGL headers',
}


def read(path):
    with open(path, encoding='utf-8', errors='replace') as f:
        return f.read().strip()


def section(title, text, note=None):
    out = ['=' * 78, title, '=' * 78, '']
    if note:
        out += [note, '']
    out += [text, '', '']
    return '\n'.join(out)


parts = ['''THIRD-PARTY SOFTWARE NOTICES

SurfaceReader (c) Raised Bar Ltd is distributed under the MIT licence (LICENSE.txt). It includes the open source
software listed below, under the licences that follow.

Windows and macOS:
  wxWidgets, Boost, Crypto++, RtMidi, and the libraries wxWidgets uses for images, compression and regular
  expressions (libjpeg-turbo, libpng, LibTIFF, libwebp, zlib, XZ Utils, Expat, PCRE2, NanoSVG).
Windows only:
  NVDA Controller Client (nvdaControllerClient64.dll), System Access API client library (SAAPI64.dll),
  Windows MIDI Services API (Windows.Devices.Midi2.dll and .pri, a Microsoft preview), C++/WinRT.

''']

# ---- vcpkg libraries
ports = sorted(p for p in os.listdir(SHARE)
               if os.path.isfile(os.path.join(SHARE, p, 'copyright')) and p not in SKIP and not p.startswith('vcpkg-'))
boost = [p for p in ports if p.startswith('boost')]
others = [p for p in ports if not p.startswith('boost')]

boost_text = read(os.path.join(SHARE, boost[0], 'copyright'))
assert all(read(os.path.join(SHARE, p, 'copyright')) == boost_text for p in boost), 'Boost ports differ'
parts.append(section('Boost (%d libraries)' % len(boost), boost_text))

for p in others:
    text = read(os.path.join(SHARE, p, 'copyright'))
    if p == 'pcre2':
        # vcpkg's file only refers to the LICENCE file in the PCRE2 distribution
        text = read(os.path.join(LICENCES, 'pcre2.txt'))
    parts.append(section(NAMES.get(p, p), text))

# ---- Windows-only components
parts.append(section(
    'NVDA Controller Client (nvdaControllerClient64.dll)', read(os.path.join(LICENCES, 'nvda-controller-client.txt')),
    'Copyright NV Access Limited and contributors. Licensed under the GNU Lesser General Public License, version 2.1. SurfaceReader '
    'uses the unmodified library, loaded at run time. Its source code is in the NVDA repository: '
    'https://github.com/nvaccess/nvda (nvdaHelper/client and extras/controllerClient).'))
parts.append(section(
    'System Access API client library (SAAPI64.dll)', read(os.path.join(LICENCES, 'sa-api.txt')),
    'By Pneuma Solutions: https://github.com/PneumaSolutions/sa-api (sa_api.dll, shipped under the name SAAPI64.dll, '
    'as its documentation allows). MIT licence:'))
parts.append(section(
    'Windows MIDI Services API (Windows.Devices.Midi2.dll, Windows.Devices.Midi2.pri)',
    read(os.path.join(LICENCES, 'windows-midi-services.txt')),
    'Microsoft, https://github.com/microsoft/MIDI. This is a preview of the Windows MIDI Services API, distributed '
    'under Microsoft\'s preview "go-live" terms (see the Preview 10 and 11 release notes). It is installed only on '
    'Windows 11 25H2 or later whose Windows doesn\'t have the API built in; once it does, Windows uses its own. '
    'MIT licence:'))
parts.append(section('C++/WinRT', read(os.path.join(LICENCES, 'cppwinrt.txt')),
                     'Microsoft, https://github.com/microsoft/cppwinrt (used by the Windows MIDI Services helper).'))

with open(OUT, 'w', encoding='utf-8', newline='\r\n') as f:
    f.write('\n'.join(parts).rstrip() + '\n')
print('wrote', OUT, 'with', len(others) + 5, 'sections')
