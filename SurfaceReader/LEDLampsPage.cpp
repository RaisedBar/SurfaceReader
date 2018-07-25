//LEDLampsPage.cpp
// Author:  T A Burgess
// Raised Bar Ltd.
// http:LEDLampsPagewww.raisedbar.net

#include "LEDLampsPage.h"


LEDLampsPage::LEDLampsPage(wxWizard * wizParent, const wxString & title, boost::shared_ptr<SurfaceProtocol> pProtocol)
       : wxWizardPageSimple( wizParent),
	   pMyProtocol( new SurfaceProtocol())
{  
pMyProtocol = pProtocol;
SetLabel( title);

// List for the lamps already defined
wxBoxSizer * vBoxLamps = new wxBoxSizer( wxVERTICAL);
  wxStaticText * lblDefinedLampsPrompt = new wxStaticText(this, wxID_ANY, wstrDefinedLEDLampsPrompt, wxDefaultPosition, wxSize( wxDefaultSize));
const wxString wxstrChoices[ 1];
lbxLampNames = new wxListBox( this, ID_LAMPS_LIST_BOX, wxDefaultPosition, wxSize( wxDefaultSize), 0, wxstrChoices, wxLB_SORT);
ListDefinedLamps();
lbxLampNames->SetFocus();
	  vBoxLamps ->Add( lblDefinedLampsPrompt, 0, wxEXPAND);  
	  	  vBoxLamps ->Add( lbxLampNames, 0, wxEXPAND);  
  
		  // Buttons to manipulate the list box contents
		  wxBoxSizer * vBoxListButtons = new wxBoxSizer( wxVERTICAL);
		  		btnAdd = new wxButton(this, ID_ADD, wstrAddControlName, wxDefaultPosition, wxSize( wxDefaultSize));
btnEdit = new wxButton(this, ID_EDIT, wstrEditControlName, wxDefaultPosition, wxSize( wxDefaultSize));
		  btnRename = new wxButton(this, ID_RENAME, wstrRenameControl, wxDefaultPosition, wxSize( wxDefaultSize));
btnDelete = new wxButton(this, ID_DELETE, wstrDeleteControlName, wxDefaultPosition, wxSize( wxDefaultSize));
wxchkUseGlobalStates = new wxCheckBox( this, ID_GlobalStates, wstrUseGlobalStatesPrompt);
		wxchkUseGlobalStates->SetValue( pMyProtocol->IsUsingGlobalStates());		
btnStates = new wxButton(this, ID_STATES, wstrDefineStates, wxDefaultPosition, wxSize( wxDefaultSize));
		
// Enable/disable appropriately
if (lbxLampNames->GetCount() == 0)
{
	btnStates->Disable();
	btnRename->Disable();
	btnDelete->Disable();
}

// Add the buttons to a sizer
vBoxListButtons->Add( btnAdd, 0, wxEXPAND);
vBoxListButtons->Add( btnEdit, 0, wxEXPAND);
vBoxListButtons->Add( btnRename, 0, wxEXPAND);
vBoxListButtons->Add( btnDelete, 0, wxEXPAND);
vBoxListButtons->Add( wxchkUseGlobalStates, 0, wxEXPAND);
vBoxListButtons->Add( btnStates, 0, wxEXPAND);
	
	wxBoxSizer * hBox1 = new wxBoxSizer( wxHORIZONTAL);
	hBox1->Add( vBoxLamps);
	hBox1->Add( vBoxListButtons);

	wxBoxSizer * vBox1 = new wxBoxSizer( wxVERTICAL);
	vBox1->Add( hBox1);
SetSizerAndFit(vBox1);
	}


void LEDLampsPage::ListDefinedLamps()
{
	// Clear the list box -necessary to prevent duplication due to the lack of data binding in wxListBox
	lbxLampNames->Clear();	

	for (unsigned int i = 0; i < pMyProtocol->GetDisplayNames().size(); i++)
	{
		if (pMyProtocol->GetDisplayItem( i).IsLEDLamp())
				{
					lbxLampNames->Append( pMyProtocol->GetDisplayNames().at( i));
				}
	}  // end for
	}


bool LEDLampsPage::IsValidWizardPage()
{
return ( true);
}


// Event handlers:

		void LEDLampsPage::OnAddDefinition( wxCommandEvent& event)
		{
DefineLEDLampDialog * myNewLampDlg = new DefineLEDLampDialog( wstrDefineLEDPropertiesTitle, pMyProtocol);

if (myNewLampDlg->ShowModal() == wxID_OK)
{
	if (myNewLampDlg->IsValidLEDLamp())
{
	std::wstring wstrLabel = myNewLampDlg->GetLEDLampName();
			// Check that the new label is unique
		std::wstring strTempHash = 			pMyProtocol->GetDisplayHash( wstrLabel);

		if (strTempHash.empty() == false)
{
	wxMessageBox( wstrDuplicateControlNameError
		, wstrErrorTitle, wxOK | wxICON_ERROR);
} // end if label not unique
else
{
	// Add a referenced definition entry to the map, then refresh the list box
																DisplayDefinition myDefinition;
// We're not implementing MIDI shift buttons yet, so this is a place-holder
	myDefinition.SetMIDIModifier( false);

std::wstring strHash = myNewLampDlg->GetDisplayHash();
														myDefinition.SetLabel( myNewLampDlg->GetLEDLampName());																												
														myDefinition.IsLEDLamp( true);
														myDefinition.SetLineCount( 0);
														myDefinition.SetDisplayLength( 0);
														myDefinition.SetStripCount( 0);
myDefinition.SetSysExAddressBytes( myNewLampDlg->GetSysExAddressBytes());																
																												pMyProtocol->SetDisplay( strHash, myDefinition);
				lbxLampNames->Append( wstrLabel); 
				lbxLampNames->SetStringSelection( myDefinition.GetLabel());			
		}  // end if unique label
}  // if ControlHash 0
}   // if dialog returns OK
		myNewLampDlg->Destroy();
		}


		void LEDLampsPage::OnEditDefinition( wxCommandEvent& event)
{
int nSelection = lbxLampNames->GetSelection();
						
			if (nSelection < 0)
	{
		return;
			}
			
			std::wstring wstrDisplayName = lbxLampNames->GetStringSelection().ToStdWstring();

			// Find the associated entry within the map
		std::wstring strOldHash = pMyProtocol->GetDisplayHash( wstrDisplayName);			
		
		if (strOldHash.empty())
		{
			return;
		}
		
		// Store the control hash and the definition
			DisplayDefinition myDisplay = pMyProtocol->GetDisplay( strOldHash);
std::wstring strNewHash;
std::vector <unsigned char> vSysExAddressBytes; 
			
// Edit the message definition
std::wstring wstrLEDLabel = lbxLampNames->GetStringSelection().ToStdWstring();
DefineMessageDialog * myDefinitionDialog = new DefineMessageDialog( wstrLampSetupTitle, wstrLEDLabel, pMyProtocol->GetSysExHeader(), strOldHash);
	
if (myDefinitionDialog->ShowModal() == wxID_OK)
{
strNewHash = myDefinitionDialog->GetMessageHash();
	vSysExAddressBytes = myDefinitionDialog->GetSysExAddressBytes();
	
	// Add the updated definition entry to the map, then refresh the list box
if (strOldHash.compare( strNewHash) == 0)
{
// Hash has not changed
return;
}
else
	{
		pMyProtocol->DeleteDisplay( strOldHash);
pMyProtocol->SetDisplay( strNewHash, myDisplay);
}

ListDefinedLamps();
// Set the selected item in the list back to the last edited item
lbxLampNames->SetStringSelection( wstrLEDLabel);
}  // dialog returns OK

// Tidy up
myDefinitionDialog->Destroy(); 
}

		
void LEDLampsPage::OnDeleteDefinition( wxCommandEvent& event)
		{
			int nSelection = lbxLampNames->GetSelection();
						
			if (nSelection > -1)
	{
		std::wstring wstrLampName = lbxLampNames->GetStringSelection().ToStdWstring();
std::wstring wstrMessage = wstrConfirmDeletion;
wstrMessage.append( wstrLampName);
wstrMessage.append( wstrQuestionMark);

if (		wxMessageBox( wstrMessage,                wstrAppTitle, wxYES_NO | wxNO_DEFAULT | wxICON_QUESTION) == wxYES) 
{
	// Try to remove the associated entry from the map
	std::wstring strHash = pMyProtocol->GetDisplayHash( wstrLampName);
			
	if (strHash .empty() == false)
		{
			pMyProtocol->DeleteDisplay( strHash);
			// Update the list box
			lbxLampNames->Delete( nSelection);
		}
}  // if deletion confirmed
			}   // end if valid selection in list box
}

		
		void LEDLampsPage::OnDefineLampStates( wxCommandEvent& event)
		{
			int nSelection;
				
			if (pMyProtocol->IsUsingGlobalStates())
			{
				if (lbxLampNames->GetCount() > 0)
				{	
// We use the first display to hold our global values
					DisplayDefinition myDisplay = pMyProtocol->GetDisplayItem( GLOBAL_STATE_INDEX);
std::wstring strHash = pMyProtocol->GetDisplayHash( myDisplay.GetLabel());
DisplayStatesDialog * myStatesDlg = new DisplayStatesDialog( wstrLEDStatesTitle, strHash, &myDisplay, pMyProtocol->GetSysExHeader());

		if (myStatesDlg->ShowModal() == wxID_OK)
		{
			pMyProtocol->SetDisplay( strHash, myDisplay);		
		}  // end if dialog returns OK
			
		// Tidy up
		myStatesDlg->Destroy();
}  // endif count > 0
		}
		else   // display-specific states
			{
				nSelection = lbxLampNames->GetSelection();
						
			if (nSelection > -1)
	{
		std::wstring wstrLampLabel = lbxLampNames->GetStringSelection().ToStdWstring();
std::wstring strHash = pMyProtocol->GetDisplayHash( wstrLampLabel);
DisplayDefinition myDisplay;
myDisplay = pMyProtocol->GetDisplay( strHash);

DisplayStatesDialog * myStatesDlg = new DisplayStatesDialog( wstrLEDStatesTitle, strHash, &myDisplay, pMyProtocol->GetSysExHeader());

		if (myStatesDlg->ShowModal() == wxID_OK)
		{
pMyProtocol->SetDisplay( strHash, myDisplay);		
				}  // end if dialog returns OK
			
		// Tidy up
		myStatesDlg->Destroy();
	}  // if valid list box selection
		}  // end if using global states
		}


		void LEDLampsPage::OnRenameLamp( wxCommandEvent& event)
		{
		int nSelection = lbxLampNames->GetSelection();
						
			if (nSelection > -1)
	{
		std::wstring wstrOldLabel = lbxLampNames->GetStringSelection().ToStdWstring();

// Get a new control label
			RBInputBox * UserLabelDlg = new RBInputBox( wstrAppTitle, wstrNewControlNamePrompt, wstrOldLabel);

			if (UserLabelDlg->ShowModal() == wxID_OK)
		{
			std::wstring wstrNewLabel = UserLabelDlg->GetText();
	
			if (wstrNewLabel.empty())
{
	wxMessageBox( wstrNoControlNameError, wstrErrorTitle, wxOK | wxICON_ERROR);
} // end if label not empty
else
			{
				// Update the map, then the list box 
				pMyProtocol->RenameDisplay( wstrOldLabel, wstrNewLabel);
lbxLampNames->SetString( nSelection, wstrNewLabel);
lbxLampNames->SetStringSelection( wstrNewLabel);			
			}   // end label not empty
				} // end if input box returns OK
else
{
	wxMessageBox( wstrNoMatchingLabelError, wstrErrorTitle, wxOK | wxICON_ERROR);
}

// Tidy up
			UserLabelDlg->Destroy();
			}   // end if valid selection in list box
}


			void LEDLampsPage::OnLEDSelect( wxCommandEvent& event)
{
	if (lbxLampNames->GetSelection() < 0)
	{
		btnStates->Disable();		
		btnRename->Disable();
		btnDelete->Disable();
}
	else
	{
		btnStates->Enable();
		btnRename->Enable();
	btnDelete->Enable();
	}
}

		
void LEDLampsPage::OnToggleGlobalStates(wxCommandEvent& event)
{
	pMyProtocol->SetGlobalStatesUsage( wxchkUseGlobalStates->GetValue());		
}


void LEDLampsPage::OnWizardCancel(wxWizardEvent& event)
    {
		std::wstring wstrMessage = wstrCancelProtocolCheck;
		
	if (		wxMessageBox( wstrMessage,                wstrAppTitle, wxYES_NO | wxNO_DEFAULT | wxICON_QUESTION) == wxNO) 
{
            event.Veto();
	}
		}

    
// We need to implement an event table in which the events received by a wxNewSurfaceDialog are routed to their respective handler functions 

	BEGIN_EVENT_TABLE(LEDLampsPage, wxWizardPageSimple)
// List box selection
EVT_LISTBOX( ID_LAMPS_LIST_BOX, LEDLampsPage::OnLEDSelect)

// List box manipulation buttons:
EVT_BUTTON( ID_ADD, LEDLampsPage::OnAddDefinition)
EVT_BUTTON( ID_EDIT, LEDLampsPage::OnEditDefinition)
EVT_BUTTON( ID_DELETE, LEDLampsPage::OnDeleteDefinition)
EVT_BUTTON( ID_STATES, LEDLampsPage::OnDefineLampStates)
EVT_BUTTON( ID_RENAME, LEDLampsPage::OnRenameLamp)

// Global states
EVT_CHECKBOX( ID_GlobalStates, LEDLampsPage::OnToggleGlobalStates)

// Wizard handling
EVT_WIZARD_CANCEL(wxID_ANY, LEDLampsPage::OnWizardCancel)
END_EVENT_TABLE()
