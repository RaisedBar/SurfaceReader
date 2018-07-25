// TranslationTablesDialog.h
// Custom dialog class definition for defining MIDI display Table translation tables
// Author:  T A Burgess
// Raised Bar Ltd.
// http://www.raisedbar.net

// #define _SCL_SECURE_NO_WARNINGS

#ifndef TRANSLATION_TABLE_DLG_H
#define TRANSLATION_TABLE_DLG_H

#pragma once

#include <map>

#include <boost/shared_ptr.hpp>

#include "MIDI.h"
#include "Protocol.h"
#include "RBTextCtrl.h"
#include "SRConstants.h"
#include "RBEnums.h"
#include "RBInputBox.h"
#include "DefineTranslationTable.h"

#include <wx/StatText.h>
#include <wx/listbox.h>
#include <wx/TextCtrl.h>
#include <wx/sizer.h>
#include <wx/msgdlg.h> 
#include <wx/button.h>
#include <wx/msw/winundef.h> 


class TranslationTablesDialog: public wxDialog
{
public:
TranslationTablesDialog( const wxString& title, std::wstring strHash, boost::shared_ptr<SurfaceProtocol> pProtocol);
~TranslationTablesDialog();

bool IsValidTableID();
std::wstring GetTableID();

private:
void CheckSelection();
void ListDefinedTables();

// Event handlers:
void OnListSelect(wxCommandEvent& event);
void OnAddTable( wxCommandEvent& event);
void OnEditTable( wxCommandEvent& event);
void OnRenameTable( wxCommandEvent& event);
void OnDeleteTable( wxCommandEvent& event);
void OnOK( wxCommandEvent& event);
void OnCancel( wxCommandEvent& event);

  wxStaticText * lblDefinedTablesPrompt;
wxListBox * lbxTables;
wxButton * AddTableButton, * RenameTableButton, * DeleteTableButton, * EditTableButton, * OKButton, * CancelButton;

// Internal storage
std::wstring strMyHash;
boost::shared_ptr<SurfaceProtocol> pMyProtocol;

DECLARE_EVENT_TABLE()
};
#endif       // TRANSLATION_TABLE_DLG_H
