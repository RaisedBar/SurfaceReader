// SurfaceReaderHelp.h
// URL strings for the SurfaceReader help system
//

#ifndef SURFACE_HELP_H
#define SURFACE_HELP_H

#pragma once

#include <string>

// Help files
const std::wstring wstrCHMHelpFile = L"\\SurfaceReader.chm";
const std::wstring wstrHTMLHelpFile = L"contents.htm";

// Manual

// NewSurfaceWizard/ProtocolWizard
const std::wstring wstrSurfacePageHelp = L"manual/surface_page.htm";
const unsigned int AddSurfaceHelp_ID = 1;

const std::wstring wstrSysExPageHelp = L"/surfacereader/manual/system_exclusive_page.htm";
const unsigned int SysExPageHelp_ID = 2;

const std::wstring wstrTextDisplaysPageHelp = L"/surfacereader/manual/text_displays_page.htm";
const unsigned int TextDisplaysPageHelp_ID = 3;

const std::wstring wstrLEDLampsPageHelp = L"/surfacereader/manual/led_lamps_page.htm";
const unsigned int LEDLampsPageHelp_ID = 4;

const std::wstring wstrHardwareControlsPageHelp = L"/surfacereader/manual/hardware_controls_page.htm";
const unsigned int HardwareControlsPageHelp_ID = 5;

// AppConfigWizard
const std::wstring wstrActionsPageHelp = L"/surfacereader/manual/actions_page.htm";
const unsigned int ActionsPageHelp_ID = 6;

const std::wstring wstrDisplaysPageHelp = L"/surfacereader/manual/displays_page.htm";
const unsigned int DisplaysPageHelp_ID = 7;

// Tutorials
const std::wstring wstrSelectingDataHelp = L"MackieBasics10.html#1053936";

#endif
