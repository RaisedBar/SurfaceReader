// SRConstants.h

#ifndef SR_CONSTANTS_H
#define SR_CONSTANTS_H

#pragma once

#include <string>

// String constants
const std::wstring strEmpty = L"";
const std::wstring wstrEmpty = L"";
const std::wstring strNoteOn = L"144";
const std::wstring HASH_DELIMITER = L".";
const std::wstring strSysExHashPrefix = L"X";
const std::wstring strDoubleHashPrefix = L"D";
const std::wstring strNibbleHashPrefix = L"N";
const std::wstring NIBBLE_RANGE_MARKER = L"R";
const std::wstring NIBBLE_WILD_CARD_MARKER = L"V";
const std::wstring strDecimalFormat = L"%d";
const std::wstring strHexFormat = L"%|02X|";
const std::wstring strFloatFormat = L"%|2f|";

const std::wstring strFullStop = L".";
const std::wstring strSpace = L" ";
const std::wstring strDoubleQuotes = L"\"";

// Company name
const std::wstring wstrRBCompanyName = L"Raised Bar";

// Title string for the main application window
const std::wstring wstrAppTitle = L"SurfaceReader";
const std::wstring wstrEncryptionFileName = L"Protocols";
const std::wstring wstrEncryptionFileExtension = L".sxx";
const std::string strLogExtension = ".log";
const std::wstring wstrLogExtension = L".log";
const std::wstring wstrBadLogError = L"Unable to open the log file: ";
const std::wstring wstrAppIconFileName = L"32SurfaceReader.ico";
const std::wstring wstrOptionsFileName = L"SurfaceReader.cfg";
const std::wstring wstrSpeechConfigFileName = L"Speech.RBS";
const std::wstring wstrDefaultVoiceName = L"com.apple.speech.synthesis.voice.Alex";

// Folder/directory names
const std::wstring wstrSpeechDir = L"Speech";
const std::wstring wstrAppConfigDir = L"Apps";
const std::wstring wstrLogDir = L"Logs";
const std::wstring wstrProtocolDir = L"Protocols";
const std::wstring wstrSurfaceDir = L"Surfaces";
const std::wstring wstrMacHelpDir = L"Help";

// Dialog titles and messages
const std::wstring wstrLogTitle = L"SurfaceReader log: ";
const std::wstring wstrScreenReaderFunctionsTitle = L"Screen Reader Functions";
const std::wstring wstrPickActionTitle = L"Pick an action to assign:";
const std::wstring wstrTest = L"Test";
const std::wstring wstrAddSurfaceWizardTitle = L"Add Surface Wizard";
const std::wstring wstrScreenReaderParametersTitle = L"Define Screen Reader Function Parameters";
const std::wstring wstrProtocolWizardTitle = L"Surface Protocol Wizard";
const std::wstring wstrAppConfigWizardTitle = L"Application Configuration Wizard";
const std::wstring wstrActionsPageTitle = L"Hardware Control Actions";
const std::wstring wstrProtocolActions = L" protocol actions";
const std::wstring wstrDisplaysPageTitle = L"Displays";

const std::wstring wstrDisplayContentsTitle = L"Display contents";
const std::wstring wstrDefineDisplayParametersTitle = L"Define display parameters";
	const std::wstring wstrErrorTitle = L"SurfaceReader Error";
const std::wstring wstrMIDIIn = L"MIDI in:  ";
const std::wstring wstrMIDIOut = L"MIDI out:  ";
const std::wstring wstrAlreadyRunningError = L"SurfaceReader is already running.";
const std::wstring wstrNoStringError = L"You did not provide any text to announce.";
const std::wstring wstrHelpInitError = L"Help Initialisation failed.";
const std::wstring wstrNoTaskBarSupportError = L"There appears to be no system tray support in your current environment.";
const std::wstring wstrTaskbarIconError = L"Could not set Taskbar icon.";
const std::wstring wstrDockIconError = L"Could not set Dock icon.";
const std::wstring wstrInvalidSurfaceNameError = L"You did not provide a name for your surface.";
const std::wstring wstrInvalidDisplayInError = L"You did not specify a MIDI input port for display monitoring.";
const std::wstring wstrInvalidDisplayOutError = L"You did not specify a MIDI output port for display monitoring.";
const std::wstring wstrInvalidHwInError = L"You did not specify a MIDI input port for hardware monitoring.";
const std::wstring wstrInvalidHwOutError = L"You did not specify a MIDI output port for hardware monitoring.";
const std::string strVirtualInSuffix = " display";
const std::string strVirtualOutSuffix = " hardware";
const std::wstring wstrVirtualInSuffix = L" display";
const std::wstring wstrVirtualOutSuffix = L" hardware";
const std::wstring wstrInput = L"input port: ";
const std::wstring wstrOutput = L"output port: ";
const std::wstring wstrInvalidProtocolError = L"You did not choose a protocol for your surface to use.";
const std::wstring wstrInvalidProtocolNameError = L"You did not provide a name for your protocol.";
const std::wstring wstrInvalidSysExManufacturerIDError = L"You did not supply a valid system exclusive manufacturer ID.";
const std::wstring wstrInvalidSysExModelIDError = L"You did not supply a valid system exclusive model ID.";
const std::wstring wstrInvalidSysExDeviceIDError = L"You did not supply a valid system exclusive device ID.";
const std::wstring wstrNotScreenReaderActionNodeError = L"Unable to copy an action set, as you are not within a screen-reader branch.";
const std::wstring wstrNoLiveModeActionError = L"Specified live mode action does not exist.";
const std::wstring wstrNoScreenReaderModeActionError = L"Specified screen reader mode action does not exist.";
const std::wstring wstrInfoNotCachedError = L"Specified information is not cached.";
const std::wstring wstrActionNotFoundError = L"No action found with that name";
const std::wstring wstrFailedFilesError = L"The following files failed to load:\n\n";
const std::wstring wstrFailedActionsError = L"The following actions were not copied, as they are not appropriate for the target screen-reader:\n\n";
const std::wstring wstrFileDoesNotExistError = L"The file specified doesn't exist.";
const std::wstring wstrFileLoadExceptionError = L"Exception thrown from file load";
const std::wstring wstrFileLoadError = L"File load error.";
const std::wstring wstrUnhandledExceptionError = L"Unhandled exception caught, program will terminate.";
const std::wstring wstrUnknownError = L"Unknown error: ";
const std::wstring wstrNoHotSpotBindingError = L"You did not specify a Hot Spot Clicker file to bind to this configuration.";
const std::wstring wstrHSCParsingError = L"Unable to parse the specified hot spot file. It may be over 64kb in size.";
const std::wstring wstrNoJawsJSDBindingError = L"You did not specify a Jaws file to bind to this configuration.";
const std::string strHardwareInError = "Hardware input port: ";
const std::string strHardwareOutError = "Hardware output port: ";
const std::string strDisplayInError = "Display input port: ";
const std::string strDisplayOutError = "Display output port: ";
const std::string strPortError = "The following ports could not be opened for the surface - ";
const std::wstring wstrPortOpenError = L" could not be opened.";
const std::wstring wstrNoSurfaceActionsAvailableError = L"No surface actions available.";
const std::wstring wstrNotActiveProductError = L"You can only configure the active screen reader";
const std::wstring wstrNoAppConfigError = L"No application configuration is loaded - you cannot edit the default behaviour";
const std::wstring wstrAppConfigNotFoundError = L"Unable to find a matching application configuration.";
const std::wstring wstrNoMessageDefinitionError = L"No matching message definition.";
const std::wstring wstrNoDisplayDefinitionError = L"No matching display definition.";


const std::wstring wstrEmptySurfaceNameError = L"No new surface name provided.";
const std::wstring wstrNoAppNameError = L"You didn't enter an application name.";
const std::wstring wstrDuplicatedSurfaceNameError = L"There is already a surface with that name.";
const std::wstring wstrMIDIOverwriteCheck = L" already uses that MIDI message.  Do you want to overwrite it?";
const std::wstring wstrSurfaceRemovalCheck = L"Are you sure you want to remove the surface:  ?";
const std::wstring wstrReplicationCheck = L"Other screen readers in this configuration already contain actions. Do you want to overwrite these?";
const std::wstring wstrActionsCheck = L"Are you sure you want to abandon these action definitions?";
const std::wstring wstrDeleteStateCheck = L"Are you sure you want to delete this state definition?";
const std::wstring wstrDeleteTableCheck = L"Are you sure you want to delete this translation table?";
const std::wstring wstrDeleteCharCheck = L"Are you sure you want to delete this character?";
const std::wstring wstrDuplicatedProtocolNameError = L"There is already a protocol with that name.";
const std::wstring wstrDuplicatedAppConfigNameError = L"There is already an application configuration for that application and protocol.";

const std::wstring wstrAppConfigOverwriteCheck = L"Do you want to overwrite the existing configuration for:  ";
const std::wstring wstrCancelProtocolCheck = L"Do you really want to abandon this protocol definition?";
const std::wstring wstrCancelSurfaceCheck = L"Do you really want to abandon this surface  definition?";
const std::wstring wstrAppChooserTitle = L"Choose an Application Configuration";
const std::wstring wstrProtocolChooserTitle = L"Choose a Protocol";
const std::wstring wstrAddSurfaceTitle = L"Control Surface Configuration";
const std::wstring wstrDisplaySetupTitle = L"Display MIDI Setup";
const std::wstring wstrLampSetupTitle = L"LED Lamp MIDI Setup";
const std::wstring wstrHardwareSetupTitle = L"Hardware Controls MIDI Setup";

const std::wstring wstrSysExPageTitle = L"Protocol System Exclusive Definition";
const std::wstring wstrSurfacesPrompt = L"My surfaces:";
const std::wstring wstrMessageStringPrompt = L"Enter a string to announce:";
const std::wstring wstrCursorFromLeftPrompt = L"Cursor from left?";
const std::wstring wstrUniCodePrompt = L"Unicode";
	const std::wstring wstrProtocolDisplaysPrompt = L" protocol displays";
const std::wstring wstrLineNumberPrompt = L"&Line number:";
const std::wstring wstrStripNumberPrompt = L"&Strip number:";
const std::wstring wstrStartPositionPrompt = L"&Starting position:";
const std::wstring wstrStringLengthPrompt = L"&Length:";
const std::wstring wstrProtocolEncryptionPrompt = L"&Encrypt this protocol?";
const std::wstring wstrConfirmDeletion = L"Do you really want to delete:  ";
const std::wstring wstrAppNamePrompt = L"Enter the name of the application you want to configure:";
const std::wstring wstrAppConfigNamePrompt = L"Enter the name of the application:";

const std::wstring wstrJawsJSDPrompt = L"Choose the Jaws definition file for your application program:";
const std::wstring wstrHSCPrompt = L"Choose the Hot Spot Clicker file for your application program:";
const std::wstring wstrUseGlobalStatesPrompt = L"&Use global state labels:";
const std::wstring wstrVoiceNamePrompt = L"Voice &name:";
const std::wstring wstrVoiceRatePrompt = L"&Rate:";
const std::wstring wstrVoiceVolumePrompt = L"&Volume:";
const std::wstring wstrTestButtonName = L"&Test";
const std::wstring wstrVoiceTestText = L"Testing, testing, 1, 2, 3";
const std::wstring wstrStateLabelTitle = L"Define State Label";
const std::wstring wstrStateLabelPrompt = L"&Label:";
const std::wstring wstrMSBPrompt = L"&MSB Value:";
const std::wstring wstrLSBPrompt = L"&LSB Value:";
const std::wstring wstrValuePrompt = L"&Value (Hex):";
const std::wstring wstrCharIndexPrompt = L"&Index:";
const std::wstring wstrCharLabelPrompt = L"&Label:";
const std::wstring wstrProtocolNamePrompt = L"&Protocol name:";
const std::wstring wstrManufacturerID1Prompt = L"System exclusive manufacturer ID byte &1:";
const std::wstring wstrDefaultSysExManufacturerIDByte1 = L"01";
const std::wstring wstrManufacturerID2Prompt = L"System exclusive manufacturer ID byte &2:";
const std::wstring wstrManufacturerID3Prompt = L"System exclusive manufacturer ID byte &3:";
const std::wstring wstrDefaultExtraSysExManufacturerIDByte = L"";
const std::wstring wstrSysExModelIDPrompt = L"System exclusive model &ID:";
const std::wstring wstrDefaultSysExModelID = L"";
const std::wstring wstrSysExDeviceIDPrompt = L"System exclusive de&vice ID:";
const std::wstring wstrDefaultSysExDeviceID = L"";

const std::wstring wstrHardwareControlsPageTitle = L"Hardware Control Definitions";
const std::wstring wstrDropStatusBytesPrompt = L"Drop Status &Bytes?";

const std::wstring wstrTextDisplaysPageTitle = L"Text Displays";
const std::wstring wstrDisplayNamePrompt = L"&Display name:";
const std::wstring wstrDisplayLineCountPrompt = L"Number of display &lines:";
const std::wstring wstrDefaultLines = L"1";
const std::wstring wstrDisplayLengthPrompt = L"Display lengt&h:";
const std::wstring wstrDefaultLineLength = L"1";
const std::wstring wstrDisplayStripCountPrompt = L"&Strip columns:";
const std::wstring wstrDefaultStripCount = L"1";

const std::wstring wstrLEDLampsPageTitle = L"LED Lamps";
const std::wstring wstrLEDLampNamePrompt = L"&LED lamp name:";
const std::wstring wstrDefineStates = L"Define LED &states...";
const std::wstring wstrDisplayStatesTitle = L"Define Text Display State";
const std::wstring wstrLEDStatesTitle = L"Define LED Lamp State";
const std::wstring wstrControlStatesTitle = L"Define Hardware Control State";
const std::wstring wstrTablesTitle = L"Define Translation Tables";
const std::wstring wstrDefineCharTitle = L"Define Character";
const std::wstring wstrDisplayPropertiesTitle = L"Define Display Properties";
const std::wstring wstrDefineLEDPropertiesTitle = L"Define LED Properties";
const std::wstring wstrMIDIMessageButtonName = L"&MIDI message definition...";

const std::wstring wstrMessageTypePrompt = L"Message &type:";
	const std::wstring wstrRB_NoteOn = L"Note On";
const std::wstring wstrRB_NoteOnOff = L"Note On/Off";
const std::wstring wstrRB_CC = L"CC";
const std::wstring wstrRB_PitchWheel = L"Pitch Wheel";
const std::wstring wstrRB_RPN = L"RPN";
	const std::wstring wstrRB_NRPN = L"NRPN";
const 	std::wstring wstrRB_SysEx = L"Complete SysEx";
const std::wstring wstrRB_Nibble = L"Nibble by nibble";
const std::wstring wstrAdminSwitchChar = L"x";
const std::wstring wstrSpace = L" ";
const std::wstring wstrQuestionMark = L"?";
const std::wstring wstrDoubleQuotes = L"\"";
	const std::wstring wstrFullStop = L".";
	const std::wstring wstrAsterisk = L"*";
	const std::wstring wstrDash = L"-";
		const std::wstring wstrSpacedDash = L" - "; 
		const std::string strSpacedColon = ":  ";
		const std::wstring wstrSpacedColon = L":  ";
		const std::wstring wstrColon = L":";
		const std::wstring wstrAmpersand = L"&";
const std::wstring wstrSpacedEquals = L" = L";
const std::wstring wstrSpacedComma = L", ";
const std::wstring wstrOpenSquareBracket = L"[";
const std::wstring wstrCloseSquareBracket = L"]";
const std::wstring wstrSpacedOpenParen = L" (";
const std::wstring wstrCloseParen = L")";
const std::wstring wstrNewLine = L"\n";
const std::string strNewLine = "\n";
const std::wstring wstrInvalidChannelOrNoteNumberError = L"Invalid MIDI channel or note number.";
const std::wstring wstrAppConfigCloneError = L"Unable to clone this application configuration.";
const std::wstring wstrInvalidChannelError = L"Invalid MIDI channel.";
const std::wstring wstrNoSysExAddressRemovalError = L"No system exclusive address bytes to remove.";
const std::wstring wstrNoNibbleRemovalError = L"No nibbles to remove.";
const std::wstring wstrNoteNumberPrompt = L"Note number (Dec):";
const std::wstring wstrCCNumberPrompt = L"CC number (Dec):";
const std::wstring wstrRPNMSBPrompt = L"RPN MSB:";
const std::wstring wstrNRPNNumberPrompt = L"NRPN number:";
const std::wstring wstrNibble1Prompt = L"Data1 byte, high nibble:";
const std::wstring wstrNibble2Prompt = L"Data1 byte, low nibble:";


const std::wstring wstrCommandNumberPrompt = L"Message &number:";
	const std::wstring wstrDefaultCommand = L"0";
	const std::wstring wstrChannelPrompt = L"MIDI &channel (1 - 16):";
	const std::wstring wstrDefaultChannel = L"1";
		const std::wstring wstrData1Prompt = L"Data byte &1:";
	const std::wstring wstrDefaultData1 = L"0";
	const std::wstring wstrSysExPrefixPrompt = L"System exclusive &prefix:";
const std::wstring wstrDefaultSysExPrefix = L"F0";
	const std::wstring wstrSysExSuffixPrompt = L"System exclusive &suffix:";
	const std::wstring wstrDefaultSysExSuffix = L"F7";

// Menu items
const std::wstring wstrFileMenu = L"&File";
const std::wstring wstrFileNewMenu = L"&New";
const std::wstring wstrFileImportMenu = L"&Import";
const std::wstring wstrFileExportMenu = L"&Export";
const std::wstring wstrFileProtocol = L"&Protocol definition...";
const std::wstring wstrFileAppConfig = L"&Application configuration...";
const std::wstring wstrHide = L"&Hide (F11)";

const std::wstring wstrEditMenu = L"&Edit";
const std::wstring wstrEditAppConfig = L"&Application configuration...";
const std::wstring wstrEditProtocol = L"&Protocol...";
const std::wstring wstrEditCloneProtocol = L"&Clone protocol...";
const std::wstring wstrEditCloneAppConfig = L"C&lone application configuration...";

const std::wstring wstrSurfaceMenu = L"&Surface";
const std::wstring wstrSurfaceAdd = L"&Add...";
const std::wstring wstrSurfaceLoadAppConfig = L"&Load application configuration...";
const std::wstring wstrSurfaceDisplayReport = L"&View display report...";
const std::wstring wstrSurfaceReadDisplayDelay = L"Display refresh &time...";
const std::wstring wstrSurfacePropertiesMenu = L"&Properties";
const std::wstring wstrSurfaceDisplayProperties = L"Setup MIDI &Display Monitoring...";
const std::wstring wstrSurfaceHardwareProperties = L"Setup MIDI &Hardware Monitoring...";
const std::wstring wstrSurfaceProtocolProperties = L"&Protocol...";
const std::wstring wstrRenameSurfacePrompt = L"Surface &name:";
const std::wstring wstrRename = L"&Rename...";
const std::wstring wstrRemove = L"&Delete...";
const std::wstring wstrMatchLEDEntries = L"&Match LEDs...";
const std::wstring wstrSurfaceModeMenu = L"&Mode";
const std::wstring wstrSurfaceModeLive = L"&Live";
const std::wstring wstrSurfaceModeID = L"&ID";
const std::wstring wstrSurfaceModeScreenReader = L"&Screen Reader";
const std::wstring wstrSurfaceModeConfig = L"&Configuration";

const std::wstring wstrOptionsMenu = L"&Options";
const std::wstring wstrVoiceMute = L"&Mute speech";
const std::wstring wstrVoiceMuteHelp = L"Alt+M";
const std::wstring wstrVoiceSetup = L"&Voice...";
const std::wstring wstrEmptyVoiceTestError = L"No text to speak.";
const std::wstring wstrStartMinimised = L"&Start minimised";
const std::wstring wstrStartMinimisedHelp = L"Alt+S";
const std::wstring wstrEnableLogging = L"&Logging";
const std::wstring wstrEnableLoggingHelp = L"Alt+L";
const std::wstring wstrViewLog = L"&View log...";

// Help menu
const std::wstring wstrHelpMenu = L"&Help";
const std::wstring wstrHelpContents = L"&Help Contents...";
const std::wstring wstrHelpUpdateCheck = L"&Check for updates...";

const std::wstring wstrHelpAboutCredits1 = L"Raised Bar would like to thank the following for their contributions to this project:\n"; 
const std::wstring wstrHelpAboutCredits2 = L"The Elisabeth Eagle-Bott Trust\n";
const std::wstring wstrHelpAboutCredits3 = L"The Royal National Institute for the Blind\n";
const std::wstring wstrHelpAboutCredits4 = L"Mackie/Loud Technologies\n";
const std::wstring wstrHelpAboutCredits5 = L"Musicians In Focus\n";
const std::wstring wstrHelpAboutCredits6 = L"Sean Farrow\n";
const std::wstring wstrHelpAboutCredits7 = L"David Hindmarch\n";
const std::wstring wstrHelpAboutCredits8 = L"Neil Graham\n";
const std::wstring wstrHelpAboutCredits9 = L"and especially to George Bell (Techno-Vision Systems";

// System Tray / Dock menu
const std::wstring wstrTBShow= L"&Show/Hide";
const std::wstring wstrTBQuit = L"&Quit";

// Status bar strings
const std::wstring wstrStatusNoSurfaces = L"No surfaces defined";
const std::wstring wstrStatusConnecting = L"Connecting...";
const std::wstring wstrStatusActive = L"Active";
const std::wstring wstrStatusNoSelectedSurface = L"No surface selected in the list.";

const std::wstring wstrAboutMessage = L"Version 1.0.6 PB4\n\n";

// String constants for control labels
const std::wstring wstrDefineNewDisplayPrompt = L"&Define Display:";
const std::wstring wstrDefineNewLEDLampPrompt = L"&Define LED Lamp:";
const std::wstring wstrDefineNewControlPrompt = L"&Define Controls";
const std::wstring wstrNewControlNamePrompt = L"Enter a name for this control:";
const std::wstring wstrNewDisplayNamePrompt = L"Enter a name for this display:";
const std::wstring wstrMIDIInPrompt = L"MIDI &In:";
const std::wstring wstrMIDIOutPrompt = L"MIDI &Out:";

const std::wstring wstrOnValuePrompt = L"&On value:";
const std::wstring wstrOffValuePrompt = L"Off value:";
const std::wstring wstrFlashingValuePrompt = L"Flashing value:";
const std::wstring wstrSysExPrompt = L"System exclusive &message:";
const std::wstring wstrNibblePrompt = L"&Message nibbles:";
const std::wstring wstrDefineMessageTitle = L"Define MIDI message";
const std::wstring wstrSysExBytePrompt = L"Enter a byte value as a pair of Hex digits:";
const std::wstring wstrNewNibblePrompt = L"Enter a nibble value as a single hex digit:";
const std::wstring wstrBadHexByteError = L"You did not enter a valid hex byte value.  The valid range is:  00 - ff";
const std::wstring wstrBadHexNibbleError = L"You did not enter a valid hex nibble value.  The valid range is:  0 - f";
const std::wstring wstrAddByteButtonName = L"&Add message address byte...";
const std::wstring wstrRemoveByteButtonName = L"&Remove message address byte";
const std::wstring wstrAddNibbleButtonName = L"&Add message nibble...";
const std::wstring wstrRemoveNibbleButtonName = L"&Remove message nibble.";
const std::wstring wstrAddStateButtonName = L"&Add state...";
const std::wstring wstrDeleteStateButtonName = L"&Delete state...";
const std::wstring wstrEditState = L"&Edit state...";
const std::wstring wstrTranslationTableButtonName = L"&Translation tables...";
const std::wstring wstrAddTableButtonName = L"&Add translation table...";
const std::wstring wstrEditTable = L"&Edit translation table...";
const std::wstring wstrRenameTable = L"&Rename translation table...";
const std::wstring wstrDeleteTableButtonName = L"&Delete translation table...";
const std::wstring wstrNewTableNamePrompt = L"Table &name:";
const std::wstring wstrDefinedCharactersPrompt = L"Defined &characters:";
const std::wstring wstrAddCharacter = L"&Add character...";
const std::wstring wstrEditCharacter = L"&Edit character...";
const std::wstring wstrDeleteCharacter = L"&Delete character...";
const std::wstring wstrIndexPrompt = L"Character &index:";

const std::wstring wstrDefinedDisplaysPrompt = L"&Defined displays:";
const std::wstring wstrDefinedLEDLampsPrompt = L"&Defined LED lamps:";
const std::wstring wstrDefinedStatesPrompt = L"&Defined states:";
const std::wstring wstrDefinedTablesPrompt = L"&Defined translation tables:";
const std::wstring wstrDefinedControlsPrompt = L"&Defined controls:";
const std::wstring wstrDefinedAppsPrompt = L"Defined &applications:";
const std::wstring wstrProtocolsPrompt = L"Protocols:";


const std::wstring wstrAddControlName = L"&Add...";
const std::wstring wstrEditControlName = L"&Edit...";
const std::wstring wstrLearnControlName = L"&Learn";
const std::wstring wstrDeleteControlName = L"&Delete";
const std::wstring wstrRenameControl = L"&Rename";
const std::wstring wstrFeedback = L"&Feedback...";
const std::wstring wstrControlLabelPrompt = L"&Control label:";
const std::wstring wstrControlValuePrompt = L"Control &value label:";

const std::wstring wstrOKButtonName = L"OK";
const std::wstring wstrCancelButtonName = L"Cancel";

const std::wstring wstrSurfaceNamePrompt = L"&Surface name:";
const std::wstring wstrProtocolPrompt = L"&Protocol:";
const std::wstring wstrDisplaySetupButtonName = L"Setup MIDI &Display Monitoring...";
const std::wstring wstrHardwareSetupButtonName = L"Setup Hard&ware MIDI Monitoring...";
	const std::wstring wstrAddButtonName = L"&Add...";
const std::wstring wstrUpButtonName = L"&Up";
const std::wstring wstrDownButtonName = L"Do&wn";
const std::wstring wstrRenameButtonName = L"&Rename...";
const std::wstring wstrReplicateButtonName = L"Re&plicate";
const std::wstring wstrReplicationComplete = L"Replication complete.";

// File handling
const std::wstring JSD_FILE_TEMPLATE = L"Jaws definition files (*.jsd)|*.jsd";
const std::wstring HSC_FILE_TEMPLATE = L"Hot Spot Clicker files (*.hsc)|*.hsc";
const std::wstring PROTOCOL_FILE_TEMPLATE = L"Protocol definition files (*.spu;*.spx)|*.spu;*.spx";
const std::wstring wstrPROTOCOL_EXTENSION_TEMPLATE = L".sp?";
	const std::wstring wstrUSER_PROTOCOL_EXTENSION = L".spu";
const std::wstring wstrPROTECTED_PROTOCOL_EXTENSION = L".spx";
const std::wstring wstrTEMPORARY_FILE_EXTENSION = L".tmp";
const std::string strTEMPORARY_FILE_EXTENSION = ".tmp";
const std::wstring APP_CONFIG_FILE_TEMPLATE = L"Application configuration files (*.sau;*.sax)|*.sau;*.sax";
const std::wstring wstrAPP_CONFIG_EXTENSION_TEMPLATE = L".sa?";
const std::wstring wstrUserAppConfigFileExtension = L".sau";
const std::wstring wstrPROTECTED_APP_CONFIG_EXTENSION = L".sax";
const std::wstring wstrSurfaceFileExtension = L".srs";

// Dialog control names
const std::wstring wstrPreviousPageButtonName = L"&Back...";
const std::wstring wstrNextPageButtonName = L"&Next...";
	const std::wstring wstrCancelWizardButtonName = L"Cancel";
const std::wstring wstrFinishedWizardButtonName = L"&Finish";

// Error messages
const std::wstring wstrEncryptedProtocolError = L"This protocol is not editable.";
	const std::wstring wstrNotYetImplementedError = L"Not yet implemented";
const std::wstring wstrProtocolNotFoundError = L"No matching protocol found.";
const std::wstring wstrAddVirtualInputButtonName = L"Add virtual input";
const std::wstring wstrAddVirtualOutputButtonName = L"Add virtual output";
const std::wstring wstrDisplayThruError = L"Error opening display thru port";
const std::wstring wstrHardwareThruError = L"Error opening hardware thru port";
const std::wstring wstrIncompleteSurfaceError = L"Incomplete surface definition.";
const std::wstring wstrNoConfiguredSurfacesError = L"No surfaces configured.";
const std::wstring wstrNoSurfaceRemovalError = L"No surface to remove";
const std::wstring wstrNotExistintError = L" does not exist.";
const std::wstring wstrDuplicateProtocolIDError = L"Duplicate Protocol ID: ";
const std::wstring wstrNoSelectionError = L"No selection";
const std::wstring wstrMIDILearnInPortError = L"Failed to open MIDI input port.";
const std::wstring wstrNoMIDIMessageError = L"No MIDI message was available to learn.  Please press a button or move a control before trying to Add a control.";
const std::wstring wstrNoControlNameError = L"No control label provided.";
const std::wstring wstrDuplicateControlNameError = L"A control by that name already exists.  Do you want to provide a new name?";
const std::wstring wstrDuplicateDisplayNameCheck = L"A display with that name already exists with a different MIDI message. Do you want to create another message definition for the existing display?";
const std::wstring wstrNoDisplayNameError = L"No display label provided.";
const std::wstring wstrNoMatchingLabelError = L"No matching label found to update.";
const std::wstring wstrWindowsOS = L"Windows";

// Action strings
const std::wstring wstrAvailableScreenReaderActionsPrompt = L"&Available actions:";
const std::wstring wstrNoAction = L"Do nothing";
			const std::wstring wstrReadControlLabelAction = L"Say control name";
			const std::wstring wstrReadControlStateAction = L"Say control state";
const std::wstring wstrReadControlNamePlusStateAction = L"Say control name and state";
			const std::wstring wstrReadDisplayAction = L"Read display";
			const std::wstring wstrReadDisplayLineAction = L"Read display line";
			const std::wstring wstrReadDisplayStripAction = L"Read display strip";
const std::wstring wstrReadDisplaySectionAction = L"Read display section";
const std::wstring wstrSpeakStringAction = L"Speak a string";
			const std::wstring wstrChangeSurfaceModeAction = L"Change surface mode";
			const std::wstring wstrRunHotSpotAction = L"Hot Spot";
			const std::wstring wstrScreenReaderAction = L"Function from Screen Reader";
const std::wstring wstrMIDIInfoAction = L"MIDI Info";
const std::wstring wstrReadLEDIfOnAction = L"Read LED if on";
const std::wstring wstrReadLEDIfOffAction = L"Read LED if off";
	
const std::wstring wstrStatus = L"Status ";
const std::wstring wstrChannel = L"Channel ";
const std::wstring wstrData1 = L"Data 1 = L";
const std::wstring wstrData2 = L"Data 2 = L";
const std::wstring wstrRealTime = L"Real time ";
 const std::wstring wstrChannelMode = L"Channel mode ";
const std::wstring wstrPolyAfterTouch = L"Poly after touch ";
const std::wstring wstrSysEx = L"Sys Ex";
const std::wstring wstrQFrame = L"Q frame ";
const std::wstring wstrSongPointer = L"Song pointer ";
const std::wstring wstrSongSelect = L"Song select ";
const std::wstring wstrTuneRequest = L"Tune request ";
const std::wstring wstrTimeClock = L"Time clock";
const std::wstring wstrStart = L"Start ";
const std::wstring wstrContinue = L"Continue ";
const std::wstring wstrStop = L"Stop ";
const std::wstring wstrActiveSensing = L"Active sensing ";
const std::wstring wstrSystemReset = L"System reset ";

// Status byte strings
const std::wstring wstrNoteOff = L"Note off ";
const std::wstring wstrNoteOnDescription = L"Note on ";
const std::wstring wstrCC = L"Control ";
const std::wstring wstrProgramChange = L"Program change ";
const std::wstring wstrChannelPressure = L"Channel pressure ";
const std::wstring wstrPitchWheel = L"Pitch wheel ";
const std::wstring wstrUnknownStatus = L"unknown ";

// Controller name strings
const std::wstring wstrBankSelectMSB = L"Bank Select MSB"; 
const std::wstring wstrModWheelMSB = L"Modulation Wheel MSB";
const std::wstring wstrBreathMSB = L"Breath Controller MSB";
const std::wstring wstrFootCtrlMSB = L"Foot Controller MSB";
const std::wstring  wstrPortamentoTimeMSB = L"Portamento Time MSB";
const std::wstring wstrDataEntryMSB = L"Data Entry MSB";
const std::wstring  wstrVolumeMSB = L"Channel Volume MSB";
const std::wstring wstrBalanceMSB = L"Balance MSB";
const std::wstring wstrPanMSB = L"Pan MSB"; 
const std::wstring wstrExpressionMSB = L"Expression MSB";
const std::wstring wstrEffect1MSB = L"Effect Control 1 MSB";
const std::wstring wstrEffect2MSB = L"Effect Control 2 MSB";
const std::wstring wstrGP1MSB = L"General Purpose Controller 1 MSB";
const std::wstring wstrGP2MSB = L"General Purpose Controller 2 MSB";
const std::wstring wstrGP3MSB = L"General Purpose Controller 3 MSB";
const std::wstring wstrGP4MSB = L"General Purpose Controller 4 MSB";
const std::wstring wstrBankSelectLSB = L"Bank Select LSB";
const std::wstring wstrModWheelLSB = L"Modulation Wheel LSB";
const std::wstring wstrBreathLSB = L"Breath Controller LSB";
const std::wstring wstrFootCtrlLSB = L"Foot Controller LSB";
const std::wstring wstrPortamentoTimeLSB = L"Portamento Time LSB";
const std::wstring wstrDataEntryLSB = L"Data Entry LSB";
const std::wstring wstrVolumeLSB = L"Channel Volume LSB";
const std::wstring wstrBalanceLSB = L"Balance LSB";
const std::wstring wstrPanLSB = L"Pan LSB";
const std::wstring wstrExpressionLSB = L"Expression LSB";
const std::wstring wstrEffect1LSB = L"Effect control 1 LSB";
const std::wstring wstrEffect2LSB = L"Effect control 2 LSB";
const std::wstring wstrGP1LSB = L"General Purpose Controller 1 LSB";
const std::wstring wstrGP2LSB = L"General Purpose Controller 2 LSB";
const std::wstring wstrGP3LSB = L"General Purpose Controller 3 LSB";
const std::wstring wstrGP4LSB = L"51  General Purpose Controller 4 LSB";
const std::wstring wstrSustainPedal = L"Sustain Pedal";
const std::wstring wstrPortamentoToggle = L"Portamento On/Off";
const std::wstring wstrSostenutoToggle = L" Sostenuto On/Off";
const std::wstring wstrSoftPedalToggle = L"Soft Pedal On/Off";
const std::wstring wstrLegatoFootswitch = L"Legato Footswitch";
const std::wstring wstrHold2 = L"Hold 2";
const std::wstring wstrVariation = L"Variation";
const std::wstring wstrTimbre = L"Timbre/Harmonic Intensity";
const std::wstring wstrReleaseTime = L"Release Time";
const std::wstring wstrAttackTime = L"Attack Time";
const std::wstring wstrBrightness = L"Brightness";
const std::wstring wstrDecayTime = L"Decay Time";
const std::wstring wstrVibratoRate = L"Vibrato Rate";
const std::wstring wstrVibratoDepth = L"Vibrato Depth";
const std::wstring wstrVibratoDelay = L"Vibrato Delay";
const std::wstring wstrGP5 = L"General Purpose Controller 5";
const std::wstring wstrGP6 = L"General Purpose Controller 6";
const std::wstring wstrGP7 = L"General Purpose Controller 7";
const std::wstring wstrGP8 = L"General Purpose Controller 8";
const std::wstring wstrPortamentoCtrl = L"Portamento Control";
const std::wstring wstrFX1Depth = L"FX1 Depth";
const std::wstring wstrFX2Depth = L"FX2 Depth";
const std::wstring wstrFX3Depth = L"FX3 Depth";
const std::wstring wstrFX4Depth = L"FX4 Depth";
const std::wstring wstrFX5Depth = L"FX5 Depth ";
const std::wstring wstrDataIncrement = L"Data Increment ";
const std::wstring wstrDataDecrement = L"Data Decrement";
const std::wstring wstrNRPNLSB = L"NRPN LSB";
const std::wstring wstrNRPNMSB = L"NRPN MSB";
const std::wstring wstrRPNLSB = L"RPN LSB";
const std::wstring 
	wstrRPNMSB = L"RPN MSB";
const std::wstring wstrAllSoundOff = L"All Sound Off";
const std::wstring wstrResetAllCtrl = L"Reset All Controllers";
const std::wstring wstrLocalCtrlOnOff = L"Local Control On/Off";
const std::wstring wstrAllNotesOff = L"All Notes Off";
const std::wstring wstrOmniOff = L"Omni Mode Off";
const std::wstring wstrOmniOn = L"Omni Mode On";
const std::wstring wstrMonoOn = L"Mono Mode On";
const std::wstring wstrPolyOn = L"Poly Mode On";
const std::wstring wstrUnassignedCC = L"Controller ";

const std::wstring wstrHotSpotsTitle = L"Available Hot Spots";
const std::wstring wstrAvailableScreenReaderFunctionsTitle = L"Available Screen Reader Functions";
		
// Product names
			const std::wstring wstrNone = L"None";
			const std::wstring wstrJaws = L"Jaws";
			const std::wstring wstrWE = L"Window Eyes";
			const std::wstring wstrDolphin = L"Dolphin";
const std::wstring wstrNVDA = L"NVDA";
const std::wstring wstrSA = L"System Access";
const std::wstring wstrMac = L"Mac Speech";

			// Action look-up literals
const std::wstring wstrNameField = L"Name";
const std::wstring wstrTrue = L"true";
const std::wstring wstrFalse = L"false";
const std::wstring wstrDisplayName = L"Display name";
const std::wstring wstrDisplayLine = L"Display line";
const std::wstring wstrDisplayStrip = L"Display strip";
const std::wstring wstrKeyModifiers = L"Key modifier";
const std::wstring wstrKeyName = L"Key name";
const std::wstring wstrSurfaceMode = L"Surface mode";
const std::wstring wstrPropertiesButtonName = L"&Properties...";

// Operational modes
			const std::wstring wstrLiveMode = L"Live Mode";
const std::wstring wstrIDMode = L"I D Mode";
const std::wstring wstrScreenReaderMode = L"Screen Reader Mode";
const std::wstring wstrConfigMode = L"Configuration mode";

const std::wstring wstrProgramUpdate = L"Program updates";
const std::wstring wstrProtocolUpdate = L"Protocol updates";
const std::wstring wstrAppConfigUpdate = L"Application updates";
const std::wstring wstrUpdateDescriptionPrompt = L"&Description";
const std::wstring wstrUpdateButtonName = L"&Update";
const std::wstring wstrGraphicLabel = L" Graphic ";
const std::wstring wstrTranslationTables = L"Translation Table...";
const std::wstring wstrDefaultAppName = L"Default";
const std::wstring wstrReadDisplayDelayTitle = L"Configure display refresh time";

const std::wstring wstrReadDisplayDelayPrompt = L"Delay before display reading (milliseconds):";
const std::wstring wstrReadDisplayDelayError = L"Invalid value for read display delay.";
const std::wstring wstrImportProtocolPrompt = L"Select the protocol file to import:";
const std::wstring wstrImportAppConfigPrompt = L"Select the application configuration file to import:";
const std::wstring wstrExportProtocolPrompt = L"Select the folder to back-up your protocol files:";
const std::wstring wstrExportAppConfigPrompt = L"Select the folder to back-up your application configuration files:";
const std::string strOverwritePrompt = " already exists, do you want to overwrite it?";
const std::string strCopyError = "Unable to copy: ";
const std::wstring wstrAllFilesTemplate = L"*.*";

// Unicode characters
const wchar_t wxucFullStop = 46;

// Event ID values
const int SPEECH_UPDATE_ID = 100000;
const int SURFACE_UPDATE_ID = 100001;
const int HOT_SPOT_CALL_ID = 100002;
const int SCREEN_READER_FUNCTION_CALL_ID = 100003;
const int STATUS_UPDATE_ID = 100004;
#endif // SR_CONSTANTS_H
