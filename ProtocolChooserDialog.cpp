// ProtocolChooserDialog.cpp
// Author:  T A Burgess
// Raised Bar Ltd.
// http://www.raisedbar.net

#define _HAS_STD_BYTE 0

#include "ProtocolChooserDialog.h"


ProtocolChooserDialog::ProtocolChooserDialog( const wxString& title, std::string strProtocolID, std::shared_ptr <ProtocolCollection> pProtocols)
       : wxDialog(NULL, -1, title, wxDefaultPosition, wxSize(250, 230)),
	   pMyProtocols( new ProtocolCollection())
{  
	pMyProtocols = pProtocols;

	wxPanel * myPanel = new wxPanel(this, -1);
	wxBoxSizer * hBoxApps= new wxBoxSizer( wxHORIZONTAL);
lblProtocolsPrompt = new wxStaticText(myPanel, wxID_ANY, wstrProtocolsPrompt);
lbxProtocols = new wxListBox( myPanel, ID_PROTOCOL_LIST_BOX,  wxDefaultPosition, wxSize( 100, 200), 0, NULL, wxLB_SORT, wxDefaultValidator, wxT( ""));
ListProtocols( strProtocolID);
hBoxApps->Add( lblProtocolsPrompt, 0, wxEXPAND);
hBoxApps->Add( lbxProtocols, 0, wxEXPAND);

// Dialog buttons
wxBoxSizer * hBoxButtons= new wxBoxSizer( wxHORIZONTAL);
wxButton * OKButton = new wxButton(myPanel, wxID_OK, wstrOKButtonName, wxDefaultPosition, wxSize( wxDefaultSize));
wxButton * CancelButton = new wxButton(myPanel, wxID_CANCEL, wstrCancelButtonName, wxDefaultPosition, wxSize( wxDefaultSize));
OKButton->SetDefault(); 
hBoxButtons->Add( OKButton, 0, wxEXPAND);
hBoxButtons->Add( CancelButton, 0, wxEXPAND);

wxBoxSizer * vBox1 = new wxBoxSizer( wxVERTICAL);
vBox1->Add( hBoxApps);
vBox1->Add( hBoxButtons);	

myPanel->SetSizer( vBox1);
vBox1->SetSizeHints( this );
myPanel->Fit();
vBox1->Fit( myPanel);
Centre();
}


ProtocolChooserDialog::~ProtocolChooserDialog()
{}


std::string ProtocolChooserDialog::GetProtocolID()
{
std::wstring wstrProtocolName;
std::string strNewProtocolID;

if (lbxProtocols->GetSelection() < 0)
{
	return strNewProtocolID;
}
else
{
	wstrProtocolName = lbxProtocols->GetStringSelection().ToStdWstring();
	return pMyProtocols->FindIDForProtocolName( wstrProtocolName);
}
}


void ProtocolChooserDialog::ListProtocols( std::string strMyProtocolID)
{
std::map <std::string, SurfaceProtocol>::iterator it;
lbxProtocols->Clear();

for (it = pMyProtocols->begin(); it != pMyProtocols->end(); it++)
	{
		lbxProtocols->Append( it->second.GetProtocolName());
	}  // end for


// Highlight the currently-active protocol
std::wstring wstrActiveProtocol = pMyProtocols->GetProtocol( strMyProtocolID).GetProtocolName();
	lbxProtocols->SetStringSelection( wstrActiveProtocol);
}


// Event handlers:

void ProtocolChooserDialog::OnListSelect( wxCommandEvent& event)
{}


	void ProtocolChooserDialog::OnOK( wxCommandEvent& event)
		{
	EndModal( wxID_OK);
		}

		
		void ProtocolChooserDialog::OnCancel( wxCommandEvent& event)
		{
		EndModal( wxID_CANCEL);
		}

		
		// We need to implement an event table in which the events received by a wxNewSurfaceDialog are routed to their respective handler functions 

	BEGIN_EVENT_TABLE(ProtocolChooserDialog, wxDialog)
		// List box selection
		EVT_LISTBOX( ID_Apps_LIST_BOX, ProtocolChooserDialog::OnListSelect)

EVT_BUTTON( wxID_OK, ProtocolChooserDialog::OnOK)
	    EVT_BUTTON( wxID_CANCEL, ProtocolChooserDialog::OnCancel)
END_EVENT_TABLE()
