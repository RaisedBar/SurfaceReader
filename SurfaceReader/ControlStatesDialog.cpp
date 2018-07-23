// ControlStatesDialog.cpp
// Author:  T A Burgess
// Raised Bar Ltd.
// http://www.raisedbar.net


#include "ControlStatesDialog.h"


ControlStatesDialog::ControlStatesDialog( const wxString& title, std::string strHash, MessageDefinition * pMessage, std::vector <unsigned char> vSysExHeader)
       : wxDialog(NULL, -1, title, wxDefaultPosition, wxSize(250, 230))
{  
strMyHash = strHash;
pMyMessage = pMessage;
vMySysExHeader = vSysExHeader;

	wxPanel * myPanel = new wxPanel(this, -1);
	
	// List for the states already defined
wxBoxSizer * vBoxStates = new wxBoxSizer( wxVERTICAL);
  lblDefinedStatesPrompt = new wxStaticText(this, wxID_ANY, wstrDefinedStatesPrompt, wxDefaultPosition, wxSize( wxDefaultSize));
lbxStates = new wxListBox( this, ID_STATES_LIST_BOX, wxDefaultPosition, wxSize( wxDefaultSize));
ListDefinedStates();
lbxStates->SetFocus();
	  vBoxStates ->Add( lblDefinedStatesPrompt, 0, wxEXPAND);  
	  	  vBoxStates ->Add( lbxStates, 0, wxEXPAND);  
  
// Dialog buttons
wxBoxSizer * hButtonSizer = new wxBoxSizer( wxHORIZONTAL);
AddStateButton = new wxButton(myPanel, ID_ADD, wstrAddStateButtonName, wxDefaultPosition, wxSize( wxDefaultSize));
EditStateButton = new wxButton(myPanel, ID_EDIT, wstrEditState, wxDefaultPosition, wxSize( wxDefaultSize));
DeleteStateButton= new wxButton(myPanel, ID_DELETE, wstrDeleteStateButtonName, wxDefaultPosition, wxSize( wxDefaultSize));
OKButton = new wxButton(myPanel, wxID_OK, wstrOKButtonName, wxDefaultPosition, wxSize( wxDefaultSize));
OKButton->SetDefault();
CancelButton = new wxButton(myPanel, wxID_CANCEL, wstrCancelButtonName, wxDefaultPosition, wxSize( wxDefaultSize));
hButtonSizer->Add( AddStateButton, 0, wxEXPAND);
hButtonSizer->Add( DeleteStateButton, 0, wxEXPAND);
hButtonSizer->Add( OKButton, 0, wxEXPAND);
hButtonSizer->Add( CancelButton, 0, wxEXPAND);

wxBoxSizer * vBox1 = new wxBoxSizer( wxVERTICAL);
vBox1->Add( vBoxStates);	
vBox1->Add( hButtonSizer);
	
myPanel->SetSizer( vBox1);
vBox1->SetSizeHints( this );
myPanel->Fit();
vBox1->Fit( myPanel);
Centre();
this->Maximize();
}


ControlStatesDialog::~ControlStatesDialog()
{}


void ControlStatesDialog::ListDefinedStates()
{
	lbxStates->Clear();
	
	for (int i = 0; i < pMyMessage->GetStateCount(); i++)
	{
		lbxStates->Append( pMyMessage->GetStateLabels().at( i));
	}  // end for
}


void ControlStatesDialog::CheckSelection()
{
int nSelection = lbxStates->GetSelection();
	
if (nSelection < 0)
{
	DeleteStateButton->Disable();
		EditStateButton->Disable();
}
else
{
	DeleteStateButton->Enable();
		EditStateButton->Enable();
}
}


// Event handlers:


void ControlStatesDialog::OnAddState( wxCommandEvent& event)
{
	DefineStateDialog * myStateDlg = new DefineStateDialog( wstrControlStatesTitle, strMyHash, vMySysExHeader);

	if (myStateDlg->ShowModal() == wxID_OK) 
		{
			if (myStateDlg->IsValidStateDefinition())
	{
		pMyMessage->SetState( myStateDlg->GetStateValue(), myStateDlg->GetStateName());
		// Refresh list
	ListDefinedStates();
	lbxStates->SetFocus();
			}  // end if valid state definition
	}  // end if dialog returns OK
		}


void ControlStatesDialog::OnDeleteState( wxCommandEvent& event)
{
	int nSelection = lbxStates->GetSelection();

	if (nSelection > -1)
	{
		if (		wxMessageBox( wstrDeleteStateCheck,                wstrAppTitle, wxYES_NO | wxNO_DEFAULT | wxICON_QUESTION) == wxYES) 
		{
		std::vector <unsigned char> vKeyBytes = pMyMessage->GetStateBytes( nSelection);
		if (pMyMessage->DeleteState( vKeyBytes))
		{
	// Refresh list
	ListDefinedStates();
		lbxStates->SetFocus();
			}  // end if Successful deletion
		}  // end if confirmed
	}  // end if Valid selection in list box
}


void ControlStatesDialog::OnEditState( wxCommandEvent& event)
{
	int nSelection = lbxStates->GetSelection();

	if (nSelection > -1)
	{
				std::vector <unsigned char> vKeyBytes = pMyMessage->GetStateBytes( nSelection);
				DefineStateDialog * myStateDlg = new DefineStateDialog( wstrControlStatesTitle, strMyHash, vMySysExHeader, lbxStates->GetStringSelection(), vKeyBytes);

	if (myStateDlg->ShowModal() == wxID_OK) 
		{
			if (myStateDlg->IsValidStateDefinition())
	{
		pMyMessage->SetState( myStateDlg->GetStateValue(), myStateDlg->GetStateName());
		// Refresh list
	ListDefinedStates();
	lbxStates->SetFocus();
			}  // end if valid state
	}  // end if OK
	}  // end if valid selection in list
}


void ControlStatesDialog::OnOK( wxCommandEvent& event)
		{
	EndModal( wxID_OK);
		}

		
		void ControlStatesDialog::OnCancel( wxCommandEvent& event)
		{
		EndModal( wxID_CANCEL);
		}

		
		void ControlStatesDialog::OnListSelect(wxCommandEvent& event)
{
CheckSelection();
		}


		// We need to implement an event table in which the events received by a wxNewSurfaceDialog are routed to their respective handler functions 

	BEGIN_EVENT_TABLE(ControlStatesDialog, wxDialog)
		// List box selection
EVT_LISTBOX( ID_STATES_LIST_BOX, ControlStatesDialog::OnListSelect)

EVT_BUTTON( ID_ADD, ControlStatesDialog::OnAddState)
EVT_BUTTON( ID_EDIT, ControlStatesDialog::OnEditState)	    
EVT_BUTTON( ID_DELETE, ControlStatesDialog::OnDeleteState)
EVT_BUTTON( wxID_OK, ControlStatesDialog::OnOK)
EVT_BUTTON( wxID_CANCEL, ControlStatesDialog::OnCancel)
END_EVENT_TABLE()
