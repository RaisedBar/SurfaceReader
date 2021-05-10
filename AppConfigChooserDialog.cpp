// AppConfigChooserDialog.cpp
// Author:  T A Burgess
// Raised Bar Ltd.
// http://www.raisedbar.net

#define _HAS_STD_BYTE 0

#include "AppConfigChooserDialog.h"


AppConfigChooserDialog::AppConfigChooserDialog( const wxString& title, std::string strProtocolID, std::shared_ptr <AppCollection> pApps)
       : wxDialog(NULL, -1, title, wxDefaultPosition, wxSize(250, 230)),
	   pMyApps( new AppCollection())
{  
	wstrMyProtocolID.assign( strProtocolID.begin(), strProtocolID.end());
pMyApps = pApps;

	wxPanel * myPanel = new wxPanel(this, -1);
	
	wxBoxSizer * hBoxApps= new wxBoxSizer( wxHORIZONTAL);
lblDefinedAppsPrompt = new wxStaticText(myPanel, wxID_ANY, wstrDefinedAppsPrompt);
lbxApps = new wxListBox( myPanel, ID_Apps_LIST_BOX,  wxDefaultPosition, wxSize( 100, 200), 0, NULL, wxLB_SORT, wxDefaultValidator, wxT( ""));
ListDefinedApps();
hBoxApps->Add( lblDefinedAppsPrompt, 0, wxEXPAND);
hBoxApps->Add( lbxApps, 0, wxEXPAND);

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
this->Maximize();
}

AppConfigChooserDialog::~AppConfigChooserDialog()
{}

std::pair <std::wstring, std::wstring> AppConfigChooserDialog::GetAppConfigID()
{
if (lbxApps->GetSelection() < 0)
{
	return { wstrDefaultAppName, wstrMyProtocolID };
}
else
{
	return { lbxApps->GetStringSelection(), wstrMyProtocolID };
}
}

void AppConfigChooserDialog::ListDefinedApps()
{
	lbxApps->Clear();
lbxApps->Append( wstrDefaultAppName);

	for(auto appName : pMyApps->GetAppNames(wstrMyProtocolID))
	{
		lbxApps->Append(appName);
	}
}

// Event handlers:

void AppConfigChooserDialog::OnListSelect( wxCommandEvent& event)
{}


	void AppConfigChooserDialog::OnOK( wxCommandEvent& event)
		{
	EndModal( wxID_OK);
		}

		
		void AppConfigChooserDialog::OnCancel( wxCommandEvent& event)
		{
		EndModal( wxID_CANCEL);
		}

		
		// We need to implement an event table in which the events received by a wxNewSurfaceDialog are routed to their respective handler functions 

	BEGIN_EVENT_TABLE(AppConfigChooserDialog, wxDialog)
		// List box selection
		EVT_LISTBOX( ID_Apps_LIST_BOX, AppConfigChooserDialog::OnListSelect)

EVT_BUTTON( wxID_OK, AppConfigChooserDialog::OnOK)
	    EVT_BUTTON( wxID_CANCEL, AppConfigChooserDialog::OnCancel)
END_EVENT_TABLE()
