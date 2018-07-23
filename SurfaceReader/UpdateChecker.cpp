// UpdateChecker.cpp
// Implementation of the UpdateChecker class 
// Author:  T A Burgess
// Raised Bar Ltd.
// http://www.raisedbar.net

#include "UpdateChecker.h"


UpdateChecker::UpdateChecker(const wxString & title)
       : wxDialog(NULL, -1, title, wxDefaultPosition, wxSize(250, 230))
{
      wxPanel *myPanel = new wxPanel(this, -1);

	  // Tree of available updates
  wxUpdateChecker = new wxTreeCtrl(myPanel, ID_UPDATE_TREE, wxDefaultPosition, wxDefaultSize, wxTR_DEFAULT_STYLE );
   // Create root node
	      wxtIDRootNode = wxUpdateChecker->AddRoot( wstrAppTitle);
                                         
		  // Create a child node for each update type
	wxtIDProgramNode = wxUpdateChecker->AppendItem( wxtIDRootNode, wstrProgramUpdate);
wxtIDProtocolNode = wxUpdateChecker->AppendItem( wxtIDRootNode, wstrProtocolUpdate);
wxtIDAppConfigNode = wxUpdateChecker->AppendItem( wxtIDRootNode, wstrAppConfigUpdate);

wxUpdateChecker->SetFocusedItem( wxtIDRootNode);

wxBoxSizer *hBox1 = new wxBoxSizer(wxHORIZONTAL);
hBox1->Add( wxUpdateChecker, 0, wxEXPAND);

// Read-only edit box to describe the selected tree item
wxBoxSizer * hBox2 = new wxBoxSizer(wxHORIZONTAL);
lblUpdateDescriptionPrompt = new wxStaticText(myPanel, wxID_ANY, wstrUpdateDescriptionPrompt);
txtUpdateDescription = new wxTextCtrl( myPanel, wxID_ANY);
  hBox2->Add( lblUpdateDescriptionPrompt, 0, wxEXPAND);
  hBox2->Add( txtUpdateDescription, 0, wxEXPAND);

// Buttons
btnUpdate = new wxButton(myPanel, wxID_OK, wstrUpdateButtonName, wxDefaultPosition, wxSize(70, 30));
btnUpdate->SetDefault();
btnCancel = new wxButton(myPanel, wxID_CANCEL, wstrCancelButtonName, wxDefaultPosition, wxSize(70, 30));

  wxBoxSizer *hBoxButtons = new wxBoxSizer(wxHORIZONTAL);
      hBoxButtons->Add(btnUpdate, 0, wxEXPAND);
  hBoxButtons->Add(btnCancel, 0, wxEXPAND);

  wxBoxSizer *vBox = new wxBoxSizer(wxVERTICAL);
vBox->Add( hBox1, 0, wxEXPAND);  
vBox->Add( hBox2, 0, wxEXPAND);  
  vBox->Add( hBoxButtons, 0, wxEXPAND);

myPanel->SetSizer( vBox);
vBox->SetSizeHints( this );
myPanel->Fit();
vBox->Fit( myPanel);
Centre();
this->Maximize();

// Check for the available updates and add leaf nodes appropriately
ListAppUpdates();
ListProtocolUpdates();
ListAppConfigUpdates();
}


UpdateChecker::~UpdateChecker()
{}


int UpdateChecker::ChildNumber( wxTreeItemId wxtIDCurrentNode)
{
wxTreeItemId myNode = wxtIDCurrentNode;
int nChildNumber = 0;

while (wxUpdateChecker->GetPrevSibling( myNode).IsOk())
{
	nChildNumber++;
	myNode = wxUpdateChecker->GetPrevSibling( myNode);
}

return nChildNumber;
}


int UpdateChecker::Level( wxTreeItemId myNode)
	{
int nLevel = 0;
wxTreeItemId wxtIDCurrentNode = myNode;
    
while (wxtIDCurrentNode .IsOk())
{
	wxtIDCurrentNode = wxUpdateChecker->GetItemParent( wxtIDCurrentNode);
	if (wxtIDCurrentNode.IsOk())
	{
	nLevel++;
	}
}
				
return nLevel;
}


void UpdateChecker::ListAppUpdates()
{
// fake value for testing
	int nUpdates = 1;

	// Do something to get the number of pending updates...

// Add the number of identified items to the tree
for (int i = 0; i < nUpdates; i++)
{
std::wstring wstrUpdateName = wstrTest;
	wxTreeItemId wxtIDNewNode = wxUpdateChecker->AppendItem( wxtIDProgramNode, wstrUpdateName);
}
}


void UpdateChecker::ListProtocolUpdates()
{}


void UpdateChecker::ListAppConfigUpdates()
{}


// Event handlers

void UpdateChecker::OnTreeSelect( wxTreeEvent& event)
{
}


void UpdateChecker::OnUpdate(wxCommandEvent& event)
{
EndModal( wxID_OK);
}


	void UpdateChecker::OnCancel(wxCommandEvent& event)
	{
	EndModal( wxID_CANCEL);
}
	

	// We need to implement an event table in which the events received by an UpdateChecker are routed to their respective handler functions 

	BEGIN_EVENT_TABLE(UpdateChecker, wxDialog)
EVT_TREE_SEL_CHANGED( ID_ACTION_TREE, UpdateChecker::OnTreeSelect)
EVT_BUTTON(wxID_OK,  UpdateChecker::OnUpdate)
	    EVT_BUTTON( wxID_CANCEL,                 UpdateChecker::OnCancel)
END_EVENT_TABLE()
	
		