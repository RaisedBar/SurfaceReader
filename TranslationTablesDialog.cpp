// TranslationTablesDialog.cpp
// Author:  T A Burgess
// Raised Bar Ltd.
// http://www.raisedbar.net

#define _HAS_STD_BYTE 0

#include "TranslationTablesDialog.h"


TranslationTablesDialog::TranslationTablesDialog( const wxString& title, std::string strHash, std::shared_ptr<SurfaceProtocol> pProtocol)
       : wxDialog(NULL, -1, title, wxDefaultPosition, wxSize(250, 230)),
	   pMyProtocol( new SurfaceProtocol())
{  
strMyHash = strHash;
pMyProtocol = pProtocol;

	wxPanel * myPanel = new wxPanel(this, -1);
	
	// List for the Tables already defined
wxBoxSizer * vBoxTables = new wxBoxSizer( wxVERTICAL);
  lblDefinedTablesPrompt = new wxStaticText(myPanel, wxID_ANY, wstrDefinedTablesPrompt, wxDefaultPosition, wxSize( wxDefaultSize));
lbxTables = new wxListBox( myPanel, ID_Tables_LIST_BOX, wxDefaultPosition, wxSize( wxDefaultSize));
ListDefinedTables();
lbxTables->SetFocus();
	  vBoxTables ->Add( lblDefinedTablesPrompt, 0, wxEXPAND);  
	  	  vBoxTables ->Add( lbxTables, 0, wxEXPAND);  
  
// Dialog buttons
wxBoxSizer * hButtonSizer = new wxBoxSizer( wxHORIZONTAL);
AddTableButton = new wxButton(myPanel, ID_ADD, wstrAddTableButtonName, wxDefaultPosition, wxSize( wxDefaultSize));
EditTableButton = new wxButton(myPanel, ID_EDIT, wstrEditTable, wxDefaultPosition, wxSize( wxDefaultSize));
RenameTableButton = new wxButton(myPanel, ID_RENAME, wstrRenameTable, wxDefaultPosition, wxSize( wxDefaultSize));
DeleteTableButton= new wxButton(myPanel, ID_DELETE, wstrDeleteTableButtonName, wxDefaultPosition, wxSize( wxDefaultSize));
OKButton = new wxButton(myPanel, wxID_OK, wstrOKButtonName, wxDefaultPosition, wxSize( wxDefaultSize));
OKButton->SetDefault();
CancelButton = new wxButton(myPanel, wxID_CANCEL, wstrCancelButtonName, wxDefaultPosition, wxSize( wxDefaultSize));
hButtonSizer->Add( AddTableButton, 0, wxEXPAND);
hButtonSizer->Add( DeleteTableButton, 0, wxEXPAND);
hButtonSizer->Add( OKButton, 0, wxEXPAND);
hButtonSizer->Add( CancelButton, 0, wxEXPAND);

wxBoxSizer * vBox1 = new wxBoxSizer( wxVERTICAL);
vBox1->Add( vBoxTables);	
vBox1->Add( hButtonSizer);
	
myPanel->SetSizer( vBox1);
vBox1->SetSizeHints( this );
myPanel->Fit();
vBox1->Fit( myPanel);
Centre();
this->Maximize();
}


TranslationTablesDialog::~TranslationTablesDialog()
{}


bool TranslationTablesDialog::IsValidTableID()
{
	return (lbxTables->GetSelection() > -1);
}


std::string TranslationTablesDialog::GetTableID()
{
	return pMyProtocol->GetTranslationTableID( lbxTables->GetStringSelection().ToStdWstring());
}


void TranslationTablesDialog::ListDefinedTables()
{
	lbxTables->Clear();
	
	for (int i = 0; i < pMyProtocol->GetTranslationTableCount(); i++)
	{
		lbxTables->Append( pMyProtocol->GetTableLabels().at( i));
	}  // end for
}


void TranslationTablesDialog::CheckSelection()
{
int nSelection = lbxTables->GetSelection();
	
if (nSelection < 0)
{
	DeleteTableButton->Disable();
		EditTableButton->Disable();
}
else
{
	DeleteTableButton->Enable();
		EditTableButton->Enable();
}
}


// Event handlers:


void TranslationTablesDialog::OnAddTable( wxCommandEvent& event)
{
RBInputBox * TableNameDlg = new RBInputBox( wstrAppTitle, wstrNewTableNamePrompt);

			if (TableNameDlg->ShowModal() == wxID_OK)
		{
			std::wstring wstrTableName = TableNameDlg->GetText();
	
			if (wstrTableName.empty() == false)
{
DefineTranslationTable * myTableDlg = new DefineTranslationTable( wstrTablesTitle, pMyProtocol);

	if (myTableDlg->ShowModal() == wxID_OK) 
		{
			if (myTableDlg->IsValidTableDefinition())
	{
		std::string strTableID = myTableDlg->GetTableID();
		std::pair <std::wstring, std::map <unsigned char, std::wstring> > myPair;
		myPair = std::make_pair( wstrTableName, myTableDlg->GetCharacterMap());
pMyProtocol->SetTranslationTable( strTableID, myPair);
		// Refresh list
	ListDefinedTables();
	lbxTables->SetFocus();
			}  // end if valid Table definition
	}  // end if character dialog returns OK
			}  // end if empty table name
			}  // end if input box OK
}


void TranslationTablesDialog::OnDeleteTable( wxCommandEvent& event)
{
	int nSelection = lbxTables->GetSelection();

	if (nSelection > -1)
	{
		if (		wxMessageBox( wstrDeleteTableCheck,                wstrAppTitle, wxYES_NO | wxNO_DEFAULT | wxICON_QUESTION) == wxYES) 
		{
			std::string strHash = pMyProtocol->GetTranslationTableID( lbxTables->GetStringSelection().ToStdWstring());
		if (pMyProtocol->DeleteTranslationTable( strHash))
		{
	// Refresh list
	ListDefinedTables();
		lbxTables->SetFocus();
			}  // end if Successful deletion
		}  // end if confirmed
	}  // end if Valid selection in list box
}


void TranslationTablesDialog::OnEditTable( wxCommandEvent& event)
{
	int nSelection = lbxTables->GetSelection();

	if (nSelection > -1)
	{
		std::string strTableID= pMyProtocol->GetTranslationTableID( lbxTables->GetStringSelection().ToStdWstring());
	DefineTranslationTable * myTableDlg = new DefineTranslationTable( wstrTablesTitle, strTableID, pMyProtocol);

	if (myTableDlg->ShowModal() == wxID_OK) 
		{
			if (myTableDlg->IsValidTableDefinition())
	{
				std::pair <std::wstring, std::map <unsigned char, std::wstring> > myPair;
				myPair = std::make_pair( pMyProtocol->GetTranslationTable( strTableID).first, myTableDlg->GetCharacterMap());
		
		pMyProtocol->SetTranslationTable( strTableID, myPair);
		// Refresh list
	ListDefinedTables();
	lbxTables->SetFocus();
			}  // end if valid Table
	}  // end if OK
	}  // end if valid selection in list
}


void TranslationTablesDialog::OnRenameTable( wxCommandEvent& event)
{
	int nSelection = lbxTables->GetSelection();

	if (nSelection > -1)
	{
		RBInputBox * TableNameDlg = new RBInputBox( wstrAppTitle, wstrNewTableNamePrompt, lbxTables->GetStringSelection());

			if (TableNameDlg->ShowModal() == wxID_OK)
		{
			std::wstring wstrTableName = TableNameDlg->GetText();
	
			if (wstrTableName.empty() == false)
{
	std::string strTableID= pMyProtocol->GetTranslationTableID( lbxTables->GetStringSelection().ToStdWstring());
pMyProtocol->SetTranslationTableName( strTableID, wstrTableName);

		// Refresh list
	ListDefinedTables();
	lbxTables->SetFocus();
			}  // end if valid name
	}  // end if OK
	}  // end if valid selection in list
}


void TranslationTablesDialog::OnOK( wxCommandEvent& event)
		{
	EndModal( wxID_OK);
		}

		
		void TranslationTablesDialog::OnCancel( wxCommandEvent& event)
		{
		EndModal( wxID_CANCEL);
		}

		
		void TranslationTablesDialog::OnListSelect(wxCommandEvent& event)
{
CheckSelection();
		}


		// We need to implement an event table in which the events received by a wxNewSurfaceDialog are routed to their respective handler functions 

	BEGIN_EVENT_TABLE(TranslationTablesDialog, wxDialog)
		// List box selection
EVT_LISTBOX( ID_Tables_LIST_BOX, TranslationTablesDialog::OnListSelect)

EVT_BUTTON( ID_ADD, TranslationTablesDialog::OnAddTable)
EVT_BUTTON( ID_RENAME, TranslationTablesDialog::OnRenameTable)	    
EVT_BUTTON( ID_EDIT, TranslationTablesDialog::OnEditTable)	    
EVT_BUTTON( ID_DELETE, TranslationTablesDialog::OnDeleteTable)
EVT_BUTTON( wxID_OK, TranslationTablesDialog::OnOK)
EVT_BUTTON( wxID_CANCEL, TranslationTablesDialog::OnCancel)
END_EVENT_TABLE()
