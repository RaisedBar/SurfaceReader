// HardwarePage.cpp
// Author:  T A Burgess
// Raised Bar Ltd.
// http://www.raisedbar.net

#include "HardwarePage.h"


HardwareControlsPage::HardwareControlsPage( wxWizard * wizParent, const wxString & title, boost::shared_ptr<SurfaceProtocol> pProtocol, boost::shared_ptr<SurfaceParameters> pParameters)
       : wxWizardPageSimple( wizParent),
	   pMyProtocol( new SurfaceProtocol()),
	   pMyParameters( new SurfaceParameters())
{  
pMyProtocol = pProtocol;
pMyParameters = pParameters;
	
SetLabel( title);
myHardwareIn = 0;
blnIsOpen = false;

try 
	{
myHardwareIn = new RtMidiIn;
} 
	catch ( RtMidiError &error ) 
	{
std::string strError = error.getMessage();    
std::wstring wstrError( strError.begin(), strError.end());
	wxMessageBox( wstrError,                wstrAppTitle, wxOK | wxICON_ERROR);
  }

wxBoxSizer * vBoxControls = new wxBoxSizer( wxVERTICAL);
  wxStaticText * lblDefinedControlPrompt = new wxStaticText( this, wxID_ANY, wstrDefinedControlsPrompt, wxDefaultPosition, wxSize( wxDefaultSize));
// List for the controls already defined
const wxString wxstrChoices[ 1];
lbxControlNames = new wxListBox( this, ID_DEFINED_CONTROLS_LIST_BOX, wxDefaultPosition, wxSize( wxDefaultSize), 0, wxstrChoices, wxLB_SORT);
lbxControlNames->SetWindowStyle( wxLB_SORT); 
ListDefinedControls();
lbxControlNames->SetFocus();
	  vBoxControls ->Add( lblDefinedControlPrompt, 0, wxEXPAND);  
	  	  vBoxControls ->Add( lbxControlNames, 0, wxEXPAND);  
  
		  		  wxBoxSizer * vBoxListButtons = new wxBoxSizer( wxVERTICAL);

				  // Buttons to manipulate the list box contents
		  btnAdd = new wxButton(this, ID_ADD, wstrAddControlName, wxDefaultPosition, wxSize( wxDefaultSize));
btnLearn = new wxButton(this, ID_LEARN, wstrLearnControlName, wxDefaultPosition, wxSize( wxDefaultSize));
		  btnEdit = new wxButton(this, ID_EDIT, wstrEditControlName, wxDefaultPosition, wxSize( wxDefaultSize));
		  btnRename = new wxButton(this, ID_RENAME, wstrRenameControl, wxDefaultPosition, wxSize( wxDefaultSize));
btnDelete = new wxButton(this, ID_DELETE, wstrDeleteControlName, wxDefaultPosition, wxSize( wxDefaultSize));
btnMatchLEDs = new wxButton(this, ID_MATCH_LED, wstrMatchLEDEntries, wxDefaultPosition, wxSize( wxDefaultSize));
// Check box for message buffering
wxchkDropStatusBytes = new wxCheckBox( this, ID_DROP_STATUS_BYTES, wstrDropStatusBytesPrompt);
wxchkDropStatusBytes->SetValue( pMyProtocol->GetDropExtraStatusBytes());
		  
// Enable/disable appropriately
if (lbxControlNames->GetCount() == 0)
{
		btnRename->Disable();
	btnDelete->Disable();
	btnMatchLEDs->Disable();	
}

// Add the buttons and check box to a sizer
vBoxListButtons->Add( btnAdd, 0, wxEXPAND);
	vBoxListButtons->Add( btnLearn, 0, wxEXPAND);
vBoxListButtons->Add( btnEdit, 0, wxEXPAND);
vBoxListButtons->Add( btnRename, 0, wxEXPAND);
vBoxListButtons->Add( btnDelete, 0, wxEXPAND);
vBoxListButtons->Add( btnMatchLEDs, 0, wxEXPAND);
vBoxListButtons->Add( wxchkDropStatusBytes, 0, wxEXPAND); 
	
	wxBoxSizer * hBox1 = new wxBoxSizer( wxHORIZONTAL);
	hBox1->Add( vBoxControls);
	hBox1->Add( vBoxListButtons);

	wxBoxSizer * vBox1 = new wxBoxSizer( wxVERTICAL);
	vBox1->Add( hBox1);
SetSizerAndFit(vBox1);
	}


void HardwareControlsPage::SetMIDIInputID( int nInputID)
{
CloseInput();
	blnIsOpen = true;

// Open the hardware input outside of our threading
try 
	{
myHardwareIn->openPort( nInputID);
// Don't ignore sysex, timing, or active sensing messages.
myHardwareIn->ignoreTypes( false, false, false );
blnIsOpen = true;
} 
	catch ( RtMidiError &error ) 
	{
std::string strError = error.getMessage();
std::wstring wstrError( strError.begin(), strError.end());
	wxMessageBox( wstrError,                wstrAppTitle, wxOK | wxICON_ERROR);
  blnIsOpen = false;
	return;
	}
	
	if (blnIsOpen == false)
	{
wxMessageBox( wstrMIDILearnInPortError, wstrErrorTitle, wxOK | wxICON_ERROR);
}
}


void HardwareControlsPage::ListDefinedControls()
{
// Clear the list box -necessary to prevent duplication due to the lack of data binding in wxListBox
	lbxControlNames->Clear();	

	for (unsigned int i = 0; i < pMyProtocol->GetHardwareControlNames().size(); i++)
	{
		lbxControlNames->Append( pMyProtocol->GetHardwareControlNames().at( i));
				}  // end for
	
// Sort
}


bool HardwareControlsPage::IsValidWizardPage()
{
return ( true);
}


bool HardwareControlsPage::TransferDataFromWindow()
{
bool blnResult = IsValidWizardPage();
return blnResult;
}


std::string HardwareControlsPage::MIDILearn()
{
std::string strHash;
strHash.clear();

		if (blnIsOpen == false)
{
wxMessageBox( strHardwareInError,                wstrAppTitle, wxOK | wxICON_ERROR);
return strHash;
		}

bool blnDone = false;

for (unsigned int i = 0; blnDone == false; i++)
{
MIDIEvents.clear();
double dStamp = myHardwareIn->getMessage( &MIDIEvents);

// Test to see if any MIDI messages were received, quit if not
if (MIDIEvents.size() > 0)
{
MIDI myMIDI( MIDIEvents, false);
strHash= myMIDI.MIDIHash();
}
else // No MIDI available
{
if (i == 0)
	{
		// No MIDI was available on our first attempt
		wxMessageBox( wstrNoMIDIMessageError, wstrErrorTitle, wxOK | wxICON_ERROR);
}

blnDone = true;
}  // end if no more MIDI
}  // end for

return strHash;
}


void HardwareControlsPage::CloseInput()
{
if (blnIsOpen)
{
		try
{
myHardwareIn->closePort();
// delete myHardwareIn;
}
catch ( RtMidiError &error ) 
	{
std::string strError = error.getMessage();
std::wstring wstrError( strError.begin(), strError.end());
	wxMessageBox( wstrError,                wstrAppTitle, wxOK | wxICON_ERROR);
  }
}  // end if open
}


// Event handlers:

void HardwareControlsPage::OnAddDefinition( wxCommandEvent& event)
{
	DefineMessageDialog * myDefinitionDialog = new DefineMessageDialog( wstrHardwareSetupTitle, lbxControlNames->GetStringSelection(), pMyProtocol->GetSysExHeader());
std::string strControlHash;
strControlHash.clear();
std::vector <unsigned char> vSysExAddressBytes;

if (myDefinitionDialog->ShowModal() == wxID_OK)
{
	strControlHash = myDefinitionDialog->GetMessageHash();
	vSysExAddressBytes = myDefinitionDialog->GetSysExAddressBytes();
}

RBInputBox * UserLabelDlg = new RBInputBox( wstrAppTitle, wstrNewControlNamePrompt);
		
		bool blnDone = false;
		
		while (blnDone == false)
			{
				if (UserLabelDlg->ShowModal() == wxID_CANCEL)
		{
blnDone = true;
				}
				else // Dialog returned OK
				{
			std::wstring wstrLabel = UserLabelDlg->GetText();
	
			if (wstrLabel.empty())
{
	wxMessageBox( wstrNoControlNameError, wstrErrorTitle, wxOK | wxICON_ERROR);
			}
			else
			{
	// Check that the new label is unique
				std::string strTempHash = 			pMyProtocol->GetHardwareControlHash( wstrLabel);
bool blnUniqueLabel = false;

if (strTempHash.empty() == false)
{
	if (		wxMessageBox( wstrDuplicateControlNameError,                wstrErrorTitle, wxYES_NO | wxNO_DEFAULT | wxICON_QUESTION) == wxNO) 
	{
		UserLabelDlg->Destroy();
		return;
	}
				} // end if label not unique
else  // Unique label, so add an entry
{
blnDone = true;
				}  // end while
				
				// Add a referenced definition entry to the map, then refresh the list box
				MessageDefinition myDefinition( wstrLabel);
// We're not implementing MIDI shift buttons yet, so this is a place-holder
	myDefinition.SetMIDIModifier( false);

if (pMyProtocol->SetHardwareControl( strControlHash, myDefinition))
				{
					lbxControlNames->Append( wstrLabel); 
ListDefinedControls();
}  // Protocol updated
			}  // end if unique label
			}  // end if Input box provides a non-blank label
		}   // InputBox returns OK
		
		// Tidy up
		UserLabelDlg->Destroy(); 
myDefinitionDialog->Destroy(); 
}

	
	void HardwareControlsPage::OnLearnDefinition( wxCommandEvent& event)
		{
std::string strControlHash = ( MIDILearn());

if (strControlHash.empty())
{
return;
}

		RBInputBox * UserLabelDlg = new RBInputBox( wstrAppTitle, wstrNewControlNamePrompt);
		
		bool blnDone = false;
		
		while (blnDone == false)
			{
				if (UserLabelDlg->ShowModal() == wxID_CANCEL)
		{
blnDone = true;
				}
				else // Dialog returned OK
				{
			std::wstring wstrLabel = UserLabelDlg->GetText();
	
			if (wstrLabel.empty())
{
	wxMessageBox( wstrNoControlNameError, wstrErrorTitle, wxOK | wxICON_ERROR);
			}
			else
			{
	// Check that the new label is unique
				std::string strTempHash = 			pMyProtocol->GetHardwareControlHash( wstrLabel);
bool blnUniqueLabel = false;

if (strTempHash.empty() == false)
{
	if (		wxMessageBox( wstrDuplicateControlNameError,                wstrErrorTitle, wxYES_NO | wxNO_DEFAULT | wxICON_QUESTION) == wxNO) 
	{
		UserLabelDlg->Destroy();
		return;
	}
				} // end if label not unique
else  // Unique label, so add an entry
{
blnDone = true;
				}  // end while
				
				// Add a referenced definition entry to the map, then refresh the list box
				MessageDefinition myDefinition( wstrLabel);
// We're not implementing MIDI shift buttons yet, so this is a place-holder
	myDefinition.SetMIDIModifier( false);

if (pMyProtocol->SetHardwareControl( strControlHash, myDefinition))
				{
lbxControlNames->Append( wstrLabel); 
ListDefinedControls();
}  // Protocol updated
			}  // end if unique label
			}  // end if Input box provides a non-blank label
		}   // InputBox returns OK
		
		// Tidy up
		UserLabelDlg->Destroy(); 
}


void HardwareControlsPage::OnEditDefinition( wxCommandEvent& event)
		{
			int nSelection = lbxControlNames->GetSelection();
						
			if (nSelection < 0)
	{
		return;
			}
			
			std::wstring wstrControlName = lbxControlNames->GetStringSelection().ToStdWstring();

			// Find the associated entry within the map
			std::string strOldHash = pMyProtocol->GetHardwareControlHash( wstrDisplayName);			
		
		if (strOldHash.empty())
		{
			return;
		}
		
		// Store the control hash and the definition
MessageDefinition myMessage= pMyProtocol->GetDisplay( strOldHash);
std::string strNewHash;
std::vector <unsigned char> vSysExAddressBytes; 
			
// Edit the message definition
DefineMessageDialog * myDefinitionDialog = new DefineMessageDialog( wstrHardwareSetupTitle, wstrControlName, pMyProtocol->GetSysExHeader(), strOldHash);
	
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
		pMyProtocol->DeleteHardwareControl( strOldHash);
		pMyProtocol->SetHardwareControl( strNewHash, myMessage);
}

ListDefinedControls();
// Set the selected item in the list back to the last edited item
lbxControlNames->SetStringSelection( wstrControlName);
}  // dialog returns OK

// Tidy up
myDefinitionDialog->Destroy(); 
}

		
	void HardwareControlsPage::OnDeleteDefinition( wxCommandEvent& event)
		{
			int nSelection = lbxControlNames->GetSelection();
						
			if (nSelection > -1)
	{
		std::wstring wstrControlName = lbxControlNames->GetStringSelection().ToStdWstring();
std::wstring wstrMessage = wstrConfirmDeletion;
wstrMessage.append( wstrControlName);
wstrMessage.append( wstrQuestionMark);

if (		wxMessageBox( wstrMessage,                wstrAppTitle, wxYES_NO | wxNO_DEFAULT | wxICON_QUESTION) == wxYES) 
{
// Try to remove the associated entry from the map
	std::string strHash = pMyProtocol->GetHardwareControlHash( wstrControlName);
			
	if (strHash != "")
		{
			pMyProtocol->DeleteHardwareControl( strHash);
			// Update the list box
			ListDefinedControls();
		}
}  // if deletion confirmed
			}   // end if valid selection in list box
}

		
		void HardwareControlsPage::OnRenameControl( wxCommandEvent& event)
		{
		int nSelection = lbxControlNames->GetSelection();
						
			if (nSelection > -1)
	{
std::wstring wstrControlName = lbxControlNames->GetStringSelection().ToStdWstring();

// Try to update the associated entry within the map
std::wstring wstrOldLabel = lbxControlNames->GetStringSelection().ToStdWstring();

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
				pMyProtocol->RenameHardwareControl( wstrOldLabel, wstrNewLabel);
ListDefinedControls();
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


		void HardwareControlsPage::OnMatchLEDs( wxCommandEvent& event)
		{
			for (unsigned int i = 0; i < lbxControlNames->GetCount(); i++)
{
	std::wstring wstrLabel = lbxControlNames->GetString( i).ToStdWstring();
			
	// Check that the new label does not already have a matching LED
		std::string strDisplayHash = 			pMyProtocol->GetDisplayHash( wstrLabel);

		if (strDisplayHash.empty())
{
		// Get the control message we're cloning
	std::string strControlHash = pMyProtocol->GetHardwareControlHash( wstrLabel);

	// Add a referenced definition entry to the map
																DisplayDefinition myLEDDefinition;
// We're not implementing MIDI shift buttons yet, so this is a place-holder
	myLEDDefinition.SetMIDIModifier( false);

	myLEDDefinition.SetLabel( wstrLabel);
														myLEDDefinition.IsLEDLamp( true);
														myLEDDefinition.SetLineCount( 0);
														myLEDDefinition.SetDisplayLength( 0);
														myLEDDefinition.SetStripCount( 0);
														myLEDDefinition.SetSysExAddressBytes( pMyProtocol->GetHardwareControl( strControlHash).GetSysExAddressBytes());
	pMyProtocol->SetDisplay( strControlHash, myLEDDefinition);
				}  // end if unique label
}  // end for
		}



    // wizard event handlers
    
		void HardwareControlsPage::OnWizardCancel(wxWizardEvent& event)
    {
		std::wstring wstrMessage = wstrCancelProtocolCheck;
		
	if (		wxMessageBox( wstrMessage,                wstrAppTitle, wxYES_NO | wxNO_DEFAULT | wxICON_QUESTION) == wxYES) 
{
CloseInput();
	}
	else
{
            event.Veto();
	}
		}

    
		void HardwareControlsPage::OnWizardPageChanging(wxWizardEvent& event)
    {
pMyProtocol->SetMessageBufferSize();		
				CloseInput();
		}


	void HardwareControlsPage::OnWizardPageChanged(wxWizardEvent& event)
    {
		pMyProtocol->SetMessageBufferSize();		
		SetMIDIInputID( pMyParameters->GetHardwareInID());
  }


	void HardwareControlsPage::OnDropStatusBytes( wxCommandEvent& event)
	{
		pMyProtocol->SetDropExtraStatusBytes( wxchkDropStatusBytes->GetValue());	
	}


	// We need to implement an event table in which the events received by a wxNewSurfaceDialog are routed to their respective handler functions 

	BEGIN_EVENT_TABLE(HardwareControlsPage, wxWizardPageSimple)
// Wizard page monitoring
EVT_WIZARD_PAGE_CHANGED(wxID_ANY, HardwareControlsPage::OnWizardPageChanged)
EVT_WIZARD_PAGE_CHANGING(wxID_ANY, HardwareControlsPage::OnWizardPageChanging)
    EVT_WIZARD_CANCEL(wxID_ANY, HardwareControlsPage::OnWizardCancel)

// List box manipulation buttons:
EVT_BUTTON( ID_ADD, HardwareControlsPage::OnAddDefinition)
EVT_BUTTON( ID_LEARN, HardwareControlsPage::OnLearnDefinition)
EVT_BUTTON( ID_EDIT, HardwareControlsPage::OnEditDefinition)
EVT_BUTTON( ID_RENAME, HardwareControlsPage::OnRenameControl)
EVT_BUTTON( ID_DELETE, HardwareControlsPage::OnDeleteDefinition)
// EVT_BUTTON( ID_STATES, HardwareControlsPage::OnDefineControlStates)
EVT_BUTTON( ID_MATCH_LED, HardwareControlsPage::OnMatchLEDs)

// Message buffering control
EVT_CHECKBOX( ID_DROP_STATUS_BYTES, HardwareControlsPage::OnDropStatusBytes)
END_EVENT_TABLE()
