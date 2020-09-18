// TextDisplaysPage.cpp
// Author:  T A Burgess
// Raised Bar Ltd.
// http://www.raisedbar.net

#define _HAS_STD_BYTE 0

#include "TextDisplaysPage.h"


TextDisplaysPage::TextDisplaysPage(wxWizard * wizParent, const wxString & title, std::shared_ptr<SurfaceProtocol> pProtocol)
       : wxWizardPageSimple( wizParent),
	   pMyProtocol( new SurfaceProtocol())
{
pMyProtocol = pProtocol;
SetLabel( title);

// List for the displays already defined
wxBoxSizer * vBoxDisplays = new wxBoxSizer( wxVERTICAL);
  wxStaticText * lblDefinedDisplaysPrompt = new wxStaticText(this, wxID_ANY, wstrDefinedDisplaysPrompt, wxDefaultPosition, wxSize( wxDefaultSize));
const wxString wxstrChoices[ 1];
lbxDisplayNames = new wxListBox( this, ID_DISPLAYS_LIST_BOX, wxDefaultPosition, wxSize( wxDefaultSize), 0, wxstrChoices, wxLB_SORT);
ListDefinedDisplays();
lbxDisplayNames->SetFocus();
	  vBoxDisplays ->Add( lblDefinedDisplaysPrompt, 0, wxEXPAND);  
	  	  vBoxDisplays ->Add( lbxDisplayNames, 0, wxEXPAND);  
  
		  // Buttons to manipulate the list box contents
		  wxBoxSizer * vBoxListButtons = new wxBoxSizer( wxVERTICAL);
		  btnAdd = new wxButton(this, ID_ADD, wstrAddControlName, wxDefaultPosition, wxSize( wxDefaultSize));
		  btnEdit = new wxButton(this, ID_EDIT, wstrEditControlName, wxDefaultPosition, wxSize( wxDefaultSize));
btnRename = new wxButton(this, ID_RENAME, wstrRenameControl, wxDefaultPosition, wxSize( wxDefaultSize));
btnDelete = new wxButton(this, ID_DELETE, wstrDeleteControlName, wxDefaultPosition, wxSize( wxDefaultSize));
btnTranslation = new wxButton(this, ID_TRANSLATION_TABLE, wstrTranslationTableButtonName, wxDefaultPosition, wxSize( wxDefaultSize));

// Enable/disable appropriately
if (lbxDisplayNames->GetCount() == 0)
{
	btnRename->Disable();
	btnTranslation->Disable();	
	btnDelete->Disable();
}

// Add the buttons to a sizer
vBoxListButtons->Add( btnAdd, 0, wxEXPAND);
vBoxListButtons->Add( btnEdit, 0, wxEXPAND);
vBoxListButtons->Add( btnRename, 0, wxEXPAND);
vBoxListButtons->Add( btnDelete, 0, wxEXPAND);
vBoxListButtons->Add( btnTranslation, 0, wxEXPAND);
	
	wxBoxSizer * hBox1 = new wxBoxSizer( wxHORIZONTAL);
	hBox1->Add( vBoxDisplays);
	hBox1->Add( vBoxListButtons);

	wxBoxSizer * vBox1 = new wxBoxSizer( wxVERTICAL);
	vBox1->Add( hBox1);
SetSizerAndFit(vBox1);
}


/*
TextDisplaysPage::~TextDisplaysPage()
{
}
*/

void TextDisplaysPage::ListDefinedDisplays()
{
// Clear the list box -necessary to prevent duplication due to the lack of data binding in wxListBox
	lbxDisplayNames->Clear();	

	for (unsigned int i = 0; i < pMyProtocol->GetDisplayNames().size(); i++)
	{
		if (pMyProtocol->GetDisplayItem( i).IsLEDLamp() == false)
				{
					lbxDisplayNames->Append( pMyProtocol->GetDisplayNames().at( i));
				}
	}  // end for
	}


bool TextDisplaysPage::IsValidWizardPage()
{
return ( true);
}


std::wstring TextDisplaysPage::GenerateLabel( std::wstring wstrOldLabel, unsigned int nLabelNumber)
{
std::wstring wstrOut = wstrOldLabel;
wstrOut.append( wstrOpenSquareBracket);
wstrOut.append( boost::lexical_cast <std::wstring> (nLabelNumber));
wstrOut.append( wstrCloseSquareBracket);
return wstrOut;
}


// Event handlers:

void TextDisplaysPage::OnDisplaySelect( wxCommandEvent& event)
{
// Enable/disable appropriately
	if (lbxDisplayNames->GetSelection() < 0)
{
	btnEdit->Disable();	
	btnRename->Disable();
	btnTranslation->Disable();	
	btnDelete->Disable();
}
else
	{
		btnEdit->Enable();
		btnRename->Enable();
		btnTranslation->Enable();	
		btnDelete->Enable();
}
}

		
void TextDisplaysPage::OnAddDefinition( wxCommandEvent& event)
{
DefineDisplayDialog * myNewDisplayDlg = new DefineDisplayDialog( wstrDisplayPropertiesTitle, false, pMyProtocol);

if (myNewDisplayDlg->ShowModal() == wxID_OK)
{
	if (myNewDisplayDlg->IsValidDisplay())
{
		std::wstring wstrLabel = myNewDisplayDlg->GetDisplayName();

		// Query any duplication (can't ban duplicates due to Mackie C4 LCD1)
		std::string strTempHash = 			pMyProtocol->GetDisplayHash( wstrLabel);
	
		if ((strTempHash.empty() == false)
			&& (strTempHash.compare( myNewDisplayDlg->GetDisplayHash()) != 0)) 
			{
if (		wxMessageBox( wstrDuplicateDisplayNameCheck,                wstrAppTitle, wxYES_NO | wxNO_DEFAULT | wxICON_QUESTION) == wxNO) 
{
			myNewDisplayDlg->Destroy();
		return;
		}
else  
	{
		// Generate a new, unique name
unsigned int nDuplicateNumber = 2;
std::wstring wstrNewLabel = GenerateLabel( wstrLabel, nDuplicateNumber);

while (pMyProtocol->GetDisplayHash( wstrNewLabel).empty() == false)
	{
nDuplicateNumber = 		nDuplicateNumber++;
		wstrNewLabel = GenerateLabel( wstrLabel, nDuplicateNumber);
}

// Add a new entry, but make sure the parameters match the existing entry, except for the hash, label and any address bytes
DisplayDefinition myDefinition = pMyProtocol->GetDisplay( strTempHash);
																																std::string strNewHash = myNewDisplayDlg->GetDisplayHash();
																																																																myDefinition.SetLinkedDisplayHash( strTempHash);
																																																																myDefinition.SetLabel( wstrNewLabel);
																																																																myDefinition.SetSysExAddressBytes( myNewDisplayDlg->GetSysExAddressBytes());																
																																myDefinition.SetCursorFromLeft( myNewDisplayDlg->IsCursorFromLeft());
																																myDefinition.SetUniCode( myNewDisplayDlg->IsUniCode());

																																if (strNewHash.substr( 0, 1).compare( strNibbleHashPrefix) == 0)
																																{
																																myDefinition.SetSysExCursorOffset( NIBBLE_CURSOR_OFFSET);
																																}
																																else
																																{
if (strNewHash.substr( 0, 1).compare( strSysExHashPrefix) == 0)
																																{
																																	myDefinition.SetSysExCursorOffset( (pMyProtocol->GetSysExHeader().size() + myDefinition.GetSysExAddressBytes().size()));
}
else
																																{
myDefinition.SetSysExCursorOffset( -1);
}
																																}
																																
																																pMyProtocol->SetDisplay( strNewHash, myDefinition);
																lbxDisplayNames->Append( pMyProtocol->GetDisplay( strNewHash).GetLabel()); 
lbxDisplayNames->SetStringSelection( pMyProtocol->GetDisplay( strNewHash).GetLabel());
myNewDisplayDlg->Destroy();
		return;
}
		}  // End if label not unique

// Unique label, so add a referenced definition entry to the map, then refresh the list box
																DisplayDefinition myDefinition;
																// We're not implementing MIDI shift buttons yet, so this is a place-holder
	myDefinition.SetMIDIModifier( false);

																std::string strHash = myNewDisplayDlg->GetDisplayHash();
																myDefinition.SetLabel( myNewDisplayDlg->GetDisplayName());								
																																myDefinition.IsLEDLamp( false);																
																																myDefinition.SetLineCount( myNewDisplayDlg->GetLineCount());
																myDefinition.SetDisplayLength( myNewDisplayDlg->GetDisplayLength());																	
																																myDefinition.SetStripCount( myNewDisplayDlg->GetStripCount());																
																																myDefinition.SetSysExAddressBytes( myNewDisplayDlg->GetSysExAddressBytes());																
																																myDefinition.SetCursorFromLeft( myNewDisplayDlg->IsCursorFromLeft());

																																if (strHash.substr( 0, 1).compare( strNibbleHashPrefix) == 0)
																																{
																																myDefinition.SetSysExCursorOffset( NIBBLE_CURSOR_OFFSET);
																																}
																																else
																																{
if (strHash.substr( 0, 1).compare( strSysExHashPrefix) == 0)
																																{
																																	myDefinition.SetSysExCursorOffset( (pMyProtocol->GetSysExHeader().size() + myDefinition.GetSysExAddressBytes().size()));
}
else
																																{
myDefinition.SetSysExCursorOffset( -1);
}
																																}
																																
																																pMyProtocol->SetDisplay( strHash, myDefinition);
																lbxDisplayNames->Append( pMyProtocol->GetDisplay( strHash).GetLabel()); 
lbxDisplayNames->SetStringSelection( pMyProtocol->GetDisplay( strHash).GetLabel());
}  // if Hash 0
}   // if dialog returns OK
myNewDisplayDlg->Destroy();
		}


void TextDisplaysPage::OnEditDefinition( wxCommandEvent& event)
{
int nSelection = lbxDisplayNames->GetSelection();
						
			if (nSelection < 0)
	{
		return;
			}
			
			std::wstring wstrDisplayName = lbxDisplayNames->GetStringSelection().ToStdWstring();

			// Find the associated entry within the map
		std::string strOldHash = pMyProtocol->GetDisplayHash( wstrDisplayName);			
		
		if (strOldHash.empty())
		{
			return;
		}
		
		// Store the control hash and the definition
			DisplayDefinition myDisplay = pMyProtocol->GetDisplay( strOldHash);

			// Edit the definition
			DefineDisplayDialog * myEditedDisplayDlg = new DefineDisplayDialog( wstrDisplayPropertiesTitle, lbxDisplayNames->GetStringSelection(), strOldHash, myDisplay, pMyProtocol);

if (myEditedDisplayDlg->ShowModal() == wxID_OK)
{
	if (myEditedDisplayDlg->IsValidDisplay())
{
				// Update values
	std::string strNewHash = myEditedDisplayDlg->GetDisplayHash();

	// We're not implementing MIDI shift buttons yet, so this is a place-holder
	myDisplay.SetMIDIModifier( false);

	myDisplay.SetSysExAddressBytes( myEditedDisplayDlg->GetSysExAddressBytes());
	myDisplay.SetLineCount( myEditedDisplayDlg->GetLineCount());
																myDisplay.SetDisplayLength( myEditedDisplayDlg->GetDisplayLength());																	
myDisplay.SetCursorFromLeft( myEditedDisplayDlg->IsCursorFromLeft());
myDisplay.SetUniCode( myEditedDisplayDlg->IsUniCode());
myDisplay.SetStripCount( myEditedDisplayDlg->GetStripCount());																

// Add the updated definition entry to the map, then refresh the list box
if (strOldHash.compare( strNewHash) == 0)
{
// Hash has not changed
	pMyProtocol->SetDisplay( strOldHash, myDisplay);
}
else
	{
		pMyProtocol->DeleteDisplay( strOldHash);
pMyProtocol->SetDisplay( strNewHash, myDisplay);
}

ListDefinedDisplays();
lbxDisplayNames->SetStringSelection( myDisplay.GetLabel());	
	}  // if valid
}   // if dialog returns OK
myEditedDisplayDlg->Destroy();
}

		
void TextDisplaysPage::OnDeleteDefinition( wxCommandEvent& event)
		{
			int nSelection = lbxDisplayNames->GetSelection();
						
			if (nSelection > -1)
	{
		std::wstring wstrDisplayName = lbxDisplayNames->GetStringSelection().ToStdWstring();
std::wstring wstrMessage = wstrConfirmDeletion;
wstrMessage.append( wstrDisplayName);
wstrMessage.append( wstrQuestionMark);

if (		wxMessageBox( wstrMessage,                wstrAppTitle, wxYES_NO | wxNO_DEFAULT | wxICON_QUESTION) == wxYES) 
{
// Try to remove the associated entry from the map
	std::string strHash = pMyProtocol->GetDisplayHash( wstrDisplayName);
			
	if (strHash.empty() == false)
		{
			pMyProtocol->DeleteDisplay( strHash);
			// Update the list box
			lbxDisplayNames->Delete( nSelection);
		}
}  // Deletion confirmed
			}   // end if valid selection in list box
}

		
		void TextDisplaysPage::OnRenameDisplay( wxCommandEvent& event)
		{
		int nSelection = lbxDisplayNames->GetSelection();
						
			if (nSelection > -1)
	{
		std::wstring wstrDisplayName = lbxDisplayNames->GetStringSelection().ToStdWstring();
// Try to update the associated entry within the map
		std::string strHash = pMyProtocol->GetDisplayHash( wstrDisplayName);			
		
		if (strHash.empty() == false)
		{
			// Store the control hash and the definition
			DisplayDefinition myDefinition = pMyProtocol->GetDisplay( strHash);
// Get a new control label
			RBInputBox * UserLabelDlg = new RBInputBox( wstrAppTitle, wstrNewControlNamePrompt, wstrDisplayName);

			if (UserLabelDlg->ShowModal() == wxID_OK)
		{
			std::wstring wstrLabel = UserLabelDlg->GetText();
			
			if (wstrLabel.empty())
{
	wxMessageBox( wstrNoControlNameError, wstrErrorTitle, wxOK | wxICON_ERROR);
} // end if label not empty
else
			{
				myDefinition.SetLabel( wstrLabel);				
								
// Update the map, then the list box 
				pMyProtocol->SetDisplay( strHash, myDefinition);
lbxDisplayNames->SetString( nSelection, wstrLabel);
lbxDisplayNames->SetStringSelection( wstrLabel);
			}   // end label not empty
				} // end if input box returns OK
			
			// Tidy up
			UserLabelDlg->Destroy();
}  // end if entry found in map
else
{
	wxMessageBox( wstrNoMatchingLabelError, wstrErrorTitle, wxOK | wxICON_ERROR);
}
}   // end if valid selection in list box
}


		void TextDisplaysPage::OnTranslationTable( wxCommandEvent& event)
		{
			if (lbxDisplayNames->GetSelection() > -1)
			{
	std::wstring wstrDisplayName = lbxDisplayNames->GetStringSelection().ToStdWstring();
std::string strDisplayHash = pMyProtocol->GetDisplayHash( wstrDisplayName);
TranslationTablesDialog * myTablesDlg = new TranslationTablesDialog( wstrTablesTitle, strDisplayHash, pMyProtocol);		
		
if (myTablesDlg->ShowModal() == wxID_OK)
		{
			if (myTablesDlg->IsValidTableID())
			{
				std::string strTableID = myTablesDlg->GetTableID();
				DisplayDefinition myDisplay = pMyProtocol->GetDisplay( strDisplayHash);
				myDisplay.SetCharacterTranslationID( strTableID);
				pMyProtocol->SetDisplay( strDisplayHash, myDisplay);								
			}
}  // dialog returns OK
			}  // is valid list selection
		}


		void TextDisplaysPage::OnWizardPageChanged(wxWizardEvent& event)
    {
		// Might need to list controls here
  }


void TextDisplaysPage::OnWizardCancel(wxWizardEvent& event)
    {
		std::wstring wstrMessage = wstrCancelProtocolCheck;
		
	if (		wxMessageBox( wstrMessage,                wstrAppTitle, wxYES_NO | wxNO_DEFAULT | wxICON_QUESTION) == wxNO) 
{
            event.Veto();
	}
		}

    
// We need to implement an event table in which the events received by a wxNewSurfaceDialog are routed to their respective handler functions 

	BEGIN_EVENT_TABLE(TextDisplaysPage, wxWizardPage)
// List box selection
EVT_LISTBOX( ID_DISPLAYS_LIST_BOX, TextDisplaysPage::OnDisplaySelect)

// Wizard handling
EVT_WIZARD_CANCEL(wxID_ANY, TextDisplaysPage::OnWizardCancel)
// EVT_WIZARD_PAGE_CHANGED(wxID_ANY, TextDisplaysPage::OnWizardPageChanged)

// List box manipulation buttons:
EVT_BUTTON( ID_ADD, TextDisplaysPage::OnAddDefinition)
EVT_BUTTON( ID_EDIT, TextDisplaysPage::OnEditDefinition)
EVT_BUTTON( ID_DELETE, TextDisplaysPage::OnDeleteDefinition)
EVT_BUTTON( ID_RENAME, TextDisplaysPage::OnRenameDisplay)
// EVT_BUTTON( ID_STATES, TextDisplaysPage::OnDisplayStates)
EVT_BUTTON( ID_TRANSLATION_TABLE, TextDisplaysPage::OnTranslationTable)
END_EVENT_TABLE()
