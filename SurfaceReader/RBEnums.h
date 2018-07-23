// RBEnums.h

#ifndef RB_ENUM_H
#define RB_ENUM_H

#pragma once


enum RBEnum
{
SURFACE_PAGE = 1,
SYSEX_PAGE,
TEXT_DISPLAYS_PAGE,
LED_LAMPS_PAGE,
HARDWARE_MESSAGES_PAGE,

// Dialog navigation
ID_BACK,
ID_NEXT,
ID_FINISHED,
		ID_NO_CHANGE,
				ID_UP,
		ID_DOWN,
ID_STATES,
ID_GlobalStates,
ID_ADD,
ID_LEARN,
ID_EDIT,
ID_DELETE,
ID_RENAME,
ID_Replicate,
ID_MATCH_LED,
ID_PROPERTIES_BUTTON,
ID_ADD_NIBBLE,
ID_DELETE_NIBBLE,

// Exception ID values
ID_NO_Display_DEFINITION,
ID_NO_SURFACE_ACTIONS,
ID_LOAD_DATA_EXCEPTION,
ID_ACTION_DEFINITION_CANCELLED,
ID_NO_ACTION_PARAMETER,
ID_NO_MATCHING_CHARACTER,
ID_NO_MATCHING_APP_CONFIG,
ID_NO_MATCHING_MESSAGE_DEFINITION,
ID_NO_MATCHING_PROTOCOL_NAME,
ID_NO_MATCHING_PROTOCOL_ID,
ID_NO_MATCHING_CHAR_TABLE,
ID_DEFINE_MIDI,
ID_ENCRYPTED,
	
	// MIDI Message types - can be used for control ID values
ID_NOTE_ON,
	ID_NOTE_ON_OFF,
	ID_CC,
	ID_PITCH_WHEEL,
	ID_RPN,
	ID_NRPN,
	ID_SYSEX,
	ID_NIBBLE,

// Menu ID values
ID_FileNew,
ID_NewProtocol,
ID_NewAppConfig,
ID_FileImport,
ID_ImportProtocol,
ID_ImportAppConfig,
ID_FileExport,
ID_ExportProtocol,
ID_ExportAppConfig,
ID_Hide,

ID_EditProtocol,
ID_EditAppConfig,
ID_EditState,
ID_EditCloneProtocol,
ID_EditCloneAppConfig,

ID_SurfaceAdd,
ID_SurfaceLoadAppConfig,
ID_SurfaceProperties,
	ID_SurfaceDisplayMIDIProperties,
	ID_SurfaceHardwareMIDIProperties,
ID_SurfaceProtocolProperties,	
ID_SurfaceDelayProperties,
ID_SurfaceBufferProperties,
ID_SurfaceRename,
ID_SurfaceRemove,
ID_SurfaceMode,
ID_SurfaceModeLive,
ID_SurfaceModeID,
ID_SurfaceModeScreenReader,
ID_SurfaceModeConfig,
ID_SurfaceDisplayReport,

ID_VoiceMute,
ID_VoiceSetup,
ID_StartMinimised,
ID_EnableLogging,
ID_ViewLog,

ID_HelpContents,
ID_Help_Update_Check,

// Context-sensitive help
ID_WizardHelp,
ID_HelpContextSurfaceList,

	    // Control ID values
ID_INPUT_BOX_TEXT,
ID_MIDI_IN_LIST_BOX,
ID_AddVirtualInput,
ID_AddVirtualOutput,

ID_DISPLAY_NAME, 
ID_DISPLAY_SYSEX_CURSOR_OFFSET,
ID_display_line_count,
ID_DISPLAY_LINE_LENGTH,
ID_TRANSLATION_TABLE,

ID_MIDI_OUT_LIST_BOX,
ID_SURFACE_LIST_BOX,
ID_STATES_LIST_BOX,
ID_Tables_LIST_BOX,
ID_Apps_LIST_BOX,
ID_CHARACTERS_LIST_BOX,
ID_CONTROLS_LIST_BOX,
ID_DISPLAYS_LIST_BOX,
ID_LAMPS_LIST_BOX,
ID_PROTOCOL_LIST_BOX,
ID_DISPLAY_PARAMETER1_LIST_BOX,
ID_DISPLAY_PARAMETER2_LIST_BOX,
ID_DisplaySetup,
		ID_HardwareSetup,
ID_DEFINED_CONTROLS_LIST_BOX,
								ID_FEEDBACK,
		ID_ACTION_TREE,
ID_ACTIONS_LIST_BOX,
ID_UPDATE_TREE,

// Parameter ID values for SurfaceAction
ID_PARAM_DISPLAY_NAME,
ID_PARAM_DISPLAY_LINE,
ID_PARAM_DISPLAY_COLUMN,
ID_PARAM_KEY_MODIFIER,
ID_PARAM_KEY_NAME,
ID_PARAM_SURFACE_MODE,
	ID_VOICE_NAMES_LIST_BOX,
	ID_TEST,
	ID_DROP_STATUS_BYTES,
	ID_CursorFromLeft,
	ID_UniCode,
	ID_ToggleDisplays
};

#endif // RB_ENUM_H
