// TextDisplaysPage.h
// Author:  T A Burgess
// Raised Bar Ltd.
// http://www.raisedbar.net

#ifndef TEXT_DISPLAYS_MESSAGESPAGE_H
#define TEXT_DISPLAYS_MESSAGESPAGE_H

#pragma once

#include <map>

#include <boost/shared_ptr.hpp>

#include "DefineDisplayDlg.h"
#include "TranslationTablesDialog.h"
#include "RBEnums.h"
#include "RBInputBox.h"
#include "Protocol.h"

#include <wx/arrstr.h>
#include <wx/StatText.h>
#include <wx/TextCtrl.h>
#include <wx/listbox.h>
#include <wx/button.h>
#include <wx/msgdlg.h> 
#include <wx/wizard.h>
#include <wx/msw/winundef.h> 


class TextDisplaysPage: public wxWizardPageSimple
{
public:
TextDisplaysPage( wxWizard * wizParent, const wxString& title, boost::shared_ptr<SurfaceProtocol> pProtocol);
// ~TextDisplaysPage();

bool IsValidWizardPage();

virtual bool TransferDataFromWindow()
{
return IsValidWizardPage();
}

private:
void ListDefinedDisplays();
std::map <std::string, DisplayDefinition> ::iterator FindDisplayName( std::wstring wstrName);
std::wstring GenerateLabel( std::wstring wstrOldLabel, unsigned int nLabelNumber);

// Event handlers:
void OnAddDefinition( wxCommandEvent& event);
void OnEditDefinition( wxCommandEvent& event);
void OnDeleteDefinition( wxCommandEvent& event);
void OnTranslationTable( wxCommandEvent& event);
void OnRenameDisplay( wxCommandEvent& event);
void OnDisplaySelect( wxCommandEvent& event);
void OnWizardPageChanged(wxWizardEvent& event);
void OnWizardCancel(wxWizardEvent& event);
    
// Internal storage
// List box of defined display names
wxListBox * lbxDisplayNames; 
// Buttons to manipulate the list box contents
wxButton * btnAdd, * btnEdit, * btnDelete, * btnRename, * btnTranslation;

// Pointer to the protocol being modified
boost::shared_ptr<SurfaceProtocol> pMyProtocol;

DECLARE_EVENT_TABLE()
};

#endif       // TEXT_DISPLAYS_MESSAGESPAGE_H