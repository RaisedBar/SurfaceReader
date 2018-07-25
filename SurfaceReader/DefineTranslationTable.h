// DefineTranslationTable.h
// Custom dialog class definition for adding MIDI message character labels
// Author:  T A Burgess
// Raised Bar Ltd.
// http://www.raisedbar.net

// #define _SCL_SECURE_NO_WARNINGS


#ifndef DEFINE_TRANSLATION_TABLE_DLG_H
#define DEFINE_TRANSLATION_TABLE_DLG_H

#pragma once

#include <map>

#include "Protocol.h"
#include "RBTextCtrl.h"
#include "SRConstants.h"
#include "RBEnums.h"
#include "DefineCharDialog.h"

#include <boost/lexical_cast.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/uuid/uuid_generators.hpp>
		#include <boost/uuid/uuid.hpp>
		#include <boost/uuid/uuid_io.hpp>         // streaming operators etc. 
#include <boost/uuid/uuid_serialize.hpp>

#include <wx/sizer.h>
#include <wx/button.h>
#include <wx/StatText.h>
#include <wx/textctrl.h>
#include <wx/listbox.h>
#include <wx/dialog.h>
#include <wx/msw/winundef.h> 


class DefineTranslationTable: public wxDialog
{
public:
DefineTranslationTable( const wxString& title, boost::shared_ptr<SurfaceProtocol> pProtocol);
	DefineTranslationTable( const wxString& title, std::wstring strTableID, boost::shared_ptr<SurfaceProtocol> pProtocol);
~DefineTranslationTable();

bool IsValidTableDefinition();
std::wstring GetTableID();
std::map <unsigned char, std::wstring> GetCharacterMap();

private:
	bool IsValidCharacterMap();
void ListDefinedCharacters();

// Event handlers:
void OnEditCharacter( wxCommandEvent& event);
	void OnOK( wxCommandEvent& event);
void OnCancel( wxCommandEvent& event);

wxListBox * lbxCharacters;

  // Internal storage
std::wstring strMyTableID;
std::map <unsigned char, std::wstring> myLabels;
boost::shared_ptr<SurfaceProtocol> pMyProtocol;

DECLARE_EVENT_TABLE()
};
#endif       // DEFINE_TRANSLATION_TABLE_DLG_H
